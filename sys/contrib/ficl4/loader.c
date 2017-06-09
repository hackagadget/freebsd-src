/*-
 * Copyright (c) 2015 Stephen J. Kiernan
 * All rights reserved.
 *
 * Copyright (c) 2000 Daniel Capo Sobral
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
 *
 *	$FreeBSD$
 */

/*******************************************************************
** l o a d e r . c
** Additional FICL words designed for FreeBSD's loader
** 
*******************************************************************/

#ifdef TESTMAIN
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#else
#include <stand.h>
#endif
#include "bootstrap.h"
#include <string.h>
#include "ficl.h"

/*		FreeBSD's loader interaction words and extras
 *
 * 		setenv      ( value n name n' -- )
 * 		setenv?     ( value n name n' flag -- )
 * 		getenv      ( addr n -- addr' n' | -1 )
 * 		unsetenv    ( addr n -- )
 * 		copyin      ( addr addr' len -- )
 * 		copyout     ( addr addr' len -- )
 * 		findfile    ( name len type len' -- addr )
 * 		pnpdevices  ( -- addr )
 * 		pnphandlers ( -- addr )
 * 		ccall       ( [[...[p10] p9] ... p1] n addr -- result )
 * 		.#	    ( value -- )
 */

void
ficlSetenv(ficlVm *vm)
{
#ifndef TESTMAIN
	char	*name, *value;
#endif
	char	*namep, *valuep;
	int	names, values;

#if FICL_ROBUST > 1
	FICL_STACK_CHECK(vm->dataStack, 4, 0);
#endif
	names = ficlStackPopInteger(vm->dataStack);
	namep = (char*) ficlStackPopPointer(vm->dataStack);
	values = ficlStackPopInteger(vm->dataStack);
	valuep = (char*) ficlStackPopPointer(vm->dataStack);

#ifndef TESTMAIN
	name = (char*) ficlMalloc(names+1);
	if (!name)
		ficlVmThrowError(vm, "Error: out of memory");
	strncpy(name, namep, names);
	name[names] = '\0';
	value = (char*) ficlMalloc(values+1);
	if (!value)
		ficlVmThrowError(vm, "Error: out of memory");
	strncpy(value, valuep, values);
	value[values] = '\0';

	setenv(name, value, 1);
	ficlFree(name);
	ficlFree(value);
#endif

	return;
}

void
ficlSetenvq(ficlVm *vm)
{
#ifndef TESTMAIN
	char	*name, *value;
#endif
	char	*namep, *valuep;
	int	names, values, overwrite;

#if FICL_ROBUST > 1
	FICL_STACK_CHECK(vm->dataStack, 5, 0);
#endif
	overwrite = ficlStackPopInteger(vm->dataStack);
	names = ficlStackPopInteger(vm->dataStack);
	namep = (char*) ficlStackPopPointer(vm->dataStack);
	values = ficlStackPopInteger(vm->dataStack);
	valuep = (char*) ficlStackPopPointer(vm->dataStack);

#ifndef TESTMAIN
	name = (char*) ficlMalloc(names+1);
	if (!name)
		ficlVmThrowError(vm, "Error: out of memory");
	strncpy(name, namep, names);
	name[names] = '\0';
	value = (char*) ficlMalloc(values+1);
	if (!value)
		ficlVmThrowError(vm, "Error: out of memory");
	strncpy(value, valuep, values);
	value[values] = '\0';

	setenv(name, value, overwrite);
	ficlFree(name);
	ficlFree(value);
#endif

	return;
}

void
ficlGetenv(ficlVm *vm)
{
#ifndef TESTMAIN
	char	*name, *value;
#endif
	char	*namep;
	int	names;

#if FICL_ROBUST > 1
	FICL_STACK_CHECK(vm->dataStack, 2, 2);
#endif
	names = ficlStackPopInteger(vm->dataStack);
	namep = (char*) ficlStackPopPointer(vm->dataStack);

#ifndef TESTMAIN
	name = (char*) ficlMalloc(names+1);
	if (!name)
		ficlVmThrowError(vm, "Error: out of memory");
	strncpy(name, namep, names);
	name[names] = '\0';

	value = getenv(name);
	ficlFree(name);

	if(value != NULL) {
		ficlStackPushPointer(vm->dataStack, value);
		ficlStackPushInteger(vm->dataStack, strlen(value));
	} else
#endif
		ficlStackPushInteger(vm->dataStack, -1);

	return;
}

