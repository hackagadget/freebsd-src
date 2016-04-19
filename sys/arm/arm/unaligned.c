/*
 * Copyright 2015 Juniper Networks, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/lock.h>
#include <sys/mutex.h>
#include <sys/sysctl.h>

#include <machine/cpu.h>
#include <machine/cpu-v6.h>
#include <machine/frame.h>
#include <machine/unaligned.h>

SYSCTL_NODE(_vm_stats, OID_AUTO, unaligned, CTLFLAG_RW, 0,
    "Unaligned access statistics");
static int unaligned_user_count;
SYSCTL_INT(_vm_stats_unaligned, OID_AUTO, user, CTLFLAG_RD,
    &unaligned_user_count, 0, "Number of unaligned user accesses handled");
static int unaligned_unhandled;
SYSCTL_INT(_vm_stats_unaligned, OID_AUTO, unhandled, CTLFLAG_RD,
    &unaligned_unhandled, 0, "Number of unhandled unaligned accesses");

static int
handle_alignment_ldrstr(struct trapframe *tf, u_int far, uint32_t instr,
    int offset)
{
	register_t *reg;
	uint32_t val;
	u_int rd;

	reg = &tf->tf_r0;

	/* Read Rd register number */
	rd = INSN_RD_BITS(instr);

	if (LDRSTR_L_BIT(instr)) {
		/* LDR */
		if (copyin((void *)far, &val, sizeof(uint32_t *)))
			return (EFAULT);
		reg[rd] = val;
	} else {
		/* STR */
		val = reg[rd];
		if (copyout(&val, (void *)far, sizeof(uint32_t *)))
			return (EFAULT);
	}

	/* If P = 0 or W = 1, write-back to Rn */
	if (!LDRSTR_P_BIT(instr) || LDRSTR_W_BIT(instr)) {
		u_int rn = INSN_RN_BITS(instr);

		/* If U = 0, shift down */
		if (!LDRSTR_U_BIT(instr))
			offset = -offset;

		/* If P = 0, post-index */
		if (!LDRSTR_P_BIT(instr))
			far += offset;

		reg[rn] = far;
	}
	return 0;
}

static int
handle_alignment_ldmstm(struct trapframe *tf, u_int far, uint32_t instr)
{
	register_t *reg;
	uintptr_t addr, base, offset;
	uint32_t val;
	u_int bits, regnum, rn;

	reg = &tf->tf_r0;

	/* Read Rn register number */
	rn = INSN_RN_BITS(instr);

	/* R15 should not be used as the base register */
	if (rn == 15)
		return (EFAULT);

	/* Get base address from Rn register */
	base = (uintptr_t)reg[rn];

	/* Determine the number of registers that we need to transfer */
	bits = bitcount(instr & 0xff);
	bits = bits ? bits - 1 : 0;
	offset = bits * sizeof(register_t);

	/* P=1, pre-increment/decrement */
	if (LDRSTR_P_BIT(instr))
		base += LDRSTR_U_BIT(instr) ? sizeof(register_t) :
		    -sizeof(register_t);

	/* Determine starting address of the block data transfer */
	addr = LDRSTR_U_BIT(instr) ? base : base + offset;

	/* Process the register list and do the data transfer */
	for (regnum = 0; regnum < 16; regnum++) {
		/* If the register is not in the list, skip it */
		if ((instr & (1 << regnum)) == 0)
			continue;

		if (LDRSTR_L_BIT(instr)) {
			/* LDM */
			if (copyin((void *)addr, &val, sizeof(register_t *)))
				return (EFAULT);
			reg[regnum] = val;
		} else {
			/* STM */
			val = reg[regnum];
			if (copyout(&val, (void *)addr, sizeof(register_t *)))
				return (EFAULT);
		}
		/* Move to the next memory location */
		addr += sizeof(register_t);
	}

	/* P=0, post-increment/decrement */
	if (LDRSTR_P_BIT(instr))
		base += LDRSTR_U_BIT(instr) ? sizeof(register_t) :
		    -sizeof(register_t);

	/* W=1, write-back to Rn */
	if (LDRSTR_W_BIT(instr) &&
	    (!LDRSTR_L_BIT(instr) || (instr & (1 << rn)) == 0)) {
       		/* Account for the registers transfered in the base */
		base += LDRSTR_U_BIT(instr) ? offset : -offset;
		reg[rn] = base;
	}
	return (0);
}

#define	LDRSTR_SHIFT_BITS(_instr)	((_instr) & 0x0ff0)
#define	SHIFT_I_BIT(_instr)		((_instr) & (1 << 4))
#define	SHIFT_RS_BITS(_instr)		(((_instr) >> 8) & 0xf)
#define	SHIFT_IMM_BITS(_instr)		(((_instr) >> 7) & 0x1f)
#define	SHIFT_TYPE_BITS(_instr)		(((_instr) >> 5) & 0x3)

union offset_un {
	uint32_t uv;
	int32_t sv;
};

int
handle_alignment_fault(struct trapframe *tf, u_int fsr, u_int far)
{
	register_t *reg;
	register_t fault_pc;
	union offset_un off;
	uint32_t fault_instruction;
	u_int rm;
	int error = EFAULT;

	fault_pc = tf->tf_pc;
	/* Make sure the fault PC is aligned */
	if (__predict_false((fault_pc & 3) != 0))
		goto failed;

	/*
	 * No need to use copyin as the instruction was just executed, so we
	 * can access it directly from memory
	 */
	fault_instruction = *(u_int32_t *)fault_pc;
	reg = &tf->tf_r0;

	switch (fault_instruction & 0x0e000000) {
	case 0x04000000:
		/* ldr or str immediate offset */
		off.uv = fault_instruction & 0x0fff;
		error = handle_alignment_ldrstr(tf, far, fault_instruction,
		    off.uv);
		break;

	case 0x06000000:
		/* ldr or str register offset */
		rm = INSN_RM_BITS(fault_instruction);
		off.uv = reg[rm];

		if (LDRSTR_SHIFT_BITS(fault_instruction)) {
			/* Shift */
			u_int shiftval;

			if (SHIFT_I_BIT(fault_instruction)) {
				/* Register shift */
				u_int rs = SHIFT_RS_BITS(fault_instruction);

				/* Lower byte of Rs is the shift amount */
				shiftval = reg[rs] & 0xff;
			} else {
				/* Immediate shift */
				shiftval = SHIFT_IMM_BITS(fault_instruction);
			}

			switch (SHIFT_TYPE_BITS(fault_instruction)) {
			case 0:	/* logical left */
				off.uv <<= shiftval;
				break;

			case 1:	/* logical right */
				off.uv >>= shiftval;
				break;

			case 2:	/* arithmetic right */
				off.sv >>= shiftval;
				break;

			case 3:	/* rotate right */
				if (shiftval == 0) {
					/* rotate right extended (RRX) */
					off.uv >>= 1;

					if (tf->tf_spsr & PSR_C)
						off.uv |= (1 << 31);
				} else {
					off.uv = (off.uv >> shiftval) |
					    (off.uv << (32 - shiftval));
				}
			}
		}
		error = handle_alignment_ldrstr(tf, far, fault_instruction,
		    off.uv);
		break;

	case 0x08000000:
		/* ldm or stm block data transfer */
		error = handle_alignment_ldmstm(tf, far, fault_instruction);
		break;
	default:
		break;
	}

failed:
	if (error == 0) {
		/* Move to next instruction */
		tf->tf_pc += INSN_SIZE;

		atomic_add_int(&unaligned_user_count, 1);
	} else {
		atomic_add_int(&unaligned_unhandled, 1);
	}
	return (error);
}

