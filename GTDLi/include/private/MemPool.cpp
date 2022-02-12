#include "PCH/PCH.h"
#include "MemPool.h"

namespace GTDLi
{


    GTD_API MemPool& MemPool::Instance(void)
    {
        static MemPool instance(MEM_POOL_DEFFAULT_SIZE, MEM_POOL_CHUNK_DEFAULT_CHUNK_SIZE);

        return instance;
    }

    MemPool::MemPool(const size_t& requested_size, const size_t& chunk_size)
        : m_Chunk_size(chunk_size)
    {
            AllocatePoolMemory(requested_size);
            CreateMemChunks();
    }

    GTD_API MemPool::~MemPool()
    {
    }

    template<typename T, typename ... Args>
    GTD_API constexpr T& MemPool::New(Args&& ... args)
    {
        T* p_mem = MemPool::Alloc(sizeof(T));
        p_mem[0] = T(std::forward<Args>(args)...);
        return p_mem[0];
    }

    template<typename T, typename ... Args>
    GTD_API constexpr T* MemPool::New(const size_t& num_items, Args&& ... args)
    {
        return nullptr;
    }

    GTD_API void* MemPool::Alloc(const size_t& size, RETCODE& out_retcode)
    {
        RETCODE retcode = RTN_OK;
        MemChunk* alloc_chunk = m_Cursor_Chunk;
        size_t allocated_size = 0;;
        size_t num_of_chunks = 0;
        alloc_chunk->m_Used_Size = size;
        void* out_memory = (void*)m_Cursor_Chunk->m_Data_Buffer;
        m_Cursor_Chunk->m_In_Use = true;
        m_Cursor_Chunk = m_Cursor_Chunk->m_Next_Chunk;

        while (allocated_size < size)
        {
            if (IS_NULL(m_Cursor_Chunk))
            {
                LOG_WARN("Memory Pool out of memory!");
                retcode |= RTN_MALLOC_FAIL;
                out_memory = nullptr;
                break;
            }

            allocated_size += m_Chunk_size;
            m_Cursor_Chunk->m_In_Use = true;
            m_Cursor_Chunk = m_Cursor_Chunk->m_Next_Chunk;
            num_of_chunks++;
        }

        m_Total_used += allocated_size;
        m_Total_free = m_Total_size - m_Total_used;

        LOG_DEBUG("Allocated %u bytes of memory in %u chunks totalling %u bytes of reserved space", size, num_of_chunks, allocated_size);
        LOG_DEBUG("Used a total of %u/%u total memory pool space (%.4f%%)", m_Total_used, m_Total_size, (double)m_Total_used / (double)m_Total_size);

        return out_memory;
    }

    GTD_API RETCODE MemPool::Alloc(const size_t& size, void** out_memory)
    {
        RETCODE retcode = RTN_OK;

        *out_memory = Alloc(size, retcode);

        return retcode;
    }

    static MemChunk* find_chunk(void* memory, MemChunk* all_chunks, size_t num_chunks)
    {
        MemChunk* ret = nullptr;
        size_t chunk_index = 0;
        for (; chunk_index < num_chunks; chunk_index++)
        {
            ret = &all_chunks[chunk_index];
            if (memory == ret->m_Data_Buffer)
            {
                return ret;
            }
        }

        return nullptr;
    }

    GTD_API RETCODE MemPool::Free(void* memory)
    {
        RETCODE retcode = RTN_OK;
        MemChunk* cursor_chunk = m_First_Chunk;

        while (!IS_NULL(cursor_chunk))
        {
            if (cursor_chunk->m_Data_Buffer == memory)
            {
                cursor_chunk->m_In_Use = false;
            }

            cursor_chunk = cursor_chunk->m_Next_Chunk;
        }
    }

    RETCODE MemPool::AllocatePoolMemory(const size_t& requested_size)
    {
        RETCODE retcode = RTN_OK;
        size_t best_chunk_size = 0;

        retcode = CalculateBestPoolSize(requested_size);

        m_Pool = (byte*)malloc(m_Total_size);

        if (IS_NULL(m_Pool))
        {
            LOG_FATAL("Could not allocate %u bytes of memory for memory pool!", m_Total_size);
            retcode |= RTN_MALLOC_FAIL;
        }

        return retcode;
    }

    RETCODE MemPool::CalculateBestPoolSize(const size_t& requested_size)
    {
        RETCODE retcode = RTN_OK;
        m_Number_of_Chunks = 0;
        if (0 != requested_size % m_Chunk_size)
        {
            // If requested size is not a multiple of chunk size we simply allocate an extra block
            m_Number_of_Chunks = 1;

            LOG_INFO("Requested memory size (%u bytes) is not divisible by chunk size (%u bytes). Allocating enough memory to cover the difference.", requested_size, m_Chunk_size);
        }

        m_Number_of_Chunks += requested_size / m_Chunk_size;
        m_Total_size = m_Number_of_Chunks * m_Chunk_size;
        
        LOG_DEBUG("Total Memory Pool size: %u bytes (%.2f MB)", m_Total_size, (double)m_Total_size / 1000000.0);

        return retcode;
    }

    RETCODE MemPool::CreateMemChunks(void)
    {
        RETCODE retcode = RTN_OK;
        size_t mem_chunk_index = 0;
        const byte* pool_cursor = m_Pool;
        MemChunk* previous = nullptr;

        m_All_chunks = (MemChunk*)calloc(m_Number_of_Chunks , sizeof(MemChunk));

        if (IS_NULL(m_All_chunks))
        {
            LOG_FATAL("Coult not create memory for mempool chunk pointers!");
            retcode |= RTN_MALLOC_FAIL;
            m_Number_of_Chunks = 0;
        }

        for (; mem_chunk_index < m_Number_of_Chunks; mem_chunk_index++)
        {
            m_All_chunks[mem_chunk_index].Initialize( pool_cursor, m_Chunk_size, mem_chunk_index < m_Number_of_Chunks - 1 ? &m_All_chunks[mem_chunk_index + 1] : nullptr, mem_chunk_index <= 0 ? nullptr : &m_All_chunks[mem_chunk_index - 1] );
            pool_cursor += m_Chunk_size;
        }

        m_First_Chunk = m_Cursor_Chunk = m_All_chunks;
        m_Last_Chunk = &m_All_chunks[m_Number_of_Chunks - 1];

        return retcode;
    }
}
