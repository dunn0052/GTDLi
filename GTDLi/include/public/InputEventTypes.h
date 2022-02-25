#pragma once
#include "Axis.h"
#include "Button.h"
#include "Retcodes.h"

namespace GTDLi
{
	using ButtonPressFunction = RETCODE(*)(const Button&);
	using MultipleButtonsPressedFunction = RETCODE(*)(const std::vector<Button>&);
	using AxisPressFunction = RETCODE(*)(const Axis&);

}