void
ficlUnsetenv(ficlVm *vm)
{
#ifndef TESTMAIN
	char	*name;
#endif
	char	*namep;
	int	names;

#if FICL_ROBUST > 1
	FICL_STACK_CHECK(vm->dataStack, 2, 0);
#endif
	names = ficlStackPopInteger(vm->dataStack);
	namep = (char*) ficlStackPopPointer(vm->dataStack);

#ifndef TESTMAIN
	name = (char*) ficlMalloc(names+1);
	if (!name)
		ficlVmThrowError(vm, "Error: out of memory");
	strncpy(name, namep, names);
	name[names] = '\0';

	unsetenv(name);
	ficlFree(name);
#endif

	return;
}

void
ficlCopyin(ficlVm *vm)
{
	void*		src;
	vm_offset_t	dest;
	size_t		len;

#if FICL_ROBUST > 1
	FICL_STACK_CHECK(vm->dataStack, 3, 0);
#endif

	len = ficlStackPopInteger(vm->dataStack);
	dest = ficlStackPopInteger(vm->dataStack);
	src = ficlStackPopPointer(vm->dataStack);

#ifndef TESTMAIN
	archsw.arch_copyin(src, dest, len);
#endif

	return;
}

void
ficlCopyout(ficlVm *vm)
{
	void*		dest;
	vm_offset_t	src;
	size_t		len;

#if FICL_ROBUST > 1
	FICL_STACK_CHECK(vm->dataStack, 3, 0);
#endif

	len = ficlStackPopInteger(vm->dataStack);
	dest = ficlStackPopPointer(vm->dataStack);
	src = ficlStackPopInteger(vm->dataStack);

#ifndef TESTMAIN
	archsw.arch_copyout(src, dest, len);
#endif

	return;
}

void
ficlFindfile(ficlVm *vm)
{
#ifndef TESTMAIN
	char	*name, *type;
#endif
	char	*namep, *typep;
	struct	preloaded_file* fp;
	int	names, types;

#if FICL_ROBUST > 1
	FICL_STACK_CHECK(vm->dataStack, 4, 1);
#endif

	types = ficlStackPopInteger(vm->dataStack);
	typep = (char*) ficlStackPopPointer(vm->dataStack);
	names = ficlStackPopInteger(vm->dataStack);
	namep = (char*) ficlStackPopPointer(vm->dataStack);
#ifndef TESTMAIN
	name = (char*) ficlMalloc(names+1);
	if (!name)
		ficlVmThrowError(vm, "Error: out of memory");
	strncpy(name, namep, names);
	name[names] = '\0';
	type = (char*) ficlMalloc(types+1);
	if (!type)
		ficlVmThrowError(vm, "Error: out of memory");
	strncpy(type, typep, types);
	type[types] = '\0';

	fp = file_findfile(name, type);
#else
	fp = NULL;
#endif
	ficlStackPushPointer(vm->dataStack, fp);

	return;
}

#ifndef TESTMAIN
#ifdef HAVE_PNP

void
ficlPnpdevices(ficlVm *vm)
{
	static int pnp_devices_initted = 0;
#if FICL_ROBUST > 1
	FICL_STACK_CHECK(vm->dataStack, 0, 1);
#endif

	if(!pnp_devices_initted) {
		STAILQ_INIT(&pnp_devices);
		pnp_devices_initted = 1;
	}

	ficlStackPushPointer(vm->dataStack, &pnp_devices);

	return;
}

void
ficlPnphandlers(ficlVm *vm)
{
#if FICL_ROBUST > 1
	FICL_STACK_CHECK(vm->dataStack, 0, 1);
#endif

	ficlStackPushPointer(vm->dataStack, pnphandlers);

	return;
}

