//
//  Scene.cpp
//  SimpleGameFramework
//
//  Created by Edward Rudd on 5/11/15.
//
//

#include "Scene.h"
#include "Renderer.h"
#include "Engine.h"

class NullScene : public Scene {
public:
	NullScene() noexcept: Scene( *Renderer::Null ) {}

	void update(const InputManager& input, float delta) override {}

	void render() override {}
};

static NullScene nullScene;

SceneRef Scene::Null = &nullScene;

Scene::Scene(Renderer& rend)
		: m_renderer( rend )
{
}

bool Scene::activateScene(const std::string& name)
{
	return engine()->activateScene( name );
}

Engine* Scene::engine() const
{
	return m_renderer.engine();
}