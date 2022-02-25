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

		GTD_API virtual RETCODE GetButtonStatus(Button& button);

		GTD_API virtual Hook<ButtonPressFunction>& ButtonPressEvent();

		GTD_API virtual Hook<ButtonPressFunction>& ButtonPressEvent(const Button& button);

		GTD_API virtual RETCODE MapButtonToKey(const Button& button, const KeyCode& key);

	private:

		static RETCODE PollKeys();

	private:
		static Hook<ButtonPressFunction> m_ButtonPressHook;

		static std::unordered_map<const Button&, Hook<ButtonPressFunction>, ButtonHashFunction> m_ButtonHooks;

		static std::unordered_map<const KeyCode&, const Button&> m_KeyButtonMap;

	};
}