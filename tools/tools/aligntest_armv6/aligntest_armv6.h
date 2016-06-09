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

#ifndef __UNALIGNED_TEST_H__
#define __UNALIGNED_TEST_H__

#define	DATA_SIZE	16

#define	PTR_1_INDEX	1
#define	PTR_1_IMM	0

#define	PTR_7_INDEX	0
#define	PTR_7_IMM	7

#define	PTR_11_INDEX	9
#define	PTR_11_IMM	2

#define	PTR_11_LSL	4
#define	PTR_11_LSR	1
#define	PTR_11_ASR	1
#define	PTR_11_ROR	4

#ifndef __ASSEMBLER__
uint32_t	ldr_unaligned_ptr_imm(const char *);
uint32_t	ldr_ptr_unaligned_imm(const char *);
uint32_t	ldr_unaligned_ptr_unaligned_imm(const char *);
void		str_unaligned_ptr_imm(const char *, uint32_t);
void		str_ptr_unaligned_imm(const char *, uint32_t);
void		str_unaligned_ptr_unaligned_imm(const char *, uint32_t);

uint32_t	ldr_unaligned_ptr_postindex_imm(const char *, uintptr_t *);
uint32_t	str_unaligned_ptr_postindex_imm(const char *, uint32_t);

uint32_t	ldr_unaligned_ptr_reg(const char *, uint32_t);
uint32_t	ldr_ptr_unaligned_reg(const char *, uint32_t);
uint32_t	ldr_unaligned_ptr_unaligned_reg(const char *, uint32_t);
void		str_unaligned_ptr_reg(const char *, uint32_t, uint32_t);
void		str_ptr_unaligned_reg(const char *, uint32_t, uint32_t);
void		str_unaligned_ptr_unaligned_reg(const char *, uint32_t,
		    uint32_t);

uint32_t	ldr_unaligned_ptr_postindex_reg(const char *, uint32_t,
		    uintptr_t *);
uint32_t	ldr_unaligned_ptr_postindex_reg_lsl(const char *, uint32_t,
		    uintptr_t *);
uint32_t	ldr_unaligned_ptr_postindex_reg_lsr(const char *, uint32_t,
		    uintptr_t *);
uint32_t	ldr_unaligned_ptr_postindex_reg_asr(const char *, uint32_t,
		    uintptr_t *);
uint32_t	ldr_unaligned_ptr_postindex_reg_ror(const char *, uint32_t,
		    uintptr_t *);
uint32_t	str_unaligned_ptr_postindex_reg(const char *, uint32_t,
		    uint32_t);
uint32_t	str_unaligned_ptr_postindex_reg_lsl(const char *, uint32_t,
		    uint32_t);
uint32_t	str_unaligned_ptr_postindex_reg_lsr(const char *, uint32_t,
		    uint32_t);
uint32_t	str_unaligned_ptr_postindex_reg_asr(const char *, uint32_t,
		    uint32_t);
uint32_t	str_unaligned_ptr_postindex_reg_ror(const char *, uint32_t,
		    uint32_t);
#endif

#endif /* __UNALIGNED_TEST_H__ */
