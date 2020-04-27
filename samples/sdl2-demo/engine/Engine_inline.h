//
//  Engine.h
//  SimpleGameFramework
//
//  Created by Edward Rudd on 4/20/20.
//

#pragma once

template<typename R, typename ...Args>
Engine* Engine::create(Args&& ... args)
{
	return new Engine( std::unique_ptr<Renderer>( new R( std::forward<Args>( args )... )));
}

template<class S, typename... Args>
bool Engine::registerScene(const std::string& name, Args&& ...args)
{
	if (m_scenes.count( name ) > 0) {
		return false;
	}
	auto ptr = std::unique_ptr<Scene>( new S( *m_renderer, std::forward<Args>( args )... ));
	auto it = m_scenes.emplace( std::piecewise_construct,
								std::forward_as_tuple( name ),
								std::forward_as_tuple( std::move( ptr )));
	return it.second;
}
