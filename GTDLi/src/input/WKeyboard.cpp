#include "PCH.h"
#include "private/WKeyboard.h"
#include <execution>


namespace GTDLi
{

	Hook<MultipleButtonsPressedFunction> WKeyboard::m_OnAnyButtonsPressed;

	std::unordered_map<Button, Hook<ButtonPressFunction>, ButtonHashFunction> WKeyboard::m_OnButtonPressed;

	std::unordered_map<KeyCode, Button> WKeyboard::m_KeyToButtonMap;

	DWORD WKeyboard::m_CurrentButtons;
	
	DWORD WKeyboard::m_PreviousButtons;

	WKeyboard::WKeyboard()
	{
		//WMKeyboard constructor
		m_KeyToButtonMap.insert
		({
			{Key::A, Button{ButtonCode::A, "A"}},
			{Key::Z, Button{ButtonCode::B, "B"}},
			{Key::S, Button{ButtonCode::X, "X"}},
			{Key::X, Button{ButtonCode::Y, "Y"}},
			{Key::Q, Button{ButtonCode::LB, "LB"}},
			{Key::E, Button{ButtonCode::RB, "RB"}},
			{Key::Up, Button{ButtonCode::UP, "UP"}},
			{Key::Down, Button{ButtonCode::RIGHT, "DOWN"}},
			{Key::Left, Button{ButtonCode::LEFT, "LEFT"}},
			{Key::Right, Button{ButtonCode::RIGHT, "RIGHT"}},
			{Key::Enter, Button{ButtonCode::START, "START"}},
			});

		m_OnButtonPressed.insert
		({
			{Button{ButtonCode::A, "A"}, Hook<ButtonPressFunction>()},
			{Button{ButtonCode::B, "B"}, Hook<ButtonPressFunction>()},
			{Button{ButtonCode::X, "X"}, Hook<ButtonPressFunction>()},
			{Button{ButtonCode::Y, "Y"}, Hook<ButtonPressFunction>()},
			{Button{ButtonCode::LB, "LB"}, Hook<ButtonPressFunction>()},
			{Button{ButtonCode::RB, "RB"}, Hook<ButtonPressFunction>()},
			{Button{ButtonCode::UP, "UP"}, Hook<ButtonPressFunction>()},
			{Button{ButtonCode::DOWN, "DOWN"}, Hook<ButtonPressFunction>()},
			{Button{ButtonCode::LEFT, "LEFT"}, Hook<ButtonPressFunction>()},
			{Button{ButtonCode::RIGHT, "RIGHT"}, Hook<ButtonPressFunction>()}
		});
	}

	WKeyboard::~WKeyboard()
	{
	}

	GTD_API RETCODE WKeyboard::OnUpdate(Timestep& dt)
	{
		return PollKeys();
	}

	GTD_API Hook<MultipleButtonsPressedFunction>& WKeyboard::ButtonPressEvent()
	{
		return m_OnAnyButtonsPressed;
	}
	GTD_API Hook<ButtonPressFunction>& WKeyboard::ButtonPressEvent(const Button& button)
	{
		return m_OnButtonPressed[button];
	}

	GTD_API RETCODE WKeyboard::MapButtonToKey(const Button& button, const KeyCode& key)
	{
		m_KeyToButtonMap[key] = button;

		return RTN_OK;
	}

	RETCODE WKeyboard::PollKeys()
	{
		RETCODE retcode = RTN_OK;
		std::vector<Button> buttons_pressed = std::vector<Button>();
		
		m_PreviousButtons = m_CurrentButtons;
		m_CurrentButtons = 0;

		for (auto& keyMap : m_KeyToButtonMap)
		{
			// copy current bit to previous bit
			keyMap.second.m_Status = keyMap.second.m_Status >> 1;

			if (KEY_PRESSED_CODE & GetKeyState(keyMap.first))
			{
				SET_BIT(keyMap.second.m_Status, 1);
			}

			if (keyMap.second.m_Status)
			{
				buttons_pressed.push_back(keyMap.second);

				retcode |= m_OnButtonPressed[keyMap.second].Invoke(keyMap.second);
			}

		}

		if (!buttons_pressed.empty())
		{
			retcode |= m_OnAnyButtonsPressed.Invoke(buttons_pressed);
		}
		

		return retcode;
	}

}