#pragma once
#include "Retcodes.h"
#include "public/Core.h"
#include "private/Timestep.h"
#include "public/References.h"
#include "private/GUIDGenerator.h"
#include <string>

namespace GTDLi
{
	struct AudioProps
	{
		RETCODE retcode;
	};

	struct SoundProps
	{
		std::string path;
		GUID64 guid;
		RETCODE retcode;
	};

	enum class PlayType
	{
		PLAY_ONCE,
		PLAY_LOOP,
		STOP,
	};

	class IAudio
	{
	public:
		GTD_API static IAudio& Instance(void);

		GTD_API virtual ~IAudio() {};

		GTD_API virtual RETCODE OnUpdate(Timestep& dt) = 0;

		GTD_API virtual RETCODE LoadSound(SoundProps& props) = 0;

		GTD_API virtual RETCODE Play(const GUID64& guid, int loops = 1) = 0;

	private:

		GTD_API virtual RETCODE PlayBuffer(const GUID64& guid, PlayType type) = 0;

	};
}