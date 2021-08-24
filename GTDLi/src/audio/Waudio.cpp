#include "PCH/PCH.h"
#include "private/WAudio.h"
#include "private/GUIDGenerator.h"
#include "private/CommonMacros.h"
#include "private/Logger.h"

namespace GTDLi
{

	GTD_API RETCODE WAudio::OnUpdate(Timestep& dt)
	{
		return RTN_FAIL;
	}

	GTD_API RETCODE WAudio::LoadSound(SoundProps& props)
	{
		RETCODE retcode = RTN_FAIL;
		FILE* sound = NULL;
		long sound_buffer_size = 0;
		size_t sound_buffer_read_size = 0;
		void* sound_buffer = NULL;

		GUID64 guid;
		RETURN_RETCODE_IF_NOT_OK(GUIDGenerator::Instance().GenerateGUID(guid));

		fopen_s(&sound, props.path.c_str(), "r");
		if (NULL == sound)
		{
			LOG_WARN("Couldn't open file: %s ", props.path.c_str());
			retcode = RTN_NULL_OBJ;
			goto CLEANUP_ERROR;
		}

		// obtain file size:
		fseek(sound, 0, SEEK_END);
		sound_buffer_size = ftell(sound);
		rewind(sound);

		// allocate buffer
		sound_buffer = calloc(sound_buffer_size, sizeof(byte));
		if (NULL == sound_buffer)
		{
			LOG_WARN("Could not allocate buffer memory for : %s", props.path.c_str());
			retcode = RTN_MALLOC_FAIL;
			goto CLEANUP_ERROR;
		}

		// copy the file into the buffer:
		sound_buffer_read_size = fread(sound_buffer, 1, sound_buffer_size, *sound);
		if (sound_buffer_read_size != sound_buffer_size)
		{
			LOG_WARN("Error reading file into buffer: %s", props.path.c_str());
			retcode = RTN_FAIL;
			goto CLEANUP_ERROR;
		}

		m_SoundMap.insert({ { guid.high, guid.low }, sound_buffer });
		SAFE_FILE_CLOSE(sound);
		return RTN_OK;

	CLEANUP_ERROR:

		// terminate
		SAFE_FILE_CLOSE(sound);
		SAFE_FREE(sound_buffer);

		return retcode;
	}

	std::unordered_map<GUID64, void*, GUID64HashFunction> WAudio::m_SoundMap;
}