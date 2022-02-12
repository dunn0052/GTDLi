#include "PCH/PCH.h"
#include "experimental/GenericSingleton.h"

namespace GTDLi
{
#if 0
	class WGeneric
	{

	};

	class IGeneric
	{

	};

	template<typename Instance, typename WInstance>
	GTD_API Instance& SingletonInstance<Instance, WInstance>::Instance(void)
	{
#if GTD_PLATFORM_WINDOWS
		static WInstance instance;
#else
		static Instnce instance;
#endif
		return instance;

	}

	//SingletonInstance<IGeneric, WGeneric> instance;
#endif
}
