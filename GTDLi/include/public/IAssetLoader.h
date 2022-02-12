#pragma once
#include "Retcodes.h"
#include "public/Core.h"
#include "private/Timestep.h"
#include "public/References.h"
#include "private/GUIDGenerator.h"
#include "private/CommonMacros.h"
#include <string>

namespace GTDLi
{
	struct AssetProps
	{
		RETCODE retcode;
		const std::string path;
		GUID64 guid;

		AssetProps(const std::string& path) : path(path), retcode(RTN_FAIL) {};
	};

	struct Asset
	{
		char* buffer;
		size_t size;
		GUID64 guid;
		const std::string path;

		Asset(char* buffer, size_t& size, GUID64& guid, const std::string& path)
			: buffer(buffer), size(size), guid(guid), path(path)
		{
		}

		~Asset()
		{
			//SAFE_DELETE(buffer);
		}
	};

	class IAssetLoader
	{
	public:
		GTD_API static IAssetLoader& Instance(void);

		GTD_API virtual ~IAssetLoader() {};

		GTD_API virtual RETCODE OnUpdate(Timestep& dt) = 0;

		GTD_API virtual RETCODE LoadAsset(AssetProps& props) = 0;
		GTD_API virtual RETCODE LoadAsset(const std::string& path, GUID64& out_guid) = 0;

		GTD_API virtual RETCODE GetAsset(const GUID64& guid, const Asset** out_asset) = 0;

		GTD_API virtual RETCODE RemoveAsset(const GUID64& guid) = 0;


	};
}