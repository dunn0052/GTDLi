#pragma once
#include "public/Core.h"

namespace GTDLi
{

	constexpr byte	BUTTON_OFF = 0x0; // !current & ~previous
	constexpr byte	BUTTON_RELEASED = 0x1; // ~current & previous
	constexpr byte	BUTTON_PRESSED = 0x2; // current & ~previous 
	constexpr byte	BUTTON_HELD = 0x3; // current & previous

	template <typename T>
	constexpr auto operator+(T e) noexcept
		-> std::enable_if_t<std::is_enum<T>::value, std::underlying_type_t<T>>
	{
		return static_cast<std::underlying_type_t<T>>(e);
	}

	enum class ButtonCode : DWORD
	{
		NONE = 0x0000,
		X = 0x0001,
		A = 0x0002,
		B = 0x0004,
		Y = 0x0008,
		LB = 0x0010,
		RB = 0x0020,
		LT = 0x0040,
		RT = 0x0080,
		BACK = 0x0100,
		START = 0x0200,
		LSB = 0x0400,
		RSB = 0x0800,
		UP = 0x1000,
		RIGHT = 0x2000,
		DOWN = 0x4000,
		LEFT = 0x8000
	};

	enum class PositionCode : DWORD
	{
		NONE = 0x0,
		X = 0x1,
		Y = 0x2,
		Z = 0x3,
		R = 0x4,
		U = 0x5,
		V = 0x6
	};

	enum class AxisCode : size_t
	{
		LX = 0,
		LY = 1,
		RX = 2,
		RY = 3
	};
}