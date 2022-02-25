#include "PCH.h"
#include "public/IKeyboard.h"
#include "private/WKeyboard.h"

namespace GTDLi
{
	GTD_API IKeyboard& IKeyboard::Instance()
	{
#if GTD_PLATFORM_WINDOWS
		static WKeyboard instance;
#else
		static IKeyboard instance;
#endif
		return instance;
	}
}