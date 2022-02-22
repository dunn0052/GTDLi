#include "PCH.h"
#include "private/WAssetLoader.h"
#include "private/GUIDGenerator.h"
#include "private/CommonMacros.h"
#include "private/Logger.h"
#include "sys/stat.h"
#include "private/MemPool.h"

namespace GTDLi
{

	constexpr int STAT_OK = 0;

	int stat_with_error_logging(const char* path, struct stat* const status);

	GTD_API RETCODE WAssetLoader::OnUpdate(Timestep& dt)
	{
		return RTN_OK;
	}

	static int stat_with_error_logging(const char* path, struct stat* const status)
	{
		int stat_error = EIO;

		/*Probably should use CFile stat*/
		stat_error = stat(path, status);

		switch (stat_error)
		{
		case EACCES:
			LOG_WARN("Search permission is denied for component of path prefix for path: %s", path);
			break;
		case EIO:
			LOG_WARN("Genral error when opening path: %s", path);
			break;
		case ELOOP:
			LOG_WARN("A loop exists in symbolic links encountered during resolution of path: %s", path);
			break;
		case ENAMETOOLONG:
			LOG_WARN("The length of path: %s exceeds the path max of: %d or a pathname component is longer than %d", path, _MAX_PATH, FILENAME_MAX);
			break;
		case ENOENT:
			LOG_WARN("A component of path: %s does not name an existing file or is an empty string", path);
			break;
		case ENOTDIR:
			LOG_WARN("The path: %s is not a directory", path);
			break;
		case EOVERFLOW:
			LOG_WARN("The file size for path: %s in bytes or the number of blocks allocated to the file or the file serial number cannot be represented correctly in the structure pointer to the buffer");
			break;
		case STAT_OK:
			LOG_DEBUG("Loaded path: %s", path);
			break;

		}

		return stat_error;
	}

	GTD_API RETCODE WAssetLoader::LoadAsset(AssetProps& props)
	{
		props.retcode = LoadAsset(props.path, props.guid);
		return props.retcode;
	}

	GTD_API RETCODE WAssetLoader::LoadAsset(const std::string& path, GUID64& out_guid)
	{
		RETCODE retcode = RTN_OK;
		std::size_t asset_size = 0;
		char* asset_buffer = nullptr;
		std::fstream asset(path, std::ios::in | std::ios::binary | std::ios::ate);

		if (asset.is_open())
		{
			asset_size = asset.tellg();
			//asset_buffer = new (std::nothrow) char[asset_size];
			asset_buffer = (char*)MemPool::Instance().Alloc(asset_size * sizeof(char), retcode);

			if (nullptr != asset_buffer)
			{
				asset.seekg(0, std::ios::beg);
				asset.read(asset_buffer, asset_size);
				if (asset.good())
				{
					RETURN_RETCODE_IF_NOT_OK(GUIDGenerator::Instance().GenerateGUID(out_guid));
					m_FileMap.insert({ out_guid, new Asset(asset_buffer, asset_size, out_guid, path) });
					m_TotalAssetSize += asset_size;

					LOG_DEBUG("Assigned %s to GUID %x-%x", path.c_str(), out_guid.high, out_guid.low);
					LOG_DEBUG("Total buffer size: %u (bytes)", m_TotalAssetSize);

					retcode |= RTN_OK;

				}
				else
				{
					LOG_WARN("Could not read file %s into memory!", path.c_str());
					retcode |= RTN_FAIL;
				}

			}
			else
			{
				LOG_FATAL("Could not allocate space for file: %s", path.c_str());
				retcode |= RTN_MALLOC_FAIL;
			}

			asset.close();

		}
		else
		{
			LOG_WARN("Couldn't open file: %s ", path.c_str());
			retcode |= RTN_FAIL;
		}

		return retcode;
	}

	GTD_API RETCODE WAssetLoader::GetAsset(const GUID64& guid, const Asset** out_asset)
	{
		RETCODE retcode = RTN_OK;

		std::unordered_map<GUID64, Asset*>::const_iterator element = m_FileMap.find(guid);

		if (m_FileMap.end() != element)
		{
			LOG_DEBUG("Found asset GUID: %x-%x", element->first.high, element->first.low);
			*out_asset = element->second;
		}
		else
		{
			LOG_WARN("Could not find asset for GUID: %x-%x", guid.high, guid.low);
			*out_asset = nullptr;
			retcode |= RTN_NOT_FOUND;
		}

		return retcode;
	}

	constexpr size_t ELEMENT_EXISTS = 1;

	GTD_API RETCODE WAssetLoader::RemoveAsset(const GUID64& guid)
	{
		RETCODE retcode = RTN_OK;
		const Asset* asset = nullptr;

		RETURN_RETCODE_IF_NOT_OK(GetAsset(guid, &asset));

		m_TotalAssetSize -= asset->size;
		
		delete asset;

		size_t element = m_FileMap.erase(guid);

		if (ELEMENT_EXISTS == element)
		{

			LOG_DEBUG("Removed asset with GUID: %x-%x", guid.high, guid.low);
			LOG_DEBUG("Total buffer size: %u (bytes)", m_TotalAssetSize);
			retcode |= RTN_OK;
		}
		else
		{
			LOG_WARN("Could not remove asset with GUID: %x-%x", guid.high, guid.low);
			retcode |= RTN_NOT_FOUND;
		}

		return retcode;
	}

	std::unordered_map<GUID64, Asset*, GUID64HashFunction> WAssetLoader::m_FileMap;
	size_t WAssetLoader::m_TotalAssetSize;

}