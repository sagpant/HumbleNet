//
//  Engine.cpp
//  SimpleGameFramework
//
//  Created by Edward Rudd on 5/11/15.
//
//

#include "Engine.h"

Engine::Engine(std::unique_ptr<Renderer> renderer)
		: m_renderer( std::move( renderer )), m_activeScene( Scene::Null )
{
	m_renderer->set_engine( this );
}

Engine::~Engine() = default;

void Engine::update(float delta)
{
	m_activeScene->update( m_input, delta );
}

void Engine::render()
{
	m_renderer->clear();

	m_activeScene->render();

	m_renderer->present();
}

bool Engine::activateScene(const std::string& name)
{
	auto it = m_scenes.find( name );
	if (it != m_scenes.end()) {
		auto new_scene = it->second.get();
		m_activeScene->onHide( name, new_scene );
		new_scene->onShow( m_activeSceneName, m_activeScene );
		m_activeScene = new_scene;
		m_activeSceneName = name;
		return true;
	}
	return false;
}