
#ifdef GTD_PLATFORM_WINDOWS
	#pragma comment(lib, "winmm.lib")
#endif

#include "PCH.h"
#include "public/Console.h"
#include "private/WAssetLoader.h"
#include "private/MemPool.h"

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

	static RETCODE PressButton(const Button& button)
	{
		LOG_DEBUG("Pressed button %s is in state %d", button.m_Name.c_str(), button.m_Status);
		return RTN_OK;
	}

	static RETCODE PressAxis(const Axis& axis)
	{
		LOG_DEBUG("Axis %s is at %d with status %d", axis.m_Name.c_str(), axis.m_Position, axis.m_Status);
		return RTN_OK;
	}

	static RETCODE SingleButtonPress(const Button& button)
	{
		LOG_DEBUG("Pressed button %s is in state %d", button.m_Name.c_str(), button.m_Status);
		return RTN_OK;
	}

	static RETCODE KeyBoardButtonPress(const std::vector<Button>& buttons)
	{
		for (const Button& button : buttons)
		{
			LOG_DEBUG("Pressed button %s is in state %d", button.m_Name.c_str(), button.m_Status);
		}

		return RTN_OK;
	}

	RETCODE Console::Run()
	{
#if 0
		std::vector<byte> nums(joyGetNumDevs());

		// Controllers start at 1 ??
		//std::iota(std::begin(nums), std::end(nums), 1);
		//std::vector<byte> nums = { 0 };
		RETURN_RETCODE_IF_NOT_OK(LoadControllers(nums));

		m_Controllers[0]->ButtonPressEvent() += &PressButton;
		m_Controllers[0]->AxisPressEvent() += &PressAxis;
		SoundProps props = { "D:\\GTDLi\\bin\\Debug\\x64\\Sandbox\\assets\\LTTP_Rupee1.wav" };

		RETURN_RETCODE_IF_NOT_OK(IAudio::Instance().LoadSound(props));

		RETURN_RETCODE_IF_NOT_OK(IAudio::Instance().Play(props.guid));

		//RETURN_RETCODE_IF_NOT_OK(IAssetLoader::Instance().RemoveAsset(props.guid));

		AssetProps asset_prop = { "D:\\GTDLi\\bin\\Debug\\x64\\Sandbox\\assets\\SalarUpdate.pdf" };

		RETURN_RETCODE_IF_NOT_OK(IAssetLoader::Instance().LoadAsset(asset_prop));

		RETCODE ret = RTN_OK;

		char* test = (char*)MemPool::Instance().Alloc(90000 * sizeof(char), ret);
		IKeyboard::Instance().ButtonPressEvent(GTDLi::allButtons[1]) += &SingleButtonPress;
#endif
		IKeyboard::Instance().ButtonPressEvent() += &KeyBoardButtonPress;

		m_Running = true;

		while (m_Running)
		{
			m_DT.UpdateDT();

			RETURN_RETCODE_IF_NOT_OK(Update(m_DT));

			Sleep(10);
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

		IKeyboard::Instance().OnUpdate(dt);

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
		return RTN_OK;
	}
}


