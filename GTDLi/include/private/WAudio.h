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

		WAudio() { }
		~WAudio() { }
	private:

		static std::unordered_map<GUID64, void*, GUID64HashFunction> m_SoundMap;
	};
}