#pragma once

namespace GTDLi
{
	typedef __int32 RETCODE;

	// Retcode types
	constexpr RETCODE RTN_OK = 0x0000;
	constexpr RETCODE RTN_FAIL = 0x0001;
	constexpr RETCODE RTN_NULL_OBJ = 0x0002;
	constexpr RETCODE RTN_MALLOC_FAIL = 0x0004;

	#define RETURN_RETCODE_IF_NOT_OK( VAL ) \
	do{\
		RETCODE retcode = VAL;\
		if(RTN_OK != retcode) \
		{\
			return retcode;\
		}\
	}while(0)

}