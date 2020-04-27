//
//  Scene.h
//  SimpleGameFramework
//
//  Created by Edward Rudd on 5/11/15.
//
//

#pragma once

#include "Renderable.h"
#include "Updatable.h"
#include "InputManager.h"

#include <string>

class Engine;

class Scene;

typedef Scene* SceneRef;

class Scene {
protected:
	Renderer& m_renderer;
public:
	explicit Scene(Renderer& renderer);

	virtual ~Scene() = default;

	static SceneRef Null;

	virtual void update(const InputManager& input, float delta) = 0;

	virtual void render() = 0;

	Engine* engine() const;

	bool activateScene(const std::string& name);

protected:
	virtual void onShow(const std::string& previous_name, const SceneRef previous) {}

	virtual void onHide(const std::string& next_name, const SceneRef next) {}

	friend Engine;
};