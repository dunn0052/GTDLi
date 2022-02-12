#pragma once
#include "public/IAudio.h"
#include "private/GUIDGenerator.h"
#include <Windows.h>

namespace GTDLi
{
	class WAudio : public IAudio
	{
	public:

		GTD_API virtual RETCODE OnUpdate(Timestep& dt);

		GTD_API virtual RETCODE LoadSound(SoundProps& props);

		GTD_API virtual RETCODE Play(const GUID64& guid, int loops =  1);

		WAudio() { }
		~WAudio() { }

	private:

		GTD_API virtual RETCODE PlayBuffer(const GUID64& guid, PlayType type);
	};
}