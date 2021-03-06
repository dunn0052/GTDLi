#pragma once

namespace GTDLi
{
	// Return code
	typedef __int32 RETCODE;

	// Retcode types

	// Everything was ok
	constexpr RETCODE RTN_OK = 0x0000;

	// General failure
	constexpr RETCODE RTN_FAIL = 0x0001;

	// Encountered a null object
	constexpr RETCODE RTN_NULL_OBJ = 0x0002;

	// Memory allocation error
	constexpr RETCODE RTN_MALLOC_FAIL = 0x0004;

	// Could not find object
	constexpr RETCODE RTN_NOT_FOUND = 0x0008;

	#define IS_RETCODE_OK( RET ) (RTN_OK == RET)

	#define RETURN_RETCODE_IF_NOT_OK( RET ) \
	do{\
		RETCODE retcode = RET;\
		if(!IS_RETCODE_OK(retcode)) \
		{\
			return retcode;\
		}\
	}while(0)

}