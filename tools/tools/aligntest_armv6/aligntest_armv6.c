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

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/endian.h>
#include <sys/types.h>
#include <sys/sysctl.h>

#include <machine/param.h>

#include <err.h>
#include <errno.h>
#include <getopt.h>
#include <inttypes.h>
#include <setjmp.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "unaligned_test.h"

static int tests_failed;
static int tests_passed;
static int total_tests;
static int saved_allow;
static jmp_buf env;
static volatile int longjmp_ok;
static volatile int sigbus_triggered;

static const u_char c_data[DATA_SIZE] __aligned(CACHE_LINE_SIZE) = {
	/* &ptr[1] + 0 test values are 1..4 */
	[1] = 0x12,
	[2] = 0x34,
	[3] = 0x56,
	[4] = 0x78,
#define	PTR_1_VAL	0x12345678
	/* &ptr[0] + 7 test values are 7..8 */
	[7] = 0x81,
	[8] = 0x62,
	[9] = 0x44,
	[10] = 0x26,
#define	PTR_7_VAL	0x81624426
	/* &ptr[9] + 2 test values are 11..14 */
	[11] = 0xf0,
	[12] = 0x5a,
	[13] = 0x96,
	[14] = 0x0f,
#define PTR_11_VAL	0xf05a960f
#define PTR_11_ALT_VAL	0x4426f05a
};

static void
sigbus_handler(int signum)
{

	sigbus_triggered = 1;
	if (longjmp_ok)
		longjmp(env, 1);
}

static int
get_allow_unaligned(void)
{
	int allow;
	size_t len;

	len = sizeof(allow);
	if (sysctlbyname("vm.user.unaligned_access", &allow, &len, NULL, 0)
	    == -1)
		err(1, "sysctlbyname(vm.user.unaligned_access)");

	return (allow);
}

static int
set_allow_unaligned(int allow)
{
	size_t len;

	len = sizeof(allow);
	if (sysctlbyname("vm.user.unaligned_access", NULL, 0, &allow, len)
	    == -1)
		return (-1);

	return (0);
}

static void
outta_here(void)
{

	(void) set_allow_unaligned(saved_allow);

	printf("\nTotal tests run: %d\n", total_tests);
	printf("   Passed tests: %d\n", tests_passed);
	printf("   Failed tests: %d\n", tests_failed);
}

#define	WITHOUT_FIXUP	0
#define	WITH_FIXUP	1

