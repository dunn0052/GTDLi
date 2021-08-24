#include "PCH/PCH.h"

#include "private/WController.h"
#include "private/Logger.h"
#include "private/CommonMacros.h"

namespace GTDLi
{

	WController::WController(IControllerProps& props) : 
		m_ID(props.ID), m_Connected(false), m_PreviousButtons(0), m_CurrentButtons(0), m_PreviousAxis{ 0 }, m_CurrentAxis{ 0 }, m_JoysticCapabilities{ 0 },
	
		m_ControllerState
		{ 
			sizeof(m_ControllerState), 
			JOY_RETURNBUTTONS | JOY_RETURNCENTERED | JOY_RETURNX | JOY_RETURNY 
		}

	{

		MMRESULT result = joyGetDevCaps(m_ID, &m_JoysticCapabilities, sizeof(m_JoysticCapabilities));

		switch (result)
		{
			case JOYERR_NOERROR:
			{
				m_ControllerState.dwFlags |= m_JoysticCapabilities.wCaps & JOYCAPS_HASZ ? JOY_RETURNZ : 0x0;
				m_ControllerState.dwFlags |= m_JoysticCapabilities.wCaps & JOYCAPS_HASR ? JOY_RETURNR : 0x0;
				m_ControllerState.dwFlags |= m_JoysticCapabilities.wCaps & JOYCAPS_HASU ? JOY_RETURNU : 0x0;
				m_ControllerState.dwFlags |= m_JoysticCapabilities.wCaps & JOYCAPS_HASV ? JOY_RETURNV : 0x0;

				m_Connected = true;
				props.retcode = RTN_OK;
				LOG_INFO("Controller %d has connected", m_ID);
				return;
			}
			case MMSYSERR_NODRIVER:
			{
				LOG_WARN("No driver found for controller %d", m_ID);
				break;
			}
			case MMSYSERR_INVALPARAM:
			case JOYERR_PARMS:
			{
				LOG_WARN("Invalid parameter in ControllerProps for controller %d", m_ID);
				break;
			}
			case JOYERR_NOCANDO:
			{
				LOG_WARN("Connection to contoller %d not completed.", m_ID);
				break;
			}
			default:
			{
				LOG_WARN("Unknown error on controller %d when getting controller capabilities: %d", m_ID, result);
			}
		}

		props.retcode = RTN_FAIL;
	}

	WController::~WController()
	{
	}

	RETCODE WController::OnUpdate(Timestep& dt)
	{
		RETURN_RETCODE_IF_NOT_OK(GetState());

#if 0
		RETURN_RETCODE_IF_NOT_OK(PrintButtons());
		RETURN_RETCODE_IF_NOT_OK(PrintAxis());
#endif
		return RTN_OK;
	}

	RETCODE WController::GetState()
	{

		if (!m_Connected || IS_NULL(&m_ControllerState))
		{
			LOG_WARN("Controller %d is not connected!", m_ID);
			return RTN_FAIL;
		}

		MMRESULT result = joyGetPosEx(m_ID, &m_ControllerState);

		switch (result)
		{
			case JOYERR_NOERROR:
			{
				m_PreviousButtons = m_CurrentButtons;
				m_CurrentButtons = m_ControllerState.dwButtons;

				// Copy everything even if not used.
				// This avoids the situation where the axis we want are not contiguous
				memcpy(m_PreviousAxis, m_CurrentAxis, sizeof(m_PreviousAxis));

				// Assuming that dwXpos -> dwVPos are in order
				memcpy(m_CurrentAxis, &m_ControllerState.dwXpos, sizeof(m_CurrentAxis));

				return RTN_OK;
			}
			case JOYERR_UNPLUGGED:
			{
				LOG_WARN("Controller %d has been disconnected!", m_ID);
				break;
			}
			default:
			{
				LOG_WARN("Unknown error on controller %d state polling: %d", m_ID, result);
				break;
			}
		}

		m_Connected = false;
		return RTN_FAIL;
	}

	RETCODE WController::GetButtonStatus(Button& button)
	{
		/*
		Set button status
		OFF = 0x0, // ~current & ~previous
		RELEASED = 0x1, // ~current & previous
		PRESSED = 0x2, // current & ~previous
		HELD = 0x3 // current & previous
		*/

		button.m_Status |= static_cast<byte>( ( m_CurrentButtons & +button.m_Code ) > 0 ) << 0x1 | static_cast<byte>( ( m_PreviousButtons & +button.m_Code ) > 0 );
		return RTN_OK;
	}

	RETCODE WController::GetAxisStatus(Axis& axis)
	{
		/*
		Set axis status
		OFF = 0x0, // ~current & ~previous
		RELEASED = 0x1, // ~current & previous
		PRESSED = 0x2, // current & ~previous
		HELD = 0x3 // current & previous
		*/

		axis.m_Status |= 0x1 << static_cast<byte>(m_CurrentAxis[RECORD_TO_INDEX(+axis.m_Code)] != NEUTRAL_POSITION) | 
								static_cast<byte>(m_PreviousAxis[RECORD_TO_INDEX(+axis.m_Code)] != NEUTRAL_POSITION);
		
		axis.m_Position = m_CurrentAxis[RECORD_TO_INDEX(+axis.m_Code)];

		return RTN_OK;
	}

	RETCODE WController::PrintButtons()
	{
		if (!m_Connected)
		{
			return RTN_FAIL;
		}

		for (Button& button : allButtons)
		{
			if(m_CurrentButtons & +button.m_Code || m_PreviousButtons & +button.m_Code)
			{
				RETURN_RETCODE_IF_NOT_OK(GetButtonStatus(button));
				LOG_DEBUG("Pressed button %s is in state %d", button.m_Name.c_str(), button.m_Status);
			}
		}

		return RTN_OK;
	}
	GTD_API RETCODE WController::PrintAxis()
	{
		byte axisIndex = 0;
		Axis* axis = allAxis;

		if (!m_Connected)
		{
			return RTN_FAIL;
		}

		// Loop through number of axis since a non polled axis returns 0 as well as an axis in min position
		for (; axisIndex < m_JoysticCapabilities.wNumAxes && axisIndex < NUM_AXIS; axisIndex++)
		{
			if (NEUTRAL_POSITION != m_CurrentAxis[axisIndex] || NEUTRAL_POSITION != m_PreviousAxis[axisIndex])
			{
				RETURN_RETCODE_IF_NOT_OK(GetAxisStatus(*axis));
				LOG_DEBUG("Axis %s is at %d with status %d", (*axis).m_Name.c_str(), (*axis).m_Position, (*axis).m_Status);
			}
			axis++;
		}

		return RTN_OK;
	}
}