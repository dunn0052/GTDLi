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

		GTD_API Button(const ButtonCode& code, const std::string& name)
			: m_Code(code), m_Name(name), m_Status(BUTTON_OFF)
		{

		}

		GTD_API Button(const ButtonProp& prop)
			: m_Code(prop.code), m_Name(prop.name), m_Status(BUTTON_OFF)
		{

		}

		GTD_API Button()
			: m_Code(ButtonCode::NONE), m_Name("NONE"), m_Status(BUTTON_OFF)
		{

		}

		bool operator == (const Button& other) const
		{
			return other.m_Code == m_Code;
		}

		Button& operator = (const Button& other)
		{
			if (&other == this)
			{
				return *this;
			}

			m_Status = other.m_Status;
			m_Code = other.m_Code;
			m_Name = other.m_Name;

			return *this;
		}

	public:
		byte m_Status;
		ButtonCode m_Code;
		std::string m_Name;
	};

	struct ButtonHashFunction
	{
		std::size_t operator() (const Button& button) const
		{
			std::size_t h1 = std::hash<ButtonCode>()(button.m_Code);

			return h1;
		}
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