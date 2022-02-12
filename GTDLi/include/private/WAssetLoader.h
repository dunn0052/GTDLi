#pragma once
#include "public/IAssetLoader.h"
#include "private/GUIDGenerator.h"
#include <Windows.h>

namespace GTDLi
{
	class WAssetLoader : public IAssetLoader
	{
	public:

		GTD_API virtual RETCODE OnUpdate(Timestep& dt);

		GTD_API virtual RETCODE LoadAsset(AssetProps& props);

		GTD_API virtual RETCODE LoadAsset(const std::string& path, GUID64& out_guid);

		GTD_API virtual RETCODE GetAsset(const GUID64& guid, const Asset** out_asset);

		GTD_API virtual RETCODE RemoveAsset(const GUID64& guid);

		WAssetLoader() { }
		~WAssetLoader() { }
	private:

		static std::unordered_map<GUID64, Asset*, GUID64HashFunction> m_FileMap;

		static size_t m_TotalAssetSize;
	};
}