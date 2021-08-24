#include "PCH/PCH.h"
#include "public/IAudio.h"

#if GTD_PLATFORM_WINDOWS
	#include "private/WAudio.h"
#endif

namespace GTDLi
{
	GTD_API IAudio& IAudio::Instance(void)
	{
#if GTD_PLATFORM_WINDOWS
		static WAudio instance;
#else
		static IAudio instance;
#endif
		return instance;

	}
}
