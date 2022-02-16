#include "PCH/PCH.h"
#include "private/GUIDGenerator.h"
#include "private/MemPool.h"
#include <cstdint>
#include <random>
#include <time.h>

#pragma comment (lib, "bcrypt.lib")

namespace GTDLi
{

	const uint32_t GUIDGenerator::lcg_a[GUIDGenerator::LCG_COUNT] = { 2147483645U, 60492498U, 1431655762U, 613566752U };
	const uint32_t GUIDGenerator::lcg_c[GUIDGenerator::LCG_COUNT] = { 629860161U, 3184768421U, 3320314307U, 2767970130U };
	const uint32_t GUIDGenerator::lcg_m[GUIDGenerator::LCG_COUNT] = { 4294967288U, 4294967287U, 4294967283U, 4294967257U };

	bool GUIDGenerator::are_lcgs_initialized = false;
	uint32_t GUIDGenerator::lcg_values[GUIDGenerator::LCG_COUNT] = { 0 };

	GTD_API GUIDGenerator& GTDLi::GUIDGenerator::Instance(void)
	{
		static GUIDGenerator instance;
		return instance;
	}

	GTD_API RETCODE GTDLi::GUIDGenerator::GenerateGUID(GUID64& out_guid)
	{
		if (!are_lcgs_initialized)
		{
			if (RTN_OK != initialize_lcgs())
			{
				are_lcgs_initialized = false;
				return RTN_FAIL;
			}
			else
			{
				are_lcgs_initialized = true;
			}
		}

		return generate_binary_uuid(out_guid);
	}

	RETCODE GTDLi::GUIDGenerator::genrate_random_bytes(uint8_t buffer[], uint32_t buffer_size)
	{
		RETCODE retcode = RTN_OK;
		uint32_t byte_index = 0;
		
#ifdef GTD_PLATFORM_WINDOWS
		NTSTATUS status;
		ULONG cbBuffer = 0;
		BCRYPT_ALG_HANDLE pCrypAlgHandle = nullptr;
		LPCWSTR algname = BCRYPT_RNG_ALGORITHM;

		status = BCryptOpenAlgorithmProvider(&pCrypAlgHandle, algname, nullptr, 0);

		if (status <= 0)
		{
			status = BCryptGenRandom(pCrypAlgHandle, buffer, buffer_size, 0);
			if (status > 0)
			{
				retcode = RTN_FAIL;
			}
		}

#else
		int rand_file = open(UNIX_RAND_PATH, O_RDONLY);
		
		if (0 < rand_file)
		{
			read(rand_file, buffer, buffer_size);
		}
#endif
		else
		{
			LOG_WARN("OS RNG could not be initialized! Using srand() like an idiot");
			srand((int)time(0));
			for (; byte_index < buffer_size; byte_index++)
			{
				buffer[byte_index] = (uint8_t)rand();
			}

		}

		return retcode;
	}

	RETCODE GTDLi::GUIDGenerator::randomize_buffer(uint32_t buffer[], size_t buffer_size)
	{
		unsigned char* p_rand_buffer = (unsigned char*)buffer;
		RETCODE retcode = RTN_FAIL;

		if (NULL == buffer || buffer_size < 1)
		{
			return RTN_NULL_OBJ;
		}

		while (buffer_size > 0)
		{
			int randbufferSize = (buffer_size > UINT32_MAX) ? UINT32_MAX : (int)buffer_size;
			retcode = genrate_random_bytes(p_rand_buffer, randbufferSize);
			if (RTN_OK != retcode)
			{
				memset(buffer, 0, buffer_size);
				return retcode;
			}

			p_rand_buffer += randbufferSize;
			buffer_size -= (size_t)randbufferSize;
		}

		return RTN_OK;
	}

	RETCODE GTDLi::GUIDGenerator::initialize_lcgs()
	{
		int lcg_index = 0;
		RETCODE retcode = randomize_buffer(lcg_values, sizeof(lcg_values));

		if (RTN_OK != retcode)
		{
			are_lcgs_initialized = false;
			return retcode;
		}

		for (; (lcg_index < LCG_COUNT); lcg_index++)
		{
			lcg_values[lcg_index] %= lcg_m[lcg_index];
		}
		return RTN_OK;
	}

	uint32_t GTDLi::GUIDGenerator::get_next_lcg_value(uint32_t lcg, uint32_t lcg_value)
	{
		uint64_t a = 0;
		uint64_t c = 0;
		uint64_t m = 0;
		uint64_t x = (uint64_t)lcg_value;

		if (lcg < LCG_COUNT)
		{
			a = (uint64_t)lcg_a[lcg];
			c = (uint64_t)lcg_c[lcg];
			m = (uint64_t)lcg_m[lcg];
		}

		return (uint32_t)(((a * x) + c) % m);
	}

	RETCODE GTDLi::GUIDGenerator::generate_binary_uuid(GUID64& uuid)
	{
		uint32_t lcg = 0;
		uint32_t v1_high = 0;
		uint32_t v1_low = 0;
		uint32_t v2_high = 0;
		uint32_t v2_low = 0;

		for (; (lcg < LCG_COUNT); lcg++)
		{
			lcg_values[lcg] = get_next_lcg_value(lcg, lcg_values[lcg]);
		}

		v1_high = lcg_values[1] >> 16;
		v1_low = lcg_values[1] & 0x0000ffff;
		v2_high = lcg_values[2] >> 16;
		v2_low = lcg_values[2] & 0x0000ffff;

		v1_high = (v1_low & 0x00000fff) | 0x00004000;
		v2_high = (v2_high & 0x00003fff) | 0x000080000;

		uuid.high = ((uint64_t)(lcg_values[0]) << 32 | ((uint64_t)v1_high << 16)) | (uint64_t)v1_low;
		uuid.low = ((uint64_t)v2_high << 48) | ((uint64_t)v2_low << 32) | (uint64_t)(lcg_values[3]);

		return RTN_OK;
	}
}