#define	FAIL_TESTCASE(_fmt, ...)					\
	do {								\
		printf("FAIL - " _fmt, ##__VA_ARGS__);			\
		tests_failed++;						\
		total_tests++;						\
	} while (0)

#define PASS_TESTCASE()							\
	do {								\
		printf("PASS\n");					\
		tests_passed++;						\
		total_tests++;						\
	} while (0)

#define	CHECK_TESTCASE(_cond, _fmt, ...)				\
	do {								\
		if (!(_cond))						\
			FAIL_TESTCASE(_fmt, ##__VA_ARGS__);		\
		else							\
			PASS_TESTCASE();				\
	} while (0)

#define	TRY_TESTCASE(_n, _a)						\
	do {								\
		sigbus_triggered = 0;					\
		if (set_allow_unaligned((_a)) == -1)			\
			err(1, "FATAL - set_allow_unaligned");		\
		if (setjmp(env) != 0)					\
			goto _n;					\
		longjmp_ok = 1;						\
		if (signal(SIGBUS, sigbus_handler) == SIG_ERR)		\
			err(1,						\
			    "FATAL - signal(SIGBUS, sigbus_handler)");	\
	} while (0)

#define	CATCH_TESTCASE(_n)						\
	do {								\
	_n:								\
		longjmp_ok = 0;						\
		if (signal(SIGBUS, SIG_DFL) == SIG_ERR)			\
			err(1, "FATAL - signal(SIGBUS, SIG_DFL)");	\
	} while (0)

static void
unaligned_imm_preindex_test(void)
{
	static u_char data[DATA_SIZE] __aligned(CACHE_LINE_SIZE);
	uint32_t expval, val;
	int indx;

	printf("BEGIN - immediate offset pre-index test\n");

	/*****************************************************
	 * Case #A.1: ldr unaligned ptr + imm w/o trap fixup *
	 *****************************************************/
	printf("[Case A.1: ldr unaligned ptr + imm w/o trap fixup]\n");

	val = 0;
	TRY_TESTCASE(ldr_imm1, WITHOUT_FIXUP);
	val = ldr_unaligned_ptr_imm(c_data);
	CATCH_TESTCASE(ldr_imm1);

	/* Make sure we received a SIGBUS signal */
	CHECK_TESTCASE(sigbus_triggered, "SIGBUS not seen\n");

	/****************************************************
	 * Case #A.2: ldr unaligned ptr + imm w/ trap fixup *
	 ****************************************************/
	printf("[Case A.2: ldr unaligned ptr + imm w/ trap fixup]\n");

	val = 0;
	TRY_TESTCASE(ldr_imm2, WITH_FIXUP);
	val = ldr_unaligned_ptr_imm(c_data);
	CATCH_TESTCASE(ldr_imm2);

	/* Make sure we did not get a SIGBUS signal */
	if (sigbus_triggered) {
		FAIL_TESTCASE("SIGBUS seen\n");
		goto done_imm2;
	}

	expval = htobe32(PTR_1_VAL);
	CHECK_TESTCASE(val == expval,
	    "value %" PRIx32 " not expected %" PRIx32 "\n", val, expval);
done_imm2:

	/*****************************************************
	 * Case #A.3: ldr ptr + unaligned imm w/o trap fixup *
	 *****************************************************/
	printf("[Case A.3: ldr ptr + unaligned imm w/o trap fixup]\n");

	val = 0;
	TRY_TESTCASE(ldr_imm3, WITHOUT_FIXUP);
	val = ldr_ptr_unaligned_imm(c_data);
	CATCH_TESTCASE(ldr_imm3);

	/* Make sure we received a SIGBUS signal */
	CHECK_TESTCASE(sigbus_triggered, "SIGBUS not seen\n");

	/****************************************************
	 * Case #A.4: ldr ptr + unaligned imm w/ trap fixup *
	 ****************************************************/
	printf("[Case A.4: ldr ptr + unaligned imm w/ trap fixup]\n");

	val = 0;
	TRY_TESTCASE(ldr_imm4, WITH_FIXUP);
	val = ldr_ptr_unaligned_imm(c_data);
	CATCH_TESTCASE(ldr_imm4);

	/* Make sure we did not get a SIGBUS signal */
	if (sigbus_triggered) {
		FAIL_TESTCASE("SIGBUS seen\n");
		goto done_imm4;
	}

	expval = htobe32(PTR_7_VAL);
	CHECK_TESTCASE(val == expval,
	    "value %" PRIx32 " not expected %" PRIx32 "\n", val, expval);
done_imm4:

	/***************************************************************
	 * Case #A.5: ldr unaligned ptr + unaligned imm w/o trap fixup *
	 ***************************************************************/
	printf("[Case A.5: ldr unaligned ptr + unaligned imm w/o trap fixup]\n");

	val = 0;
	TRY_TESTCASE(ldr_imm5, WITHOUT_FIXUP);
	val = ldr_unaligned_ptr_unaligned_imm(c_data);
	CATCH_TESTCASE(ldr_imm5);

	/* Make sure we received a SIGBUS signal */
	CHECK_TESTCASE(sigbus_triggered, "SIGBUS not seen\n");

	/**************************************************************
	 * Case #A.6: ldr unaligned ptr + unaligned imm w/ trap fixup *
	 **************************************************************/
	printf("[Case A.6: ldr unaligned ptr + unaligned imm w/ trap fixup]\n");

	val = 0;
	TRY_TESTCASE(ldr_imm6, WITH_FIXUP);
	val = ldr_unaligned_ptr_unaligned_imm(c_data);
	CATCH_TESTCASE(ldr_imm6);

	/* Make sure we did not get a SIGBUS signal */
	if (sigbus_triggered) {
		FAIL_TESTCASE("SIGBUS seen\n");
		goto done_imm6;
	}

	expval = htobe32(PTR_11_VAL);
	CHECK_TESTCASE(val == expval,
	    "value %" PRIx32 " not expected %" PRIx32 "\n", val, expval);
done_imm6:

	/*****************************************************
	 * Case #A.7: str unaligned ptr + imm w/o trap fixup *
	 *****************************************************/
	printf("[Case A.7: str unaligned ptr + imm w/o trap fixup]\n");

	bzero(data, sizeof(data));
	TRY_TESTCASE(str_imm7, WITHOUT_FIXUP);
	str_unaligned_ptr_imm(data, htobe32(PTR_1_VAL));
	CATCH_TESTCASE(str_imm7);

	/* Make sure we received a SIGBUS signal */
	CHECK_TESTCASE(sigbus_triggered, "SIGBUS not seen\n");

	/****************************************************
	 * Case #A.8: str unaligned ptr + imm w/ trap fixup *
	 ****************************************************/
	printf("[Case A.8: str unaligned ptr + imm w/ trap fixup]\n");

	bzero(data, sizeof(data));
	TRY_TESTCASE(str_imm8, WITH_FIXUP);
	str_unaligned_ptr_imm(data, htobe32(PTR_1_VAL));
	CATCH_TESTCASE(str_imm8);

	/* Make sure we did not get a SIGBUS signal */
	if (sigbus_triggered) {
		FAIL_TESTCASE("SIGBUS seen\n");
		goto done_imm8;
	}

	indx = PTR_1_INDEX + PTR_1_IMM;
	CHECK_TESTCASE(!memcmp(&c_data[indx], &data[indx], sizeof(uint32_t)),
	    "value %02x%02x%02x%02x not expected %02x%02x%02x%02x\n",
	    data[indx], data[indx + 1], data[indx + 2], data[indx + 3],
	    c_data[indx], c_data[indx + 1], c_data[indx + 2], c_data[indx + 3]);
done_imm8:

	/*****************************************************
	 * Case #A.9: str ptr + unaligned imm w/o trap fixup *
	 *****************************************************/
	printf("[Case A.9: str ptr + unaligned imm w/o trap fixup]\n");

	bzero(data, sizeof(data));
	TRY_TESTCASE(str_imm9, WITHOUT_FIXUP);
	str_ptr_unaligned_imm(data, htobe32(PTR_7_VAL));
	CATCH_TESTCASE(str_imm9);

	/* Make sure we received a SIGBUS signal */
	CHECK_TESTCASE(sigbus_triggered, "SIGBUS not seen\n");

	/*****************************************************
	 * Case #A.10: str ptr + unaligned imm w/ trap fixup *
	 *****************************************************/
	printf("[Case A.10: str ptr + unaligned imm w/ trap fixup]\n");

	bzero(data, sizeof(data));
	TRY_TESTCASE(str_imm10, WITH_FIXUP);
	str_ptr_unaligned_imm(data, htobe32(PTR_7_VAL));
	CATCH_TESTCASE(str_imm10);

	/* Make sure we did not get a SIGBUS signal */
	if (sigbus_triggered) {
		FAIL_TESTCASE("SIGBUS seen\n");
		goto done_imm10;
	}

	indx = PTR_7_INDEX + PTR_7_IMM;
	CHECK_TESTCASE(!memcmp(&c_data[indx], &data[indx], sizeof(uint32_t)),
	    "value %02x%02x%02x%02x not expected %02x%02x%02x%02x\n",
	    data[indx], data[indx + 1], data[indx + 2], data[indx + 3],
	    c_data[indx], c_data[indx + 1], c_data[indx + 2], c_data[indx + 3]);
done_imm10:

	/****************************************************************
	 * Case #A.11: str unaligned ptr + unaligned imm w/o trap fixup *
	 ****************************************************************/
	printf("[Case A.11: str unaligned ptr + unaligned imm w/o trap fixup]\n");

	bzero(data, sizeof(data));
	TRY_TESTCASE(str_imm11, WITHOUT_FIXUP);
	str_unaligned_ptr_unaligned_imm(data, htobe32(PTR_11_VAL));
	CATCH_TESTCASE(str_imm11);

	/* Make sure we received a SIGBUS signal */
	CHECK_TESTCASE(sigbus_triggered, "SIGBUS not seen\n");

	/***************************************************************
	 * Case #A.12: str unaligned ptr + unaligned imm w/ trap fixup *
	 ***************************************************************/
	printf("[Case A.12: str unaligned ptr + unaligned imm w/ trap fixup]\n");

	bzero(data, sizeof(data));
	TRY_TESTCASE(str_imm12, WITH_FIXUP);
	str_unaligned_ptr_unaligned_imm(data, htobe32(PTR_11_VAL));
	CATCH_TESTCASE(str_imm12);

	/* Make sure we did not get a SIGBUS signal */
	if (sigbus_triggered) {
		FAIL_TESTCASE("SIGBUS seen\n");
		goto done_imm12;
	}

	indx = PTR_11_INDEX + PTR_11_IMM;
	CHECK_TESTCASE(!memcmp(&c_data[indx], &data[indx], sizeof(uint32_t)),
	    "value %02x%02x%02x%02x not expected %02x%02x%02x%02x\n",
	    data[indx], data[indx + 1], data[indx + 2], data[indx + 3],
	    c_data[indx], c_data[indx + 1], c_data[indx + 2], c_data[indx + 3]);
done_imm12:

	printf("END - immediate offset pre-index test\n\n");
}

static void
unaligned_imm_postindex_test(void)
{
	static u_char data[DATA_SIZE] __aligned(CACHE_LINE_SIZE);
	uintptr_t ptrval;
	uint32_t expval, val;
	int indx;

	printf("BEGIN - immediate offset post-index test\n");

	/****************************************************************
	 * Case #B.1: ldr unaligned ptr + post-index imm w/o trap fixup *
	 ****************************************************************/
	printf("[Case B.1: ldr unaligned ptr + post-index imm w/o trap fixup]\n");

	val = 0;
	ptrval = 0;
	TRY_TESTCASE(ldr_pi_imm1, WITHOUT_FIXUP);
	val = ldr_unaligned_ptr_postindex_imm(c_data, &ptrval);
	CATCH_TESTCASE(ldr_pi_imm1);

	/* Make sure we received a SIGBUS signal */
	CHECK_TESTCASE(sigbus_triggered, "SIGBUS not seen\n");

	/***************************************************************
	 * Case #B.2: ldr unaligned ptr + post-index imm w/ trap fixup *
	 ***************************************************************/
	printf("[Case B.2: ldr unaligned ptr + post-index imm w/ trap fixup]\n");

	val = 0;
	ptrval = 0;
	TRY_TESTCASE(ldr_pi_imm2, WITH_FIXUP);
	val = ldr_unaligned_ptr_postindex_imm(c_data, &ptrval);
	CATCH_TESTCASE(ldr_pi_imm2);

	/* Make sure we did not get a SIGBUS signal */
	if (sigbus_triggered) {
		FAIL_TESTCASE("SIGBUS seen\n");
		goto done_pi_imm2;
	}

	expval = (uint32_t)&c_data[PTR_11_INDEX + PTR_11_IMM];
	if (ptrval != expval) {
		FAIL_TESTCASE("ptrval %08x != %08x c_data[%d + %d]\n",
		    ptrval, expval, PTR_11_INDEX, PTR_11_IMM);
		goto done_pi_imm2;
	}

	expval = htobe32(PTR_11_ALT_VAL);
	CHECK_TESTCASE(val == expval,
	    "value %" PRIx32 " not expected %" PRIx32 "\n", val, expval);

done_pi_imm2:

	/****************************************************************
	 * Case #B.3: str unaligned ptr + post-index imm w/o trap fixup *
	 ****************************************************************/
	printf("[Case B.3: str unaligned ptr + post-index imm w/o trap fixup]\n");

	bzero(data, sizeof(data));
	ptrval = 0;
	TRY_TESTCASE(str_pi_imm3, WITHOUT_FIXUP);
	ptrval = str_unaligned_ptr_postindex_imm(data, htobe32(PTR_11_ALT_VAL));
	CATCH_TESTCASE(str_pi_imm3);

	/* Make sure we received a SIGBUS signal */
	CHECK_TESTCASE(sigbus_triggered, "SIGBUS not seen\n");

	/***************************************************************
	 * Case #B.4: str unaligned ptr + post-index imm w/ trap fixup *
	 ***************************************************************/
	printf("[Case B.4: str unaligned ptr + post-index imm w/ trap fixup]\n");

	bzero(data, sizeof(data));
	ptrval = 0;
	TRY_TESTCASE(str_pi_imm4, WITH_FIXUP);
	ptrval = str_unaligned_ptr_postindex_imm(data, htobe32(PTR_11_ALT_VAL));
	CATCH_TESTCASE(str_pi_imm4);

	/* Make sure we did not get a SIGBUS signal */
	if (sigbus_triggered) {
		FAIL_TESTCASE("SIGBUS seen\n");
		goto done_pi_imm4;
	}

	expval = (uint32_t)&data[PTR_11_INDEX + PTR_11_IMM];
	if (ptrval != expval) {
		FAIL_TESTCASE("ptrval %08x != %08x data[%d + %d]\n",
		    ptrval, expval, PTR_11_INDEX, PTR_11_IMM);
		goto done_pi_imm4;
	}

	indx = PTR_11_INDEX;
	CHECK_TESTCASE(!memcmp(&c_data[indx], &data[indx], sizeof(uint32_t)),
	    "value %02x%02x%02x%02x not expected %02x%02x%02x%02x\n",
	    data[indx], data[indx + 1], data[indx + 2], data[indx + 3],
	    c_data[indx], c_data[indx + 1], c_data[indx + 2], c_data[indx + 3]);
done_pi_imm4:

	printf("END - immediate offset post-index test\n\n");
}

static void
unaligned_imm_test(void)
{

	unaligned_imm_preindex_test();
	unaligned_imm_postindex_test();
}

static void
unaligned_reg_preindex_test(void)
{
	static u_char data[DATA_SIZE] __aligned(CACHE_LINE_SIZE);
	uint32_t expval, val;
	int indx;

	printf("BEGIN - register offset pre-index test\n");

	/*****************************************************
	 * Case #C.1: ldr unaligned ptr + reg w/o trap fixup *
	 *****************************************************/
	printf("[Case C.1: ldr unaligned ptr + reg w/o trap fixup]\n");

	val = 0;
	TRY_TESTCASE(ldr_reg1, WITHOUT_FIXUP);
	val = ldr_unaligned_ptr_reg(c_data, PTR_1_IMM);
	CATCH_TESTCASE(ldr_reg1);

	/* Make sure we received a SIGBUS signal */
	CHECK_TESTCASE(sigbus_triggered, "SIGBUS not seen\n");

	/****************************************************
	 * Case #C.2: ldr unaligned ptr + reg w/ trap fixup *
	 ****************************************************/
	printf("[Case C.2: ldr unaligned ptr + reg w/ trap fixup]\n");

	val = 0;
	TRY_TESTCASE(ldr_reg2, WITH_FIXUP);
	val = ldr_unaligned_ptr_reg(c_data, PTR_1_IMM);
	CATCH_TESTCASE(ldr_reg2);

	/* Make sure we did not get a SIGBUS signal */
	if (sigbus_triggered) {
		FAIL_TESTCASE("SIGBUS seen\n");
		goto done_reg2;
	}

	expval = htobe32(PTR_1_VAL);
	CHECK_TESTCASE(val == expval,
	    "value %" PRIx32 " not expected %" PRIx32 "\n", val, expval);
done_reg2:

	/*****************************************************
	 * Case #C.3: ldr ptr + unaligned reg w/o trap fixup *
	 *****************************************************/
	printf("[Case C.3: ldr ptr + unaligned reg w/o trap fixup]\n");

	val = 0;
	TRY_TESTCASE(ldr_reg3, WITHOUT_FIXUP);
	val = ldr_ptr_unaligned_reg(c_data, PTR_7_IMM);
	CATCH_TESTCASE(ldr_reg3);

	/* Make sure we received a SIGBUS signal */
	CHECK_TESTCASE(sigbus_triggered, "SIGBUS not seen\n");

	/****************************************************
	 * Case #C.4: ldr ptr + unaligned reg w/ trap fixup *
	 ****************************************************/
	printf("[Case C.4: ldr ptr + unaligned reg w/ trap fixup]\n");

	val = 0;
	TRY_TESTCASE(ldr_reg4, WITH_FIXUP);
	val = ldr_ptr_unaligned_reg(c_data, PTR_7_IMM);
	CATCH_TESTCASE(ldr_reg4);

	/* Make sure we did not get a SIGBUS signal */
	if (sigbus_triggered) {
		FAIL_TESTCASE("SIGBUS seen\n");
		goto done_reg4;
	}

	expval = htobe32(PTR_7_VAL);
	CHECK_TESTCASE(val == expval,
	    "value %" PRIx32 " not expected %" PRIx32 "\n", val, expval);
done_reg4:

	/*****************************************************
	 * Case #C.5: ldr ptr + unaligned reg w/o trap fixup *
	 *****************************************************/
	printf("[Case C.5: ldr ptr + unaligned reg w/o trap fixup]\n");

	val = 0;
	TRY_TESTCASE(ldr_reg5, WITHOUT_FIXUP);
	val = ldr_unaligned_ptr_unaligned_reg(c_data, PTR_11_IMM);
	CATCH_TESTCASE(ldr_reg5);

	/* Make sure we received a SIGBUS signal */
	CHECK_TESTCASE(sigbus_triggered, "SIGBUS not seen\n");

	/****************************************************
	 * Case #C.6: ldr ptr + unaligned reg w/ trap fixup *
	 ****************************************************/
	printf("[Case C.6: ldr ptr + unaligned reg w/ trap fixup]\n");

	val = 0;
	TRY_TESTCASE(ldr_reg6, WITH_FIXUP);
	val = ldr_unaligned_ptr_unaligned_reg(c_data, PTR_11_IMM);
	CATCH_TESTCASE(ldr_reg6);

	/* Make sure we did not get a SIGBUS signal */
	if (sigbus_triggered) {
		FAIL_TESTCASE("SIGBUS seen\n");
		goto done_reg6;
	}

	expval = htobe32(PTR_11_VAL);
	CHECK_TESTCASE(val == expval,
	    "value %" PRIx32 " not expected %" PRIx32 "\n", val, expval);
done_reg6:

	/*****************************************************
	 * Case #C.7: str unaligned ptr + reg w/o trap fixup *
	 *****************************************************/
	printf("[Case C.7: str unaligned ptr + reg w/o trap fixup]\n");

	bzero(data, sizeof(data));
	TRY_TESTCASE(str_reg7, WITHOUT_FIXUP);
	str_unaligned_ptr_reg(data, PTR_1_IMM, htobe32(PTR_1_VAL));
	CATCH_TESTCASE(str_reg7);

	/* Make sure we received a SIGBUS signal */
	CHECK_TESTCASE(sigbus_triggered, "SIGBUS not seen\n");

	/****************************************************
	 * Case #C.8: str unaligned ptr + reg w/ trap fixup *
	 ****************************************************/
	printf("[Case C.8: str unaligned ptr + reg w/ trap fixup]\n");

	bzero(data, sizeof(data));
	TRY_TESTCASE(str_reg8, WITH_FIXUP);
	str_unaligned_ptr_reg(data, PTR_1_IMM, htobe32(PTR_1_VAL));
	CATCH_TESTCASE(str_reg8);

	/* Make sure we did not get a SIGBUS signal */
	if (sigbus_triggered) {
		FAIL_TESTCASE("SIGBUS seen\n");
		goto done_reg8;
	}

	indx = PTR_1_INDEX + PTR_1_IMM;
	CHECK_TESTCASE(!memcmp(&c_data[indx], &data[indx], sizeof(uint32_t)),
	    "value %02x%02x%02x%02x not expected %02x%02x%02x%02x\n",
	    data[indx], data[indx + 1], data[indx + 2], data[indx + 3],
	    c_data[indx], c_data[indx + 1], c_data[indx + 2], c_data[indx + 3]);
done_reg8:

	/*****************************************************
	 * Case #C.9: str ptr + unaligned reg w/o trap fixup *
	 *****************************************************/
	printf("[Case C.9: str ptr + unaligned reg w/o trap fixup]\n");

	bzero(data, sizeof(data));
	TRY_TESTCASE(str_reg9, WITHOUT_FIXUP);
	str_ptr_unaligned_reg(data, PTR_7_IMM, htobe32(PTR_7_VAL));
	CATCH_TESTCASE(str_reg9);

	/* Make sure we received a SIGBUS signal */
	CHECK_TESTCASE(sigbus_triggered, "SIGBUS not seen\n");

	/****************************************************
	 * Case #C.10: str ptr + unaligned reg w/ trap fixup *
	 ****************************************************/
	printf("[Case C.10: str ptr + unaligned reg w/ trap fixup]\n");

	bzero(data, sizeof(data));
	TRY_TESTCASE(str_reg10, WITH_FIXUP);
	str_ptr_unaligned_reg(data, PTR_7_IMM, htobe32(PTR_7_VAL));
	CATCH_TESTCASE(str_reg10);

	/* Make sure we did not get a SIGBUS signal */
	if (sigbus_triggered) {
		FAIL_TESTCASE("SIGBUS seen\n");
		goto done_reg10;
	}

	indx = PTR_7_INDEX + PTR_7_IMM;
	CHECK_TESTCASE(!memcmp(&c_data[indx], &data[indx], sizeof(uint32_t)),
	    "value %02x%02x%02x%02x not expected %02x%02x%02x%02x\n",
	    data[indx], data[indx + 1], data[indx + 2], data[indx + 3],
	    c_data[indx], c_data[indx + 1], c_data[indx + 2], c_data[indx + 3]);
done_reg10:

	/******************************************************
	 * Case #C.11: str ptr + unaligned reg w/o trap fixup *
	 ******************************************************/
	printf("[Case C.11: str ptr + unaligned reg w/o trap fixup]\n");

	bzero(data, sizeof(data));
	TRY_TESTCASE(str_reg11, WITHOUT_FIXUP);
	str_unaligned_ptr_unaligned_reg(data, PTR_11_IMM, htobe32(PTR_11_VAL));
	CATCH_TESTCASE(str_reg11);

	/* Make sure we received a SIGBUS signal */
	CHECK_TESTCASE(sigbus_triggered, "SIGBUS not seen\n");

	/*****************************************************
	 * Case #C.12: str ptr + unaligned reg w/ trap fixup *
	 *****************************************************/
	printf("[Case C.12: str ptr + unaligned reg w/ trap fixup]\n");

	bzero(data, sizeof(data));
	TRY_TESTCASE(str_reg12, WITH_FIXUP);
	str_unaligned_ptr_unaligned_reg(data, PTR_11_IMM, htobe32(PTR_11_VAL));
	CATCH_TESTCASE(str_reg12);

	/* Make sure we did not get a SIGBUS signal */
	if (sigbus_triggered) {
		FAIL_TESTCASE("SIGBUS seen\n");
		goto done_reg12;
	}

	indx = PTR_11_INDEX + PTR_11_IMM;
	CHECK_TESTCASE(!memcmp(&c_data[indx], &data[indx], sizeof(uint32_t)),
	    "value %02x%02x%02x%02x not expected %02x%02x%02x%02x\n",
	    data[indx], data[indx + 1], data[indx + 2], data[indx + 3],
	    c_data[indx], c_data[indx + 1], c_data[indx + 2], c_data[indx + 3]);
done_reg12:

	printf("END - register offset pre-index test\n\n");
}

static void
unaligned_reg_postindex_test(void)
{
	static u_char data[DATA_SIZE] __aligned(CACHE_LINE_SIZE);
	uintptr_t ptrval;
	uint32_t expval, hi, lo, val;
	int indx;

	printf("BEGIN - register offset post-index test\n");

	/****************************************************************
	 * Case #D.1: ldr unaligned ptr + post-index reg w/o trap fixup *
	 ****************************************************************/
	printf("[Case D.1: ldr unaligned ptr + post-index reg w/o trap fixup]\n");

	val = 0;
	ptrval = 0;
	TRY_TESTCASE(ldr_pi_reg1, WITHOUT_FIXUP);
	val = ldr_unaligned_ptr_postindex_reg(c_data, PTR_1_IMM, &ptrval);
	CATCH_TESTCASE(ldr_pi_reg1);

	/* Make sure we received a SIGBUS signal */
	CHECK_TESTCASE(sigbus_triggered, "SIGBUS not seen\n");

	/***************************************************************
	 * Case #D.2: ldr unaligned ptr + post-index reg w/ trap fixup *
	 ***************************************************************/
	printf("[Case D.2: ldr unaligned ptr + post-index reg w/ trap fixup]\n");

	val = 0;
	ptrval = 0;
	TRY_TESTCASE(ldr_pi_reg2, WITH_FIXUP);
	val = ldr_unaligned_ptr_postindex_reg(c_data, PTR_1_IMM, &ptrval);
	CATCH_TESTCASE(ldr_pi_reg2);

	/* Make sure we did not get a SIGBUS signal */
	if (sigbus_triggered) {
		FAIL_TESTCASE("SIGBUS seen\n");
		goto done_pi_reg2;
	}

	expval = (uint32_t)&c_data[PTR_1_INDEX + PTR_1_IMM];
	if (ptrval != expval) {
		FAIL_TESTCASE("ptrval %08x != %08x c_data[%d + %d]\n",
		    ptrval, expval, PTR_1_INDEX, PTR_1_IMM);
		goto done_pi_reg2;
	}

	expval = htobe32(PTR_1_VAL);
	CHECK_TESTCASE(val == expval,
	    "value %" PRIx32 " not expected %" PRIx32 "\n", val, expval);

done_pi_reg2:

	/****************************************************************
	 * Case #D.3: str unaligned ptr + post-index reg w/o trap fixup *
	 ****************************************************************/
	printf("[Case D.3: str unaligned ptr + post-index reg w/o trap fixup]\n");

	bzero(data, sizeof(data));
	ptrval = 0;
	TRY_TESTCASE(str_pi_reg3, WITHOUT_FIXUP);
	ptrval = str_unaligned_ptr_postindex_reg(data, PTR_1_IMM,
	    htobe32(PTR_1_VAL));
	CATCH_TESTCASE(str_pi_reg3);

	/* Make sure we received a SIGBUS signal */
	CHECK_TESTCASE(sigbus_triggered, "SIGBUS not seen\n");

	/***************************************************************
	 * Case #D.4: str unaligned ptr + post-index reg w/ trap fixup *
	 ***************************************************************/
	printf("[Case D.4: str unaligned ptr + post-index reg w/ trap fixup]\n");

	bzero(data, sizeof(data));
	ptrval = 0;
	TRY_TESTCASE(str_pi_reg4, WITH_FIXUP);
	ptrval = str_unaligned_ptr_postindex_reg(data, PTR_1_IMM,
	    htobe32(PTR_1_VAL));
	CATCH_TESTCASE(str_pi_reg4);

	/* Make sure we did not get a SIGBUS signal */
	if (sigbus_triggered) {
		FAIL_TESTCASE("SIGBUS seen\n");
		goto done_pi_reg4;
	}

	expval = (uint32_t)&data[PTR_1_INDEX + PTR_1_IMM];
	if (ptrval != expval) {
		FAIL_TESTCASE("ptrval %08x != %08x data[%d + %d]\n",
		    ptrval, expval, PTR_1_INDEX, PTR_1_IMM);
		goto done_pi_reg4;
	}

	indx = PTR_1_INDEX;
	CHECK_TESTCASE(!memcmp(&c_data[indx], &data[indx], sizeof(uint32_t)),
	    "value %02x%02x%02x%02x not expected %02x%02x%02x%02x\n",
	    data[indx], data[indx + 1], data[indx + 2], data[indx + 3],
	    c_data[indx], c_data[indx + 1], c_data[indx + 2], c_data[indx + 3]);
done_pi_reg4:

	/********************************************************************
	 * Case #D.5: ldr unaligned ptr + post-index reg LSL w/o trap fixup *
	 ********************************************************************/
	printf("[Case D.5: ldr unaligned ptr + post-index reg LSL w/o trap fixup]\n");

	val = 0;
	ptrval = 0;
	TRY_TESTCASE(ldr_pi_reg5, WITHOUT_FIXUP);
	val = ldr_unaligned_ptr_postindex_reg_lsl(c_data, PTR_11_IMM, &ptrval);
	CATCH_TESTCASE(ldr_pi_reg5);

	/* Make sure we received a SIGBUS signal */
	CHECK_TESTCASE(sigbus_triggered, "SIGBUS not seen\n");

	/*******************************************************************
	 * Case #D.6: ldr unaligned ptr + post-index reg LSL w/ trap fixup *
	 *******************************************************************/
	printf("[Case D.6: ldr unaligned ptr + post-index reg LSL w/ trap fixup]\n");

	val = 0;
	ptrval = 0;
	TRY_TESTCASE(ldr_pi_reg6, WITH_FIXUP);
	val = ldr_unaligned_ptr_postindex_reg_lsl(c_data, PTR_11_IMM, &ptrval);
	CATCH_TESTCASE(ldr_pi_reg6);

	/* Make sure we did not get a SIGBUS signal */
	if (sigbus_triggered) {
		FAIL_TESTCASE("SIGBUS seen\n");
		goto done_pi_reg6;
	}

	expval = (uint32_t)&c_data[PTR_11_INDEX + (PTR_11_IMM << PTR_11_LSL)];
	if (ptrval != expval) {
		FAIL_TESTCASE("ptrval %08x != %08x c_data[%d + %d]\n",
		    ptrval, expval, PTR_11_INDEX, PTR_11_IMM << PTR_11_LSL);
		goto done_pi_reg6;
	}

	expval = htobe32(PTR_11_ALT_VAL);
	CHECK_TESTCASE(val == expval,
	    "value %" PRIx32 " not expected %" PRIx32 "\n", val, expval);

done_pi_reg6:

	/********************************************************************
	 * Case #D.7: str unaligned ptr + post-index reg LSL w/o trap fixup *
	 ********************************************************************/
	printf("[Case D.7: str unaligned ptr + post-index reg LSL w/o trap fixup]\n");

	bzero(data, sizeof(data));
	ptrval = 0;
	TRY_TESTCASE(str_pi_reg7, WITHOUT_FIXUP);
	ptrval = str_unaligned_ptr_postindex_reg_lsl(data, PTR_11_IMM,
	    htobe32(PTR_11_ALT_VAL));
	CATCH_TESTCASE(str_pi_reg7);

	/* Make sure we received a SIGBUS signal */
	CHECK_TESTCASE(sigbus_triggered, "SIGBUS not seen\n");

	/*******************************************************************
	 * Case #D.8: str unaligned ptr + post-index reg LSL w/ trap fixup *
	 *******************************************************************/
	printf("[Case D.8: str unaligned ptr + post-index reg LSL w/ trap fixup]\n");

	bzero(data, sizeof(data));
	ptrval = 0;
	TRY_TESTCASE(str_pi_reg8, WITH_FIXUP);
	ptrval = str_unaligned_ptr_postindex_reg_lsl(data, PTR_11_IMM,
	    htobe32(PTR_11_ALT_VAL));
	CATCH_TESTCASE(str_pi_reg8);

	/* Make sure we did not get a SIGBUS signal */
	if (sigbus_triggered) {
		FAIL_TESTCASE("SIGBUS seen\n");
		goto done_pi_reg8;
	}

	expval = (uint32_t)&data[PTR_11_INDEX + (PTR_11_IMM << PTR_11_LSL)];
	if (ptrval != expval) {
		FAIL_TESTCASE("ptrval %08x != %08x data[%d + %d]\n",
		    ptrval, expval, PTR_11_INDEX, PTR_11_IMM << PTR_11_LSL);
		goto done_pi_reg8;
	}

	indx = PTR_11_INDEX;
	CHECK_TESTCASE(!memcmp(&c_data[indx], &data[indx], sizeof(uint32_t)),
	    "value %02x%02x%02x%02x not expected %02x%02x%02x%02x\n",
	    data[indx], data[indx + 1], data[indx + 2], data[indx + 3],
	    c_data[indx], c_data[indx + 1], c_data[indx + 2], c_data[indx + 3]);
done_pi_reg8:

	/********************************************************************
	 * Case #D.9: ldr unaligned ptr + post-index reg LSR w/o trap fixup *
	 ********************************************************************/
	printf("[Case D.9: ldr unaligned ptr + post-index reg LSR w/o trap fixup]\n");

	val = 0;
	ptrval = 0;
	TRY_TESTCASE(ldr_pi_reg9, WITHOUT_FIXUP);
	val = ldr_unaligned_ptr_postindex_reg_lsr(c_data, PTR_11_IMM, &ptrval);
	CATCH_TESTCASE(ldr_pi_reg9);

	/* Make sure we received a SIGBUS signal */
	CHECK_TESTCASE(sigbus_triggered, "SIGBUS not seen\n");

	/********************************************************************
	 * Case #D.10: ldr unaligned ptr + post-index reg LSR w/ trap fixup *
	 ********************************************************************/
	printf("[Case D.10: ldr unaligned ptr + post-index reg LSR w/ trap fixup]\n");

	val = 0;
	ptrval = 0;
	TRY_TESTCASE(ldr_pi_reg10, WITH_FIXUP);
	val = ldr_unaligned_ptr_postindex_reg_lsr(c_data, PTR_11_IMM, &ptrval);
	CATCH_TESTCASE(ldr_pi_reg10);

	/* Make sure we did not get a SIGBUS signal */
	if (sigbus_triggered) {
		FAIL_TESTCASE("SIGBUS seen\n");
		goto done_pi_reg10;
	}

	expval = (uint32_t)&c_data[PTR_11_INDEX + (PTR_11_IMM >> PTR_11_LSR)];
	if (ptrval != expval) {
		FAIL_TESTCASE("ptrval %08x != %08x c_data[%d + %d]\n",
		    ptrval, expval, PTR_11_INDEX, PTR_11_IMM >> PTR_11_LSR);
		goto done_pi_reg10;
	}

	expval = htobe32(PTR_11_ALT_VAL);
	CHECK_TESTCASE(val == expval,
	    "value %" PRIx32 " not expected %" PRIx32 "\n", val, expval);

done_pi_reg10:

	/*********************************************************************
	 * Case #D.11: str unaligned ptr + post-index reg LSR w/o trap fixup *
	 *********************************************************************/
	printf("[Case D.11: str unaligned ptr + post-index reg LSR w/o trap fixup]\n");

	bzero(data, sizeof(data));
	ptrval = 0;
	TRY_TESTCASE(str_pi_reg11, WITHOUT_FIXUP);
	ptrval = str_unaligned_ptr_postindex_reg_lsr(data, PTR_11_IMM,
	    htobe32(PTR_11_ALT_VAL));
	CATCH_TESTCASE(str_pi_reg11);

	/* Make sure we received a SIGBUS signal */
	CHECK_TESTCASE(sigbus_triggered, "SIGBUS not seen\n");

	/********************************************************************
	 * Case #D.12: str unaligned ptr + post-index reg LSR w/ trap fixup *
	 ********************************************************************/
	printf("[Case D.12: str unaligned ptr + post-index reg LSR w/ trap fixup]\n");

	bzero(data, sizeof(data));
	ptrval = 0;
	TRY_TESTCASE(str_pi_reg12, WITH_FIXUP);
	ptrval = str_unaligned_ptr_postindex_reg_lsr(data, PTR_11_IMM,
	    htobe32(PTR_11_ALT_VAL));
	CATCH_TESTCASE(str_pi_reg12);

	/* Make sure we did not get a SIGBUS signal */
	if (sigbus_triggered) {
		FAIL_TESTCASE("SIGBUS seen\n");
		goto done_pi_reg12;
	}

	expval = (uint32_t)&data[PTR_11_INDEX + (PTR_11_IMM >> PTR_11_LSR)];
	if (ptrval != expval) {
		FAIL_TESTCASE("ptrval %08x != %08x data[%d + %d]\n",
		    ptrval, expval, PTR_11_INDEX, PTR_11_IMM >> PTR_11_LSR);
		goto done_pi_reg12;
	}

	indx = PTR_11_INDEX;
	CHECK_TESTCASE(!memcmp(&c_data[indx], &data[indx], sizeof(uint32_t)),
	    "value %02x%02x%02x%02x not expected %02x%02x%02x%02x\n",
	    data[indx], data[indx + 1], data[indx + 2], data[indx + 3],
	    c_data[indx], c_data[indx + 1], c_data[indx + 2], c_data[indx + 3]);
done_pi_reg12:

	/*********************************************************************
	 * Case #D.13: ldr unaligned ptr + post-index reg ASR w/o trap fixup *
	 *********************************************************************/
	printf("[Case D.13: ldr unaligned ptr + post-index reg ASR w/o trap fixup]\n");

	val = 0;
	ptrval = 0;
	TRY_TESTCASE(ldr_pi_reg13, WITHOUT_FIXUP);
	val = ldr_unaligned_ptr_postindex_reg_asr(c_data, PTR_11_IMM, &ptrval);
	CATCH_TESTCASE(ldr_pi_reg13);

	/* Make sure we received a SIGBUS signal */
	CHECK_TESTCASE(sigbus_triggered, "SIGBUS not seen\n");

	/********************************************************************
	 * Case #D.14: ldr unaligned ptr + post-index reg ASR w/ trap fixup *
	 ********************************************************************/
	printf("[Case D.14: ldr unaligned ptr + post-index reg ASR w/ trap fixup]\n");

	val = 0;
	ptrval = 0;
	TRY_TESTCASE(ldr_pi_reg14, WITH_FIXUP);
	val = ldr_unaligned_ptr_postindex_reg_asr(c_data, PTR_11_IMM, &ptrval);
	CATCH_TESTCASE(ldr_pi_reg14);

	/* Make sure we did not get a SIGBUS signal */
	if (sigbus_triggered) {
		FAIL_TESTCASE("SIGBUS seen\n");
		goto done_pi_reg14;
	}

	expval = (uint32_t)&c_data[PTR_11_INDEX + (PTR_11_IMM >> PTR_11_ASR)];
	if (ptrval != expval) {
		FAIL_TESTCASE("ptrval %08x != %08x c_data[%d + %d]\n",
		    ptrval, expval, PTR_11_INDEX, PTR_11_IMM >> PTR_11_ASR);
		goto done_pi_reg14;
	}

	expval = htobe32(PTR_11_ALT_VAL);
	CHECK_TESTCASE(val == expval,
	    "value %" PRIx32 " not expected %" PRIx32 "\n", val, expval);

done_pi_reg14:

	/*********************************************************************
	 * Case #D.15: str unaligned ptr + post-index reg ASR w/o trap fixup *
	 *********************************************************************/
	printf("[Case D.15: str unaligned ptr + post-index reg ASR w/o trap fixup]\n");

	bzero(data, sizeof(data));
	ptrval = 0;
	TRY_TESTCASE(str_pi_reg15, WITHOUT_FIXUP);
	ptrval = str_unaligned_ptr_postindex_reg_asr(data, PTR_11_IMM,
	    htobe32(PTR_11_ALT_VAL));
	CATCH_TESTCASE(str_pi_reg15);

	/* Make sure we received a SIGBUS signal */
	CHECK_TESTCASE(sigbus_triggered, "SIGBUS not seen\n");

	/********************************************************************
	 * Case #D.16: str unaligned ptr + post-index reg ASR w/ trap fixup *
	 ********************************************************************/
	printf("[Case D.16: str unaligned ptr + post-index reg ASR w/ trap fixup]\n");

	bzero(data, sizeof(data));
	ptrval = 0;
	TRY_TESTCASE(str_pi_reg16, WITH_FIXUP);
	ptrval = str_unaligned_ptr_postindex_reg_asr(data, PTR_11_IMM,
	    htobe32(PTR_11_ALT_VAL));
	CATCH_TESTCASE(str_pi_reg16);

	/* Make sure we did not get a SIGBUS signal */
	if (sigbus_triggered) {
		FAIL_TESTCASE("SIGBUS seen\n");
		goto done_pi_reg16;
	}

	expval = (uint32_t)&data[PTR_11_INDEX + (PTR_11_IMM >> PTR_11_ASR)];
	if (ptrval != expval) {
		FAIL_TESTCASE("ptrval %08x != %08x data[%d + %d]\n",
		    ptrval, expval, PTR_11_INDEX, PTR_11_IMM >> PTR_11_ASR);
		goto done_pi_reg16;
	}

	indx = PTR_11_INDEX;
	CHECK_TESTCASE(!memcmp(&c_data[indx], &data[indx], sizeof(uint32_t)),
	    "value %02x%02x%02x%02x not expected %02x%02x%02x%02x\n",
	    data[indx], data[indx + 1], data[indx + 2], data[indx + 3],
	    c_data[indx], c_data[indx + 1], c_data[indx + 2], c_data[indx + 3]);
done_pi_reg16:

	/*********************************************************************
	 * Case #D.17: ldr unaligned ptr + post-index reg ROR w/o trap fixup *
	 *********************************************************************/
	printf("[Case D.17: ldr unaligned ptr + post-index reg ROR w/o trap fixup]\n");

	val = 0;
	ptrval = 0;
	TRY_TESTCASE(ldr_pi_reg17, WITHOUT_FIXUP);
	val = ldr_unaligned_ptr_postindex_reg_ror(c_data, PTR_11_IMM, &ptrval);
	CATCH_TESTCASE(ldr_pi_reg17);

	/* Make sure we received a SIGBUS signal */
	CHECK_TESTCASE(sigbus_triggered, "SIGBUS not seen\n");

	/********************************************************************
	 * Case #D.18: ldr unaligned ptr + post-index reg ROR w/ trap fixup *
	 ********************************************************************/
	printf("[Case D.18: ldr unaligned ptr + post-index reg ROR w/ trap fixup]\n");

	val = 0;
	ptrval = 0;
	TRY_TESTCASE(ldr_pi_reg18, WITH_FIXUP);
	val = ldr_unaligned_ptr_postindex_reg_ror(c_data, PTR_11_IMM, &ptrval);
	CATCH_TESTCASE(ldr_pi_reg18);

	/* Make sure we did not get a SIGBUS signal */
	if (sigbus_triggered) {
		FAIL_TESTCASE("SIGBUS seen\n");
		goto done_pi_reg18;
	}

	hi = PTR_11_IMM;
	hi <<= (32 - PTR_11_ROR);
	lo = PTR_11_IMM;
	lo >>= PTR_11_ROR;
	indx = (hi | lo);
	expval = (uint32_t)&c_data[PTR_11_INDEX + indx];
	if (ptrval != expval) {
		FAIL_TESTCASE("ptrval %08x != %08x c_data[%d + %d]\n",
		    ptrval, expval, PTR_11_INDEX, indx);
		goto done_pi_reg18;
	}

	expval = htobe32(PTR_11_ALT_VAL);
	CHECK_TESTCASE(val == expval,
	    "value %" PRIx32 " not expected %" PRIx32 "\n", val, expval);

done_pi_reg18:

	/*********************************************************************
	 * Case #D.19: str unaligned ptr + post-index reg ROR w/o trap fixup *
	 *********************************************************************/
	printf("[Case D.19: str unaligned ptr + post-index reg ROR w/o trap fixup]\n");

	bzero(data, sizeof(data));
	ptrval = 0;
	TRY_TESTCASE(str_pi_reg19, WITHOUT_FIXUP);
	ptrval = str_unaligned_ptr_postindex_reg_ror(data, PTR_11_IMM,
	    htobe32(PTR_11_ALT_VAL));
	CATCH_TESTCASE(str_pi_reg19);

	/* Make sure we received a SIGBUS signal */
	CHECK_TESTCASE(sigbus_triggered, "SIGBUS not seen\n");

	/********************************************************************
	 * Case #D.20: str unaligned ptr + post-index reg ROR w/ trap fixup *
	 ********************************************************************/
	printf("[Case D.20: str unaligned ptr + post-index reg ROR w/ trap fixup]\n");

	bzero(data, sizeof(data));
	ptrval = 0;
	TRY_TESTCASE(str_pi_reg20, WITH_FIXUP);
	ptrval = str_unaligned_ptr_postindex_reg_ror(data, PTR_11_IMM,
	    htobe32(PTR_11_ALT_VAL));
	CATCH_TESTCASE(str_pi_reg20);

	/* Make sure we did not get a SIGBUS signal */
	if (sigbus_triggered) {
		FAIL_TESTCASE("SIGBUS seen\n");
		goto done_pi_reg20;
	}

	hi = PTR_11_IMM;
	hi <<= (32 - PTR_11_ROR);
	lo = PTR_11_IMM;
	lo >>= PTR_11_ROR;
	indx = (hi | lo);
	expval = (uint32_t)&data[PTR_11_INDEX + indx];
	if (ptrval != expval) {
		FAIL_TESTCASE("ptrval %08x != %08x data[%d + %d]\n",
		    ptrval, expval, PTR_11_INDEX, indx);
		goto done_pi_reg20;
	}

	indx = PTR_11_INDEX;
	CHECK_TESTCASE(!memcmp(&c_data[indx], &data[indx], sizeof(uint32_t)),
	    "value %02x%02x%02x%02x not expected %02x%02x%02x%02x\n",
	    data[indx], data[indx + 1], data[indx + 2], data[indx + 3],
	    c_data[indx], c_data[indx + 1], c_data[indx + 2], c_data[indx + 3]);
done_pi_reg20:

	printf("END - register offset post-index test\n\n");
}

static void
unaligned_reg_test(void)
{

	unaligned_reg_preindex_test();
	unaligned_reg_postindex_test();
}

static void
usage(int exitcode)
{
	printf("Usage: %s [-a] [-h] [-i] [-r]\n", getprogname());
	printf("\t-a\tRun all tests\n");
	printf("\t-h\tDisplay usage message\n");
	printf("\t-i\tRun immediate offset tests\n");
	printf("\t-r\tRun register offset tests\n");
	exit(exitcode);
}

int
main (int argc, char *argv[])
{
	int aflag, ch, iflag, rflag;

	aflag = iflag = rflag = 0;
	while ((ch = getopt(argc, argv, "ahir")) != -1) {
		switch (ch) {
		case 'a':
			aflag = 1;
			break;
		case 'i':
			iflag = 1;
			break;
		case 'r':
			rflag = 1;
			break;
		case 'h':
			usage(0);
		case '?':
		default:
			usage(EX_USAGE);
		}
	}
	argc -= optind;
	argv += optind;

	if (!(aflag || iflag || rflag))
		usage(EX_USAGE);

	saved_allow = get_allow_unaligned();
	atexit(outta_here);

	if (aflag || iflag)
		unaligned_imm_test();
	if (aflag || rflag)
		unaligned_reg_test();

	return ((tests_failed == 0) ? 0 : EX_DATAERR);
}