#endif

#endif /* ndef TESTMAIN */

void
ficlCcall(ficlVm *vm)
{
	int (*func)(int, ...);
	int result, p[10];
	int nparam, i;

#if FICL_ROBUST > 1
	FICL_STACK_CHECK(vm->dataStack, 2, 0);
#endif

	func = ficlStackPopPointer(vm->dataStack);
	nparam = ficlStackPopInteger(vm->dataStack);

#if FICL_ROBUST > 1
	FICL_STACK_CHECK(vm->dataStack, nparam, 1);
#endif

	for (i = 0; i < nparam; i++)
		p[i] = ficlStackPopInteger(vm->dataStack);

	result = func(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8],
	    p[9]);

	ficlStackPushInteger(vm->dataStack, result);

	return;
}

/**************************************************************************
                        f i c l E x e c F D
** reads in text from file fd and passes it to ficlExec()
 * returns FICL_VM_STATUS_OUT_OF_TEXT on success or the ficlExec() error code on
 * failure.
 */ 
#define nLINEBUF 256
int ficlExecFD(ficlVm *vm, int fd)
{
    char    cp[nLINEBUF];
    int     nLine = 0, rval = FICL_VM_STATUS_OUT_OF_TEXT;
    char    ch;
    ficlString  s;
    ficlCell    id;

    id = vm->sourceId;
    vm->sourceId.i = fd;

    /* feed each line to ficlExec */
    while (1) {
	int status, i;

	i = 0;
	while ((status = read(fd, &ch, 1)) > 0 && ch != '\n')
	    cp[i++] = ch;
        nLine++;
	if (!i) {
	    if (status < 1)
		break;
	    continue;
	}
	FICL_STRING_SET_POINTER(s, cp);
	FICL_STRING_SET_LENGTH(s, i);
        rval = ficlVmExecuteString(vm, s);
	if(rval != FICL_VM_STATUS_QUIT && rval != FICL_VM_STATUS_USER_EXIT &&
	   rval != FICL_VM_STATUS_OUT_OF_TEXT)
        {
            vm->sourceId = id;
            return rval; 
        }
    }
    /*
    ** Pass an empty line with SOURCE-ID == -1 to flush
    ** any pending REFILLs (as required by FILE wordset)
    */
    ficlVmEvaluate(vm, "");

    vm->sourceId = id;
    return rval;
}

static void displayCellNoPad(ficlVm *vm)
{
    ficlCell c;
#if FICL_ROBUST > 1
    FICL_STACK_CHECK(vm->dataStack, 1, 0);
#endif
    c = ficlStackPop(vm->dataStack);
    ficlLtoa((c).i, vm->pad, vm->base);
    ficlVmTextOut(vm, vm->pad);
    return;
}

/*      isdir? - Return whether an fd corresponds to a directory.
 *
 * isdir? ( fd -- bool )
 */
static void isdirQuestion(ficlVm *vm)
{
    struct stat sb;
    ficlInteger flag;
    int fd;

#if FICL_ROBUST > 1
    FICL_STACK_CHECK(vm->dataStack, 1, 1);
#endif

    fd = ficlStackPopInteger(vm->dataStack);
    flag = FICL_FALSE;
    do {
        if (fd < 0)
            break;
        if (fstat(fd, &sb) < 0)
            break;
        if (!S_ISDIR(sb.st_mode))
            break;
        flag = FICL_TRUE;
    } while (0);
    ficlStackPushInteger(vm->dataStack, flag);
}

/*          fopen - open a file and return new fd on stack.
 *
 * fopen ( ptr count mode -- fd )
 */
static void pfopen(ficlVm *vm)
{
    int     mode, fd, count;
    char    *ptr, *name;

#if FICL_ROBUST > 1
    FICL_STACK_CHECK(vm->dataStack, 3, 1);
#endif

    mode = ficlStackPopInteger(vm->dataStack);    /* get mode */
    count = ficlStackPopInteger(vm->dataStack);   /* get count */
    ptr = ficlStackPopPointer(vm->dataStack);     /* get ptr */

    if ((count < 0) || (ptr == NULL)) {
        ficlStackPushInteger(vm->dataStack, -1);
        return;
    }

    /* ensure that the string is null terminated */
    name = (char *)malloc(count+1);
    bcopy(ptr,name,count);
    name[count] = 0;

    /* open the file */
    fd = open(name, mode);
    free(name);
    ficlStackPushInteger(vm->dataStack, fd);
    return;
}
 
