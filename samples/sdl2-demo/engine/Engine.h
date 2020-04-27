//
//  Engine.h
//  SimpleGameFramework
//
//  Created by Edward Rudd on 5/11/15.
//
//

#pragma once

#include "Renderer.h"
#include "Scene.h"
#include "InputManager.h"

#include <map>
#include <string>
#include <memory>

class Engine {
	std::unique_ptr<Renderer> m_renderer;
	InputManager m_input;

	std::map<std::string, std::unique_ptr<Scene>> m_scenes;
	SceneRef m_activeScene;
	std::string m_activeSceneName;
	bool m_shouldQuit = false;
public:
	explicit Engine(std::unique_ptr<Renderer> renderer);

	template<class R, typename ... Args>
	static Engine* create(Args&& ...args);

	~Engine();

	InputManager& input() { return m_input; }

	Renderer& renderer() { return *m_renderer; }

	void update(float delta);

	void render();

	template<class S, typename... Args>
	bool registerScene(const std::string& name, Args&& ...args);

	bool activateScene(const std::string& name);

	void setShouldQuit() { m_shouldQuit = true; }

	bool shouldQuit() const { return m_shouldQuit; }
};

#include "Engine_inline.h"