#ifndef _FICL4_COMPAT_H_
#define _FICL4_COMPAT_H_

#define FICL_DICT	ficlDictionary
#define FICL_SYSTEM	ficlSystem
#define FICL_VM		ficlVm
#define FICL_WORD	ficlWord

#define FW_DEFAULT	FICL_WORD_DEFAULT

#define VM_ABORT	FICL_VM_STATUS_ABORT
#define VM_ABORTQ	FICL_VM_STATUS_ABORTQ
#define VM_ERREXIT	FICL_VM_STATUS_ERROR_EXIT
#define VM_OUTOFTEXT	FICL_VM_STATUS_OUT_OF_TEXT
#define	VM_QUIT		FICL_VM_STATUS_QUIT
#define VM_USEREXIT	FICL_VM_STATUS_USER_EXIT

#define pStack		dataStack

#define dictAppendWord(_dp, _n, _pc, _f) \
	ficlDictionarySetPrimitive((_dp), (_n), (_pc), (_f))
#define ficlExec(_vm, _s)	ficlVmEvaluate((_vm), _s)
#define ficlSetEnv(_s, _w, _v)	ficlDictionarySetConstant(ficlSystemGetEnvironment((_s)), (_w), (_v))
#define ficlNewVM(_s)		ficlSystemCreateVm((_s))
#define ficlLookup(_s, _w)	ficlSystemLookup((_s), (_w))

#define stackFetch(_s, _i)	ficlStackFetch((_s), (_i))
#define stackPopINT(_s)		ficlStackPopInteger((_s))
#define stackPopPtr(_s)		ficlStackPopPointer((_s))
#define stackPopPTR(_s)		stackPopPtr(_s)
#define stackPushINT(_s, _v)	ficlStackPushInteger((_s), (_v))
#define stackPushPtr(_s, _v)	ficlStackPushPointer((_s), (_v))
#define stackPushPTR(_s, _v)	stackPushPtr(_s), (_v))

#define vmCheckStack(_vm, _pop, _push) FICL_STACK_CHECK((_vm)->dataStack, (_pop), (_push))
#define vmGetInBuf(_vm)		ficlVmGetInBuf((_vm))
#define vmThrow(_vm, _r)	ficlVmThrow((_vm), (_r))
#define vmUpdateTib(_vm, _tib)	ficlVmUpdateTib((_vm), (_tib))

#endif
