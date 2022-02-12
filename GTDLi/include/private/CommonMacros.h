#pragma once

// Bits
#define MAKE_BIT( BIT_NUM ) ( 1ull << BIT_NUM )
#define SET_BIT( FLAGS, BIT_NUM ) ( FLAGS |= MAKE_BIT( BIT_NUM) )
#define UNSET_BIT( FLAGS, BIT_NUM ) ( FLAGS &= ~MAKE_BIT( BIT_NUM ) )
#define IS_BIT_SET(FLAGS, BIT_NUM) (FLAGS & MAKE_BIT(BIT_NUM) ? 0x1 : 0x0)

// Checks
#define IS_NULL( OBJ ) (nullptr == OBJ)

// Util
#define RECORD_TO_INDEX( RECORD ) ( RECORD - 1 )

#define SAFE_DELETE( POINTER_TO_OBJECT )\
do{\
	if(!IS_NULL(POINTER_TO_OBJECT))\
	{\
		delete POINTER_TO_OBJECT;\
		POINTER_TO_OBJECT = NULL;\
	}\
}while(0)

#define SAFE_FREE( POINTER_TO_ALLOCATED_MEMORY )\
do{\
	if(!IS_NULL(POINTER_TO_ALLOCATED_MEMORY))\
	{\
		free(POINTER_TO_ALLOCATED_MEMORY);\
		POINTER_TO_ALLOCATED_MEMORY = NULL;\
	}\
}while(0)

#define SAFE_FILE_CLOSE( POINTER_TO_OPEN_FILE )\
do{\
	if(!IS_NULL(POINTER_TO_OPEN_FILE))\
	{\
		fclose(POINTER_TO_OPEN_FILE);\
		POINTER_TO_OPEN_FILE = NULL;\
	}\
}while(0)