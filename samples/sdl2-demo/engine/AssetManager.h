//
//  AssetManager.h
//  SimpleGameFramework
//
//  Created by Edward Rudd on 2/25/15.
//
//

#pragma once

#include <vector>
#include <string>

class AssetManager {
	static AssetManager* m_defaultManager;

	std::vector<std::string> m_search_paths;
public:
	static AssetManager& DefaultManager();

	static void setDefaultManager(AssetManager* manager) { m_defaultManager = manager; }

	AssetManager();

	void add_path(const std::string& path);

	std::string find_resource(const std::string& name, const std::string& type);
};