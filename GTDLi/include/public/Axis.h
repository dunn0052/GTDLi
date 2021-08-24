#pragma once
#include <string>
#include "public/Core.h"
#include "public/ContextCodes.h"

namespace GTDLi
{
	struct AxisProp
	{
		const PositionCode code;
		std::string name;
	};

	// This is what seems to be calibrated when I debug
	constexpr DWORD NEUTRAL_POSITION = 32511;

	class Axis
	{
	public:
		Axis(const AxisProp& prop)
			: m_Name(prop.name), m_Code(prop.code), m_Position(NEUTRAL_POSITION), m_Status(BUTTON_OFF)
		{

		}

		Axis()
			: m_Name("NULL"), m_Code(PositionCode::NONE), m_Position(NEUTRAL_POSITION), m_Status(BUTTON_OFF)
		{

		}

	public:
		DWORD m_Position;
		byte m_Status;
		const PositionCode m_Code;
		const std::string m_Name;
	};

	constexpr byte NUM_AXIS = 0x6;

	static Axis allAxis[NUM_AXIS]
	{
		Axis(AxisProp{PositionCode::X, "X_AXIS"}),
		Axis(AxisProp{PositionCode::Y, "Y_AXIS"}),
		Axis(AxisProp{PositionCode::Z, "Z_AXIS"}),
		Axis(AxisProp{PositionCode::R, "R_AXIS"}),
		Axis(AxisProp{PositionCode::U, "U_AXIS"}),
		Axis(AxisProp{PositionCode::V, "V_AXIS"}),
	};
}