/*          fclose - close a file who's fd is on stack.
 *
 * fclose ( fd -- )
 */
static void pfclose(ficlVm *vm)
{
    int fd;

#if FICL_ROBUST > 1
    FICL_STACK_CHECK(vm->dataStack, 1, 0);
#endif
    fd = ficlStackPopInteger(vm->dataStack); /* get fd */
    if (fd != -1)
	close(fd);
    return;
}

/*          fread - read file contents
 *
 * fread  ( fd buf nbytes  -- nread )
 */
static void pfread(ficlVm *vm)
{
    int     fd, len;
    char *buf;

#if FICL_ROBUST > 1
    FICL_STACK_CHECK(vm->dataStack, 3, 1);
#endif
    len = ficlStackPopInteger(vm->dataStack); /* get number of bytes to read */
    buf = ficlStackPopPointer(vm->dataStack); /* get buffer */
    fd = ficlStackPopInteger(vm->dataStack); /* get fd */
    if (len > 0 && buf && fd != -1)
	ficlStackPushInteger(vm->dataStack, read(fd, buf, len));
    else
	ficlStackPushInteger(vm->dataStack, -1);
    return;
}

/*      freaddir - read directory contents
 *
 * freaddir ( fd -- ptr len TRUE | FALSE )
 */
static void pfreaddir(ficlVm *vm)
{
#ifdef TESTMAIN
    static struct dirent dirent;
    struct stat sb;
    char *buf;
    off_t off, ptr;
    u_int blksz;
    int bufsz;
#endif
    struct dirent *d;
    int fd;

#if FICL_ROBUST > 1
    FICL_STACK_CHECK(vm->dataStack, 1, 3);
#endif

    fd = ficlStackPopInteger(vm->dataStack);
#if TESTMAIN
    /*
     * The readdirfd() function is specific to the loader environment.
     * We do the best we can to make freaddir work, but it's not at
     * all guaranteed.
     */
    d = NULL;
    buf = NULL;
    do {
	if (fd == -1)
	    break;
	if (fstat(fd, &sb) == -1)
	    break;
	blksz = (sb.st_blksize) ? sb.st_blksize : getpagesize();
	if ((blksz & (blksz - 1)) != 0)
	    break;
	buf = malloc(blksz);
	if (buf == NULL)
	    break;
	off = lseek(fd, 0LL, SEEK_CUR);
	if (off == -1)
	    break;
	ptr = off;
	if (lseek(fd, 0, SEEK_SET) == -1)
	    break;
	bufsz = getdents(fd, buf, blksz);
	while (bufsz > 0 && bufsz <= ptr) {
	    ptr -= bufsz;
	    bufsz = getdents(fd, buf, blksz);
	}
	if (bufsz <= 0)
	    break;
	d = (void *)(buf + ptr);
	dirent = *d;
	off += d->d_reclen;
	d = (lseek(fd, off, SEEK_SET) != off) ? NULL : &dirent;
    } while (0);
    if (buf != NULL)
	free(buf);
#else
    d = readdirfd(fd);
#endif
    if (d != NULL) {
        ficlStackPushPointer(vm->dataStack, d->d_name);
        ficlStackPushInteger(vm->dataStack, strlen(d->d_name));
        ficlStackPushInteger(vm->dataStack, FICL_TRUE);
    } else {
        ficlStackPushInteger(vm->dataStack, FICL_FALSE);
    }
}

/*          fload - interpret file contents
 *
 * fload  ( fd -- )
 */
