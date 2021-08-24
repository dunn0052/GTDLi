#pragma once
#include "public/Core.h"
#include "public/References.h"
#include "public/Retcodes.h"
#include "private/Timestep.h"
#include "Button.h"
#include "Axis.h"


namespace GTDLi
{
    struct GTD_API IControllerProps
    {
        unsigned int ID;
        RETCODE retcode;
    };

	class IController
	{
    public:
        GTD_API static Ref<IController> Create(IControllerProps& prop);

        virtual ~IController() {};

        GTD_API virtual RETCODE OnUpdate(Timestep& dt) = 0;

        GTD_API virtual RETCODE GetButtonStatus(Button& button) = 0;

        GTD_API virtual RETCODE GetAxisStatus(Axis& axis) = 0;
	};
}