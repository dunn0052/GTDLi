#pragma once
#ifdef GTD_PLATFORM_WINDOWS
#include <windows.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <Xinput.h>
#else
#error JOYSTICK NOT IMPLEMENTED FOR YOUR OS
#endif // GTD_PLATFORM_WINDOWS
#include <string>

#include "public/IController.h"

namespace GTDLi
{

    class WController : public IController
    {

    public:
        GTD_API WController(IControllerProps& props);
        ~WController();

        GTD_API virtual RETCODE OnUpdate(Timestep& dt);

        GTD_API virtual RETCODE GetButtonStatus(Button& button);

        GTD_API virtual RETCODE GetAxisStatus(Axis& axis);

        GTD_API RETCODE PollButtons();

        GTD_API RETCODE PollAxis();

        GTD_API Hook<ButtonPressFunction>& ButtonPressEvent();

        GTD_API Hook<AxisPressFunction>& AxisPressEvent();

    private:
        RETCODE GetState();
    private:
        JOYINFOEX m_ControllerState;
        const UINT m_ID;
        bool m_Connected;
        DWORD m_PreviousButtons;
        DWORD m_CurrentButtons;
        JOYCAPS m_JoysticCapabilities;

        DWORD m_CurrentAxis[NUM_AXIS];
        DWORD m_PreviousAxis[NUM_AXIS];

        Hook<ButtonPressFunction> OnButtonPress;
        Hook<AxisPressFunction> OnAxisPress;
    };
    
}