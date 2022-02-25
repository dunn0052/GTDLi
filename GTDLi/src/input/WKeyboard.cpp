#include "PCH.h"
#include "private/WKeyboard.h"

namespace GTDLi
{

	std::unordered_map<const KeyCode&, const Button&> WKeyboard::m_KeyButtonMap;

	std::unordered_map<const Button&, Hook<ButtonPressFunction>, ButtonHashFunction> WKeyboard::m_ButtonHooks;

	WKeyboard::WKeyboard()
	{
		//WMKeyboard constructor
		m_KeyButtonMap.insert
		({
			{Key::A, Button{ButtonCode::A, "A"}},
			{Key::Z, Button{ButtonCode::B, "B"}},
			{Key::S, Button{ButtonCode::X, "X"}},
			{Key::X, Button{ButtonCode::Y, "Y"}},
			{Key::Q, Button{ButtonCode::LB, "LB"}},
			{Key::R, Button{ButtonCode::RB, "RB"}},
			{Key::Up, Button{ButtonCode::UP, "UP"}},
			{Key::Down, Button{ButtonCode::RIGHT, "DOWN"}},
			{Key::Left, Button{ButtonCode::LEFT, "LEFT"}},
			{Key::Right, Button{ButtonCode::RIGHT, "RIGHT"}},
			{Key::Enter, Button{ButtonCode::START, "START"}},
		});

		m_ButtonHooks.insert
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
		return RTN_OK;
	}
	GTD_API RETCODE WKeyboard::GetButtonStatus(Button& button)
	{
		return RTN_OK;
	}

	GTD_API Hook<ButtonPressFunction>& WKeyboard::ButtonPressEvent()
	{
		return m_ButtonPressHook;
	}
	GTD_API Hook<ButtonPressFunction>& WKeyboard::ButtonPressEvent(const Button& button)
	{
		return m_ButtonHooks[button];
	}
	GTD_API RETCODE WKeyboard::MapButtonToKey(const Button& button, const KeyCode& key)
	{
		m_KeyButtonMap.insert_or_assign(key, button);

		return RTN_OK;
	}

	RETCODE WKeyboard::PollKeys()
	{
		RETCODE retcode = RTN_OK;

		for (auto& keyMap : m_KeyButtonMap)
		{
			if (Key::KeyPressed == GetKeyState(keyMap.first))
			{
				retcode |= m_ButtonHooks[keyMap.second].Invoke(keyMap.second);
			}
		}

		return retcode;
	}


}