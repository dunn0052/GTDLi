#pragma once
#include "public/IKeyboard.h"

namespace GTDLi
{
	class WKeyboard : public IKeyboard
	{
	public:
		GTD_API WKeyboard();

		~WKeyboard();

		GTD_API virtual RETCODE OnUpdate(Timestep& dt);

		GTD_API virtual Hook<MultipleButtonsPressedFunction>& ButtonPressEvent();

		GTD_API virtual Hook<ButtonPressFunction>& ButtonPressEvent(const Button& button);

		GTD_API virtual RETCODE MapButtonToKey(const Button& button, const KeyCode& key);

	private:

		static RETCODE PollKeys();

	private:
		
		static DWORD m_PreviousButtons;

		static DWORD m_CurrentButtons;

		static Hook<MultipleButtonsPressedFunction> m_OnAnyButtonsPressed;

		static std::unordered_map<Button, Hook<ButtonPressFunction>, ButtonHashFunction> m_OnButtonPressed;

		static std::unordered_map<KeyCode, Button> m_KeyToButtonMap;

	};
}