#include "PCH/PCH.h"
#include "private/WAudio.h"
#include "private/GUIDGenerator.h"
#include "private/CommonMacros.h"
#include "private/Logger.h"
#include "public/IAssetLoader.h"

namespace GTDLi
{

	GTD_API RETCODE WAudio::OnUpdate(Timestep& dt)
	{
		return RTN_FAIL;
	}

	GTD_API RETCODE WAudio::LoadSound(SoundProps& props)
	{
		props.retcode |= IAssetLoader::Instance().LoadAsset(props.path, props.guid);

		return props.retcode;
	}

	GTD_API RETCODE WAudio::Play(const GUID64& guid, int loops)
	{
		RETCODE retcode = RTN_OK;

		switch (loops)
		{
			case -1:
			{
				retcode |= PlayBuffer(guid, PlayType::PLAY_LOOP);
				break;
			}
			case 0:
			{
				retcode |= PlayBuffer(guid, PlayType::STOP);
				break;
			}
			default:
			{
				// This doesn't work..
				for (int i = 0; i < loops; i++)
				{
					RETURN_RETCODE_IF_NOT_OK(PlayBuffer(guid, PlayType::PLAY_ONCE));
				}
			}
		}


		return retcode;
	}

	GTD_API RETCODE WAudio::PlayBuffer(const GUID64& guid, PlayType type)
	{
		RETCODE retcode = RTN_OK;
		UINT play_flags = 0x0;
		const Asset* sound_buffer = nullptr;

		switch(type)
		{
		case PlayType::STOP:
		{
			sndPlaySound(nullptr, play_flags);
			break;
		}
		case PlayType::PLAY_LOOP:
		{
			play_flags |= SND_LOOP;
		}
		[[fallthrough]]; //Only need to set loop flag
		case PlayType::PLAY_ONCE:
		{
			retcode |= IAssetLoader::Instance().GetAsset(guid, &sound_buffer);
			if (IS_RETCODE_OK(retcode) && !IS_NULL(sound_buffer))
			{
				LOG_DEBUG("Playing sound: %s", sound_buffer->path.c_str());
				sndPlaySound((LPCWSTR)sound_buffer->buffer, play_flags | SND_MEMORY | SND_ASYNC);
			}
			break;
		}
		default:
		{
			LOG_WARN("Invalid play type: %u for GUID: %u-%u", type, guid.high, guid.low);
		}
		}

		return retcode;
	
	}
}