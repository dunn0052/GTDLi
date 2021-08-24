#include "PCH/PCH.h"
#include "public/Console.h"

namespace GTDLi
{

	Console::Console() : m_Running(false), m_DT(), m_Controllers()
	{
	}

	Console::~Console()
	{
	}

	RETCODE Console::LoadGame()
	{
		return RTN_OK;
	}

	RETCODE Console::LoadControllers(std::vector<byte>& numbers)
	{
		for (byte& id : numbers)
		{
			IControllerProps props = { id, RTN_FAIL };
			Ref<IController> c = IController::Create(props);

			RETURN_RETCODE_IF_NOT_OK(props.retcode);
			
			m_Controllers.push_back(c);
		}

		return RTN_OK;
	}

	RETCODE Console::Run()
	{
		//std::vector<byte> nums(joyGetNumDevs());

		// Controllers start at 1 ??
		//std::iota(std::begin(nums), std::end(nums), 1);
		std::vector<byte> nums = { 0 };
		RETURN_RETCODE_IF_NOT_OK(LoadControllers(nums));

		SoundProps props = { "" };
		RETURN_RETCODE_IF_NOT_OK(IAudio::Instance().LoadSound(props));

		m_Running = true;

		while (m_Running)
		{
			m_DT.UpdateDT();

			RETURN_RETCODE_IF_NOT_OK(Update(m_DT));
		}
		return RTN_OK;
	}

	RETCODE Console::Update(Timestep& dt)
	{
		RETURN_RETCODE_IF_NOT_OK(DoCommands(dt));

		RETURN_RETCODE_IF_NOT_OK(UpdateGame(dt));

		RETURN_RETCODE_IF_NOT_OK(UpdateVideo(dt));

		RETURN_RETCODE_IF_NOT_OK(UpdateAudio(dt));

		return RTN_OK;
	}

	RETCODE Console::DoCommands(Timestep& dt)
	{
		for (Ref<IController> controller : m_Controllers)
		{
			RETURN_RETCODE_IF_NOT_OK(controller->OnUpdate(dt));
		}

		return RTN_OK;
	}

	RETCODE Console::UpdateAudio(Timestep& dt)
	{
		return RTN_OK;
	}

	RETCODE Console::UpdateVideo(Timestep& dt)
	{
		return RTN_OK;
	}

	RETCODE Console::UpdateGame(Timestep& dt)
	{
		ButtonProp prop = { ButtonCode::A, "A" };
		Button btn = Button(prop);

		for (Ref<IController> controller : m_Controllers)
		{
			RETURN_RETCODE_IF_NOT_OK(controller->GetButtonStatus(btn));
			if (BUTTON_PRESSED & btn.m_Status)
			{
			}
		}
		return RTN_OK;
	}
}


