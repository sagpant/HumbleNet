//
//  Game.h
//  SimpleGameFramework
//
//  Created by Edward Rudd on 2/20/15.
//
//

#pragma once

#include "engine/Renderer.h"
#include "Player.h"
#include "engine/Renderable.h"
#include "engine/Scene.h"
#include "Enemy.h"
#include "Explosion.h"

#include <list>

class Game : public Scene {
public:
private:
	Player m_Player;

	std::list<Projectile> m_projectiles;
	std::list<Enemy> m_enemies;
	std::list<Explosion> m_explosions;

	static const int32_t MAX_LAYERS = 2;

	std::list<Renderable*> m_renderables[MAX_LAYERS];
	std::list<Updatable*> m_updatables;
public:
	explicit Game(Renderer& r);

	void update(const InputManager& input, float delta) override;

	void render() override;

	void add_renderable(Renderable* renderable, int32_t layer = 0);

	void remove_renderable(Renderable* renderable, bool updatable_also = true);

private:
	void fire_torpedo(Player& player);

	void check_enemies();

	void check_collisions();
};