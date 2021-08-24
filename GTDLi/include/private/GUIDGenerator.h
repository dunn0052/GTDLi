#pragma once
#include "public/Retcodes.h"
#include "public/Core.h"

namespace GTDLi
{
	struct GUID64
	{
	public:
		uint64_t high;
		uint64_t low;
	
	public:

		GUID64() : high(0), low(0) {}
		GUID64(uint64_t high_value, uint64_t low_value) : high(high_value), low(low_value) { }

		bool operator == (const GUID64& other) const
		{
			return other.high == high && other.low == low;
		}
	};

	struct GUID64HashFunction
	{
		std::size_t operator() (const GUID64& guid) const
		{
			std::size_t h1 = std::hash<uint64_t>()(guid.high);
			std::size_t h2 = std::hash<uint64_t>()(guid.low);

			return h1 << 1 ^ h2;
		}
	};

	class GUIDGenerator
	{
	public:
		GTD_API static GUIDGenerator& Instance(void);

		GTD_API static RETCODE GenerateGUID(GUID64& out_guid);
	private:

		GUIDGenerator() {}
		~GUIDGenerator() {/* should clean up any statically allocated memory here*/ };
		GUIDGenerator(GUIDGenerator const&) = delete;
		void operator = (GUIDGenerator const&) = delete;

		static RETCODE genrate_random_bytes(uint8_t buffer[], uint32_t buffer_size);
		static RETCODE randomize_buffer(uint32_t buffer[], size_t buffer_size);
		static RETCODE initialize_lcgs();
		static uint32_t get_next_lcg_value(uint32_t lcg, uint32_t lcg_value);
		static RETCODE generate_binary_uuid(GUID64& uuid);

	private:

		static const uint8_t LCG_COUNT = 4;

		static const uint32_t lcg_a[LCG_COUNT];
		static const uint32_t lcg_c[LCG_COUNT];
		static const uint32_t lcg_m[LCG_COUNT];

		static uint32_t lcg_values[LCG_COUNT];

		static bool are_lcgs_initialized;
	};

}

