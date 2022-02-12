#pragma once

#include "public/Retcodes.h"
#include "public/Core.h"
#include "private/CommonMacros.h"
#include "private/Logger.h"
#include <vector>

namespace GTDLi
{
	#define MEM_POOL_DEFFAULT_SIZE 0x1<<30
	#define MEM_POOL_CHUNK_DEFAULT_CHUNK_SIZE 1024

	/* Memory Chunk - Keeps track of a slice in a memory pool */
	struct MemChunk
	{
		const byte* m_Data_Buffer;
		bool m_In_Use;
		size_t m_Buffer_Size;
		size_t m_Used_Size;
		MemChunk* m_Next_Chunk;
		MemChunk* m_Previous_chunk;

		MemChunk(const byte* buffer, size_t& buffer_size, MemChunk* next, MemChunk* previous)
			: m_Data_Buffer(buffer), m_In_Use(false), m_Buffer_Size(buffer_size), m_Used_Size(0), m_Next_Chunk(next), m_Previous_chunk(previous)
		{
		}

		RETCODE Initialize(const byte* buffer, size_t& buffer_size, MemChunk* next, MemChunk* previous)
		{
			RETCODE retcode = RTN_OK;

			if (IS_NULL(buffer) || 0 == buffer_size)
			{
				retcode |= RTN_NULL_OBJ;
			}

			m_Data_Buffer = buffer;
			m_In_Use = false;
			m_Buffer_Size = buffer_size;
			m_Used_Size = 0;
			m_Next_Chunk = next;
			m_Previous_chunk = previous;

			return retcode;
		}

		~MemChunk()
		{
			// Data should be freed on MemPool desctructor
			//SAFE_DELETE(m_Data_Buffer);
		}
	};

	class MemPool
	{
	public:

		struct MemPoolParams
		{
			size_t poolSize;
		};

		GTD_API static MemPool& Instance(void);

		GTD_API ~MemPool();

		template<typename T, typename ... Args>
		GTD_API constexpr T& New(Args&& ... args);

		template<typename T, typename ... Args>
		GTD_API constexpr T* New(const size_t& num_items, Args&& ... args);

		GTD_API void* Alloc(const size_t& size, RETCODE& out_retcode);

		GTD_API RETCODE Alloc(const size_t& size, void** out_memory);

		GTD_API RETCODE Free(void* memory);

	private:

		// Private constructor as we are making this a global instance
		MemPool(const size_t& pool_size, const size_t& chunk_size);
		RETCODE AllocatePoolMemory(const size_t& total_size);
		RETCODE CreateMemChunks(void);
		RETCODE CalculateBestPoolSize(const size_t& requested_size);

		// Ideal linked list
		MemChunk* m_First_Chunk;
		MemChunk* m_Cursor_Chunk;
		MemChunk* m_Last_Chunk;
		MemChunk* m_All_chunks;
		size_t m_Number_of_Chunks;
		size_t m_Chunk_size;

		const byte* m_Pool;
		size_t m_Total_size;
		size_t m_Total_used;
		// m_Total_size - sum of chunks.
		// Keep track of memory free, not number of free chunks
		size_t m_Total_free;
		
	};
}

