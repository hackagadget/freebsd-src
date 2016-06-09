/*-
 * Copyright (c) 2015 Juniper Networks, Inc.
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

#include <machine/asm.h>

#include "unaligned_test.h"

/*********************
 * ldr/str [ptr,imm] *
 *********************/

/* Cases 1 & 2 - ldr unaligned ptr + imm */
ENTRY_NP(ldr_unaligned_ptr_imm)
	push	{fp, lr}
	add	r0, #PTR_1_INDEX
	ldr	r0, [r0, #PTR_1_IMM]
	pop	{fp, pc}
END(ldr_unaligned_ptr_imm)

/* Cases 3 & 4 - ldr ptr + unaligned imm*/
ENTRY_NP(ldr_ptr_unaligned_imm)
	push	{fp, lr}
	ldr	r0, [r0, #PTR_7_IMM]
	pop	{fp, pc}
END(ldr_ptr_unaligned_imm)

/* Cases 5 & 6 - ldr unaligned ptr + unaligned imm */
ENTRY_NP(ldr_unaligned_ptr_unaligned_imm)
	push	{fp, lr}
	add	r0, #PTR_11_INDEX
	ldr	r0, [r0, #PTR_11_IMM]
	pop	{fp, pc}
END(ldr_unaligned_ptr_unaligned_imm)

/* Cases 7 & 8 - str unaligned ptr + imm */
ENTRY_NP(str_unaligned_ptr_imm)
	push	{fp, lr}
	add	r0, #PTR_1_INDEX
	str	r1, [r0, #PTR_1_IMM]
	pop	{fp, pc}
END(str_unaligned_ptr_imm)

/* Cases 9 & 10 - str ptr + unaligned imm */
ENTRY_NP(str_ptr_unaligned_imm)
	push	{fp, lr}
	str	r1, [r0, #PTR_7_IMM]
	pop	{fp, pc}
END(str_unaligned_ptr_imm)

/* Cases 11 & 12 - str unaligned ptr + unaligned imm */
ENTRY_NP(str_unaligned_ptr_unaligned_imm)
	push	{fp, lr}
	add	r0, #PTR_11_INDEX
	str	r1, [r0, #PTR_11_IMM]
	pop	{fp, pc}
END(str_unaligned_ptr_imm)

/*********************
 * ldr/str [ptr],imm *
 *********************/

/* Cases 1 & 2 - ldr unaligned ptr + post-index imm */
ENTRY_NP(ldr_unaligned_ptr_postindex_imm)
	push	{fp, lr}
	mov	r2, r0
	add	r2, #PTR_11_INDEX
	ldr	r0, [r2], #PTR_11_IMM
	str	r2, [r1]
	pop	{fp, pc}
END(ldr_unaligned_ptr_postindex_imm)

/* Cases 3 & 4 - str unaligned ptr + post-index imm */
ENTRY_NP(str_unaligned_ptr_postindex_imm)
	push	{r6, r7, fp, lr}
	add	r0, #PTR_11_INDEX
	str	r1, [r0], #PTR_11_IMM
	pop	{r6, r7, fp, pc}
END(str_unaligned_ptr_postindex_imm)

/*******************************
 * ldr/str [ptr,reg{,<shift>}] *
 *******************************/

/* Cases 1 & 2 - ldr unaligned ptr + reg */
ENTRY_NP(ldr_unaligned_ptr_reg)
	push	{fp, lr}
	add	r0, #PTR_1_INDEX
	ldr	r0, [r0, r1]
	pop	{fp, pc}
END(ldr_unaligned_ptr_reg)

/* Cases 3 & 4 - ldr ptr + unaligned reg */
ENTRY_NP(ldr_ptr_unaligned_reg)
	push	{fp, lr}
	ldr	r0, [r0, r1]
	pop	{fp, pc}
END(ldr_ptr_unaligned_reg)

/* Cases 5 & 6 - ldr unaligned ptr + unaligned reg */
ENTRY_NP(ldr_unaligned_ptr_unaligned_reg)
	push	{fp, lr}
	add	r0, #PTR_11_INDEX
	ldr	r0, [r0, r1]
	pop	{fp, pc}
END(ldr_ptr_unaligned_reg)

/* Cases 7 & 8 - str unaligned ptr + reg */
ENTRY_NP(str_unaligned_ptr_reg)
	push	{fp, lr}
	add	r0, #PTR_1_INDEX
	str	r2, [r0, r1]
	pop	{fp, pc}
END(str_unaligned_ptr_reg)

/* Cases 9 & 10 - str ptr + unaligned reg */
ENTRY_NP(str_ptr_unaligned_reg)
	push	{fp, lr}
	str	r2, [r0, r1]
	pop	{fp, pc}
END(str_ptr_unaligned_reg)

/* Cases 11 & 12 - str unaligned ptr + unaligned reg */
ENTRY_NP(str_unaligned_ptr_unaligned_reg)
	push	{fp, lr}
	add	r0, #PTR_11_INDEX
	str	r2, [r0, r1]
	pop	{fp, pc}
END(str_ptr_unaligned_reg)

/*******************************
 * ldr/str [ptr],reg{,<shift>} *
 *******************************/

/* Cases 1 & 2 - ldr unaligned ptr + post-index reg */
ENTRY_NP(ldr_unaligned_ptr_postindex_reg)
	push	{fp, lr}
	mov	r3, r0
	add	r3, #PTR_1_INDEX
	ldr	r0, [r3], r1
	str	r3, [r2]
	pop	{fp, pc}
END(ldr_unaligned_ptr_postindex_reg)

/* Cases 3 & 4 - str unaligned ptr + post-index reg */
ENTRY_NP(str_unaligned_ptr_postindex_reg)
	push	{fp, lr}
	add	r0, #PTR_1_INDEX
	str	r2, [r0], r1
	pop	{fp, pc}
END(str_unaligned_ptr_postindex_reg)

/* Cases 5 & 6 - ldr unaligned ptr + post-index reg LSL */
ENTRY_NP(ldr_unaligned_ptr_postindex_reg_lsl)
	push	{fp, lr}
	mov	r3, r0
	add	r3, #PTR_11_INDEX
	ldr	r0, [r3], r1, lsl #PTR_11_LSL
	str	r3, [r2]
	pop	{fp, pc}
END(ldr_unaligned_ptr_postindex_reg_lsl)

/* Cases 7 & 8 - str unaligned ptr + post-index reg LSL */
ENTRY_NP(str_unaligned_ptr_postindex_reg_lsl)
	push	{fp, lr}
	add	r0, #PTR_11_INDEX
	str	r2, [r0], r1, lsl #PTR_11_LSL
	pop	{fp, pc}
END(str_unaligned_ptr_postindex_reg_lsl)

/* Cases 9 & 10 - ldr unaligned ptr + post-index reg LSR */
ENTRY_NP(ldr_unaligned_ptr_postindex_reg_lsr)
	push	{fp, lr}
	mov	r3, r0
	add	r3, #PTR_11_INDEX
	ldr	r0, [r3], r1, lsr #PTR_11_LSR
	str	r3, [r2]
	pop	{fp, pc}
END(ldr_unaligned_ptr_postindex_reg_lsr)

/* Cases 11 & 12 - str unaligned ptr + post-index reg LSR */
ENTRY_NP(str_unaligned_ptr_postindex_reg_lsr)
	push	{fp, lr}
	add	r0, #PTR_11_INDEX
	str	r2, [r0], r1, lsr #PTR_11_LSR
	pop	{fp, pc}
END(str_unaligned_ptr_postindex_reg_lsr)

/* Cases 13 & 14 - ldr unaligned ptr + post-index reg ASR */
ENTRY_NP(ldr_unaligned_ptr_postindex_reg_asr)
	push	{fp, lr}
	mov	r3, r0
	add	r3, #PTR_11_INDEX
	ldr	r0, [r3], r1, asr #PTR_11_ASR
	str	r3, [r2]
	pop	{fp, pc}
END(ldr_unaligned_ptr_postindex_reg_asr)

/* Cases 15 & 16 - str unaligned ptr + post-index reg ASR */
ENTRY_NP(str_unaligned_ptr_postindex_reg_asr)
	push	{fp, lr}
	add	r0, #PTR_11_INDEX
	str	r2, [r0], r1, asr #PTR_11_ASR
	pop	{fp, pc}
END(str_unaligned_ptr_postindex_reg_asr)

/* Cases 17 & 18 - ldr unaligned ptr + post-index reg ROR */
ENTRY_NP(ldr_unaligned_ptr_postindex_reg_ror)
	push	{fp, lr}
	mov	r3, r0
	add	r3, #PTR_11_INDEX
	ldr	r0, [r3], r1, ror #PTR_11_ROR
	str	r3, [r2]
	pop	{fp, pc}
END(ldr_unaligned_ptr_postindex_reg_ror)

/* Cases 19 & 20 - str unaligned ptr + post-index reg ROR */
ENTRY_NP(str_unaligned_ptr_postindex_reg_ror)
	push	{fp, lr}
	add	r0, #PTR_11_INDEX
	str	r2, [r0], r1, ror #PTR_11_ROR
	pop	{fp, pc}
END(str_unaligned_ptr_postindex_reg_ror)

