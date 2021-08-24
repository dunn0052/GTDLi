#pragma once

#include "public/Core.h"
#include "private/Logger.h"
#include "public/Retcodes.h"
#include "private/Timestep.h"
#include "public/IController.h"
#include "public/IAudio.h"
#include <vector>

namespace GTDLi
{
	class Console
	{

	/* Hardware representation */

	public:
		GTD_API Console();

		GTD_API ~Console();

		/* Load user created game */
		GTD_API RETCODE LoadGame();

		/* Set up controllers */
		GTD_API RETCODE LoadControllers(std::vector<byte>& numbers);

		/* Run the game */
		GTD_API RETCODE Run();

	private:

		/* Updates all needed components */
		RETCODE Update(Timestep& dt);

		RETCODE DoCommands(Timestep& dt);
		RETCODE UpdateAudio(Timestep& dt);
		RETCODE UpdateVideo(Timestep& dt);
		RETCODE UpdateGame(Timestep& dt);

	private:
		bool m_Running;
		Timestep m_DT;

		std::vector<Ref<IController>> m_Controllers;
	};
	// To be defined in client
	Console* CreateConsole();
}