static void pfload(ficlVm *vm)
{
    int     fd;

#if FICL_ROBUST > 1
    FICL_STACK_CHECK(vm->dataStack, 1, 0);
#endif
    fd = ficlStackPopInteger(vm->dataStack); /* get fd */
    if (fd != -1)
	ficlExecFD(vm, fd);
    return;
}

/*          fwrite - write file contents
 *
 * fwrite  ( fd buf nbytes  -- nwritten )
 */
static void pfwrite(ficlVm *vm)
{
    int     fd, len;
    char *buf;

#if FICL_ROBUST > 1
    FICL_STACK_CHECK(vm->dataStack, 3, 1);
#endif
    len = ficlStackPopInteger(vm->dataStack); /* get number of bytes to read */
    buf = ficlStackPopPointer(vm->dataStack); /* get buffer */
    fd = ficlStackPopInteger(vm->dataStack); /* get fd */
    if (len > 0 && buf && fd != -1)
	ficlStackPushInteger(vm->dataStack, write(fd, buf, len));
    else
	ficlStackPushInteger(vm->dataStack, -1);
    return;
}

/*          fseek - seek to a new position in a file
 *
 * fseek  ( fd ofs whence  -- pos )
 */
static void pfseek(ficlVm *vm)
{
    int     fd, pos, whence;

#if FICL_ROBUST > 1
    FICL_STACK_CHECK(vm->dataStack, 3, 1);
#endif
    whence = ficlStackPopInteger(vm->dataStack);
    pos = ficlStackPopInteger(vm->dataStack);
    fd = ficlStackPopInteger(vm->dataStack);
    ficlStackPushInteger(vm->dataStack, lseek(fd, pos, whence));
    return;
}

/*           key - get a character from stdin
 *
 * key ( -- char )
 */
static void key(ficlVm *vm)
{
#if FICL_ROBUST > 1
    FICL_STACK_CHECK(vm->dataStack, 0, 1);
#endif
    ficlStackPushInteger(vm->dataStack, getchar());
    return;
}

/*           key? - check for a character from stdin (FACILITY)
 *
 * key? ( -- flag )
 */
static void keyQuestion(ficlVm *vm)
{
#if FICL_ROBUST > 1
    FICL_STACK_CHECK(vm->dataStack, 0, 1);
#endif
#ifdef TESTMAIN
    /* XXX Since we don't fiddle with termios, let it always succeed... */
    ficlStackPushInteger(vm->dataStack, FICL_TRUE);
#else
    /* But here do the right thing. */
    ficlStackPushInteger(vm->dataStack, ischar()? FICL_TRUE : FICL_FALSE);
#endif
    return;
}

/* seconds - gives number of seconds since beginning of time
 *
 * beginning of time is defined as:
 *
 *	BTX	- number of seconds since midnight
 *	FreeBSD	- number of seconds since Jan 1 1970
 *
 * seconds ( -- u )
 */
static void pseconds(ficlVm *vm)
{
#if FICL_ROBUST > 1
    FICL_STACK_CHECK(vm->dataStack,0,1);
#endif
    ficlStackPushUnsigned(vm->dataStack, (ficlUnsigned) time(NULL));
    return;
}

/* ms - wait at least that many milliseconds (FACILITY)
 *
 * ms ( u -- )
 *
 */
static void ms(ficlVm *vm)
{
#if FICL_ROBUST > 1
    FICL_STACK_CHECK(vm->dataStack,1,0);
#endif
#ifdef TESTMAIN
    usleep(ficlStackPopUnsigned(vm->dataStack)*1000);
#else
    delay(ficlStackPopUnsigned(vm->dataStack)*1000);
#endif
    return;
}

/*           fkey - get a character from a file
 *
 * fkey ( file -- char )
 */
static void fkey(ficlVm *vm)
{
    int i, fd;
    char ch;

#if FICL_ROBUST > 1
    FICL_STACK_CHECK(vm->dataStack, 1, 1);
#endif
    fd = ficlStackPopInteger(vm->dataStack);
    i = read(fd, &ch, 1);
    ficlStackPushInteger(vm->dataStack, i > 0 ? ch : -1);
    return;
}


