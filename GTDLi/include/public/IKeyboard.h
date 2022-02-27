#pragma once
#include "public/Core.h"
#include "public/References.h"
#include "public/Retcodes.h"
#include "private/Timestep.h"
#include "Button.h"
#include "KeyCodes.h"
#include "Hook.h"
#include "InputEventTypes.h"

namespace GTDLi
{
	class IKeyboard
	{
	public:
		GTD_API static IKeyboard& Instance();

		virtual ~IKeyboard() {};

		GTD_API virtual RETCODE OnUpdate(Timestep& dt) = 0;

		GTD_API virtual Hook<MultipleButtonsPressedFunction>& ButtonPressEvent() = 0;

		GTD_API virtual Hook<ButtonPressFunction>& ButtonPressEvent(const Button& button) = 0;

		GTD_API virtual RETCODE MapButtonToKey(const Button& button, const KeyCode& key) = 0;
	};
}