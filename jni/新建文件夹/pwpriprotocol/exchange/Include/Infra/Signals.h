#ifndef __INIFRA_SIGNALS_H__
#define __INIFRA_SIGNALS_H__

#if defined(WIN32)
	#pragma warning (disable : 4786)
#endif
#include "Function.h"
#include "Mutex.h"

//TSignal0
#define SIGNAL_NUMBER 0
#define SIGNAL_CLASS_TYPES void
#define SIGNAL_TYPES_COMMA
#define SIGNAL_TYPE_ARGS void 
#define SIGNAL_ARGS
#include "Signal.h"
#undef  SIGNAL_NUMBER
#undef  SIGNAL_CLASS_TYPES
#undef	SIGNAL_TYPES_COMMA
#undef	SIGNAL_TYPE_ARGS
#undef	SIGNAL_ARGS

//TSignal1
#define SIGNAL_NUMBER 1
#define SIGNAL_CLASS_TYPES typename T1
#define SIGNAL_TYPES_COMMA , T1
#define SIGNAL_TYPE_ARGS T1 a1 
#define SIGNAL_ARGS a1 
#include "Signal.h"
#undef  SIGNAL_NUMBER
#undef  SIGNAL_CLASS_TYPES
#undef	SIGNAL_TYPES_COMMA
#undef	SIGNAL_TYPE_ARGS
#undef	SIGNAL_ARGS

//TSignal2
#define SIGNAL_NUMBER 2
#define SIGNAL_CLASS_TYPES typename T1, typename T2
#define SIGNAL_TYPES_COMMA , T1, T2
#define SIGNAL_TYPE_ARGS T1 a1, T2 a2
#define SIGNAL_ARGS a1, a2
#include "Signal.h"
#undef  SIGNAL_NUMBER
#undef  SIGNAL_CLASS_TYPES
#undef	SIGNAL_TYPES_COMMA
#undef	SIGNAL_TYPE_ARGS
#undef	SIGNAL_ARGS

//TSignal3
#define SIGNAL_NUMBER 3
#define SIGNAL_CLASS_TYPES typename T1, typename T2, typename T3
#define SIGNAL_TYPES_COMMA , T1, T2, T3
#define SIGNAL_TYPE_ARGS T1 a1, T2 a2, T3 a3
#define SIGNAL_ARGS a1, a2, a3
#include "Signal.h"
#undef  SIGNAL_NUMBER
#undef  SIGNAL_CLASS_TYPES
#undef	SIGNAL_TYPES_COMMA
#undef	SIGNAL_TYPE_ARGS
#undef	SIGNAL_ARGS

//TSignal4
#define SIGNAL_NUMBER 4
#define SIGNAL_CLASS_TYPES typename T1, typename T2, typename T3, typename T4
#define SIGNAL_TYPES_COMMA , T1, T2, T3, T4
#define SIGNAL_TYPE_ARGS T1 a1, T2 a2, T3 a3, T4 a4
#define SIGNAL_ARGS a1, a2, a3, a4
#include "Signal.h"
#undef  SIGNAL_NUMBER
#undef  SIGNAL_CLASS_TYPES
#undef	SIGNAL_TYPES_COMMA
#undef	SIGNAL_TYPE_ARGS
#undef	SIGNAL_ARGS

//TSignal5
#define SIGNAL_NUMBER 5
#define SIGNAL_CLASS_TYPES typename T1, typename T2, typename T3, typename T4, typename T5
#define SIGNAL_TYPES_COMMA , T1, T2, T3, T4, T5
#define SIGNAL_TYPE_ARGS T1 a1, T2 a2, T3 a3, T4 a4, T5 a5 
#define SIGNAL_ARGS a1, a2, a3, a4, a5
#include "Signal.h"
#undef  SIGNAL_NUMBER
#undef  SIGNAL_CLASS_TYPES
#undef	SIGNAL_TYPES_COMMA
#undef	SIGNAL_TYPE_ARGS
#undef	SIGNAL_ARGS

//TSignal6
#define SIGNAL_NUMBER 6
#define SIGNAL_CLASS_TYPES typename T1, typename T2, typename T3, typename T4, typename T5, typename T6
#define SIGNAL_TYPES_COMMA , T1, T2, T3, T4, T5, T6
#define SIGNAL_TYPE_ARGS T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6
#define SIGNAL_ARGS a1, a2, a3, a4, a5, a6
#include "Signal.h"
#undef  SIGNAL_NUMBER
#undef  SIGNAL_CLASS_TYPES
#undef	SIGNAL_TYPES_COMMA
#undef	SIGNAL_TYPE_ARGS
#undef	SIGNAL_ARGS

#endif //__SIGNALS_H__