#ifdef __i386__
/*
 * pcibios-device-count (devid -- count)
 *
 * Returns the PCI BIOS' count of how many devices matching devid are in the system.
 * devid is the 32-bit vendor + device.
 */
static void
ficlPciBiosCountDevices(ficlVm *vm)
{
	uint32_t devid;
	int i;

	devid = ficlStackPopInteger(vm->dataStack);

	i = biospci_count_device_type(devid);

	ficlStackPushInteger(vm->dataStack, i);
}

/*
 * pcibios-write-config (locator offset width value -- )
 *
 * Writes the specified config register.
 * Locator is bus << 8 | device << 3 | fuction
 * offset is the pci config register
 * width is 0 for byte, 1 for word, 2 for dword
 * value is the value to write
 */
static void
ficlPciBiosWriteConfig(ficlVm *vm)
{
	uint32_t value, width, offset, locator;

	value = ficlStackPopInteger(vm->dataStack);
	width = ficlStackPopInteger(vm->dataStack);
	offset = ficlStackPopInteger(vm->dataStack);
	locator = ficlStackPopInteger(vm->dataStack);

	biospci_write_config(locator, offset, width, value);
}

/*
 * pcibios-read-config (locator offset width -- value)
 *
 * Reads the specified config register.
 * Locator is bus << 8 | device << 3 | fuction
 * offset is the pci config register
 * width is 0 for byte, 1 for word, 2 for dword
 * value is the value to read from the register
 */
static void
ficlPciBiosReadConfig(ficlVm *vm)
{
	uint32_t value, width, offset, locator;

	width = ficlStackPopInteger(vm->dataStack);
	offset = ficlStackPopInteger(vm->dataStack);
	locator = ficlStackPopInteger(vm->dataStack);

	biospci_read_config(locator, offset, width, &value);

	ficlStackPushInteger(vm->dataStack, value);
}

/*
 * pcibios-find-devclass (class index -- locator)
 *
 * Finds the index'th instance of class in the pci tree.
 * must be an exact match.
 * class is the class to search for.
 * index 0..N (set to 0, increment until error)
 *
 * Locator is bus << 8 | device << 3 | fuction (or -1 on error)
 */
static void
ficlPciBiosFindDevclass(ficlVm *vm)
{
	uint32_t index, class, locator;

	index = ficlStackPopInteger(vm->dataStack);
	class = ficlStackPopInteger(vm->dataStack);

	if (biospci_find_devclass(class, index, &locator))
		locator = 0xffffffff;

	ficlStackPushInteger(vm->dataStack, locator);
}

/*
 * pcibios-find-device(devid index -- locator)
 *
 * Finds the index'th instance of devid in the pci tree.
 * must be an exact match.
 * class is the class to search for.
 * index 0..N (set to 0, increment until error)
 *
 * Locator is bus << 8 | device << 3 | fuction (or -1 on error)
 */
static void
ficlPciBiosFindDevice(ficlVm *vm)
{
	uint32_t index, devid, locator;

	index = ficlStackPopInteger(vm->dataStack);
	devid = ficlStackPopInteger(vm->dataStack);

	if (biospci_find_device(devid, index, &locator))
		locator = 0xffffffff;

	ficlStackPushInteger(vm->dataStack, locator);
}

/*
 * pcibios-find-device(bus device function -- locator)
 *
 * converts bus, device, function to locator.
 *
 * Locator is bus << 8 | device << 3 | fuction
 */
static void
ficlPciBiosLocator(ficlVm *vm)
{
	uint32_t bus, device, function, locator;

	function = ficlStackPopInteger(vm->dataStack);
	device = ficlStackPopInteger(vm->dataStack);
	bus = ficlStackPopInteger(vm->dataStack);

	locator = biospci_locator(bus, device, function);

	ficlStackPushInteger(vm->dataStack, locator);
}
#endif

/*
** Retrieves free space remaining on the dictionary
*/

static void freeHeap(ficlVm *vm)
{
    ficlDictionary *dictionary = ficlVmGetDictionary(vm);

    ficlStackPushInteger(vm->dataStack,
	ficlDictionaryCellsAvailable(dictionary));
}

