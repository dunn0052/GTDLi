#include "PCH/PCH.h"
#include "public/IController.h"

#if GTD_PLATFORM_WINDOWS
	#include "private/WController.h"
#endif

namespace GTDLi
{
	Ref<IController> GTDLi::IController::Create(IControllerProps& prop)
	{
	#if GTD_PLATFORM_WINDOWS
		return CreateRef<WController>(prop);
	#endif
	}
}
