#pragma once
#include "public/Core.h"
#include "private/CommonMacros.h"
#include "ContextCodes.h"

#include <string>

namespace GTDLi
{

	struct ButtonProp
	{
		const ButtonCode code;
		std::string name;
	};

	struct Button
	{
		GTD_API Button(const ButtonProp& prop)
			: m_Code(prop.code), m_Name(prop.name), m_Status(BUTTON_OFF)
		{

		}

		GTD_API Button()
			: m_Code(ButtonCode::NONE), m_Name("NONE"), m_Status(BUTTON_OFF)
		{

		}

	public:
		byte m_Status;
		const ButtonCode m_Code;
		const std::string m_Name;
	};

	// For debugging
	constexpr byte NUM_BUTTONS = 0x10;

	static Button allButtons[NUM_BUTTONS] =
	{
		Button(ButtonProp{ ButtonCode::X, "X" }),
		Button(ButtonProp{ ButtonCode::A, "A" }),
		Button(ButtonProp{ ButtonCode::B, "B" }),
		Button(ButtonProp{ ButtonCode::Y, "Y" }),
		Button(ButtonProp{ ButtonCode::LB, "LB" }),
		Button(ButtonProp{ ButtonCode::RB, "RB" }),
		Button(ButtonProp{ ButtonCode::LT, "LT" }),
		Button(ButtonProp{ ButtonCode::RT, "RT" }),
		Button(ButtonProp{ ButtonCode::BACK, "BACK" }),
		Button(ButtonProp{ ButtonCode::START, "START" }),
		Button(ButtonProp{ ButtonCode::LSB, "LSB" }),
		Button(ButtonProp{ ButtonCode::RSB, "RSB" }),
		Button(ButtonProp{ ButtonCode::UP, "UP" }),
		Button(ButtonProp{ ButtonCode::RIGHT, "RIGHT" }),
		Button(ButtonProp{ ButtonCode::DOWN, "DOWN" }),
		Button(ButtonProp{ ButtonCode::LEFT, "LEFT" })
	};
}