#ifndef TESTMAIN
#ifdef __i386__
/* 
 * outb ( port# c -- )
 * Store a byte to I/O port number port#
 */
static void
ficlOutb(ficlVm *vm)
{
	u_char c;
	u_int32_t port;

	port = ficlStackPopUnsigned(vm->dataStack);
	c = (u_char)ficlStackPopInteger(vm->dataStack);
	outb(port,c);
}

/*
 * inb ( port# -- c )
 * Fetch a byte from I/O port number port#
 */
static void
ficlInb(ficlVm *vm)
{
	u_char c;
	u_int32_t port;

	port = ficlStackPopUnsigned(vm->dataStack);
	c = inb(port);
	ficlStackPushInteger(vm->dataStack, c);
}
#endif
#endif


/**************************************************************************
                        f i c l C o m p i l e P l a t f o r m
** Build FreeBSD platform extensions into the system dictionary
**************************************************************************/
void ficlCompilePlatform(ficlSystem *system)
{
    ficlDictionary *dictionary = ficlSystemGetDictionary(system);
    ficlDictionary *environment = ficlSystemGetEnvironment(system);

    FICL_SYSTEM_ASSERT(system, dictionary);
    FICL_SYSTEM_ASSERT(system, environment);

    ficlDictionarySetPrimitive(dictionary, ".#",        displayCellNoPad,    FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "isdir?",    isdirQuestion,  FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "fopen",	    pfopen,	    FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "fclose",    pfclose,	    FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "fread",	    pfread,	    FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "freaddir",  pfreaddir,	    FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "fload",	    pfload,	    FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "fkey",	    fkey,	    FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "fseek",     pfseek,	    FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "fwrite",    pfwrite,	    FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "key",	    key,	    FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "key?",	    keyQuestion,    FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "ms",        ms,             FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "seconds",   pseconds,       FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "heap?",     freeHeap,       FICL_WORD_DEFAULT);

    ficlDictionarySetPrimitive(dictionary, "setenv",    ficlSetenv,	    FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "setenv?",   ficlSetenvq,    FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "getenv",    ficlGetenv,	    FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "unsetenv",  ficlUnsetenv,   FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "copyin",    ficlCopyin,	    FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "copyout",   ficlCopyout,    FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "findfile",  ficlFindfile,   FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "ccall",	    ficlCcall,	    FICL_WORD_DEFAULT);
#ifndef TESTMAIN
#ifdef __i386__
    ficlDictionarySetPrimitive(dictionary, "outb",      ficlOutb,       FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "inb",       ficlInb,        FICL_WORD_DEFAULT);
#endif
#ifdef HAVE_PNP
    ficlDictionarySetPrimitive(dictionary, "pnpdevices",ficlPnpdevices, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "pnphandlers",ficlPnphandlers, FICL_WORD_DEFAULT);
#endif
#endif
#ifdef __i386__
    ficlDictionarySetPrimitive(dictionary, "pcibios-device-count", ficlPciBiosCountDevices, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "pcibios-read-config", ficlPciBiosReadConfig, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "pcibios-write-config", ficlPciBiosWriteConfig, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "pcibios-find-devclass", ficlPciBiosFindDevclass, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "pcibios-find-device", ficlPciBiosFindDevice, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "pcibios-locator", ficlPciBiosLocator, FICL_WORD_DEFAULT);
#endif

#if defined(PC98)
    ficlDictionarySetConstant(environment, "arch-pc98",         FICL_TRUE);
#elif defined(__i386__)
    ficlDictionarySetConstant(environment, "arch-i386",         FICL_TRUE);
    ficlDictionarySetConstant(environment, "arch-powerpc",      FICL_FALSE);
#elif defined(__powerpc__)
    ficlDictionarySetConstant(environment, "arch-i386",         FICL_FALSE);
    ficlDictionarySetConstant(environment, "arch-powerpc",      FICL_TRUE);
#endif

    FICL_SYSTEM_ASSERT(system, ficlDictionaryCellsAvailable(dictionary) > 0);
    return;
}
