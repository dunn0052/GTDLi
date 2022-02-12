#include "PCH/PCH.h"
#include "public/IAssetLoader.h"

#if GTD_PLATFORM_WINDOWS
#include "private/WAssetLoader.h"
#endif

namespace GTDLi
{
	GTD_API IAssetLoader& IAssetLoader::Instance(void)
	{
#if GTD_PLATFORM_WINDOWS
		static WAssetLoader instance;
#else
		static IAssetLoader instance;
#endif
		return instance;

	}
}
