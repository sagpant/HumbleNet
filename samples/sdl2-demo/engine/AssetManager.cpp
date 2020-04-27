//
//  AssetManager.cpp
//  SimpleGameFramework
//
//  Created by Edward Rudd on 2/25/15.
//
//

#include "AssetManager.h"
#include "FileSystem.h"

#include <stdexcept>

AssetManager* AssetManager::m_defaultManager = nullptr;

AssetManager& AssetManager::DefaultManager()
{
	if (m_defaultManager) {
		return *m_defaultManager;
	} else {
		throw std::runtime_error( "No Default Manager Configured!" );
	}
}

AssetManager::AssetManager() = default;

void AssetManager::add_path(const std::string& path)
{
	m_search_paths.push_back( path );
}

std::string AssetManager::find_resource(const std::string& name, const std::string& type)
{
	std::string filename = name + "." + type;
	for (auto& m_search_path : m_search_paths) {
		std::string full_path = FileSystem::join_path( m_search_path, filename );
		if (FileSystem::exists( full_path )) {
			return full_path;
		}
	}
	return "";
}