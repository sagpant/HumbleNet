//
//  Game.cpp
//  SimpleGameFramework
//
//  Created by Edward Rudd on 2/20/15.
//
//

#include "Game.h"

#include "engine/Math.h"
#include "engine/Utilities.h"

#include <cmath>

#include <random>
#include <algorithm>

#define PLAYER_X_SPEED      384.0f
#define TORPEDO_SPEED       448.0f
#define TORPEDO_RELOAD_TIME 0.4f

#define ENEMY_MAX           3
#define ENEMY_PROBABILITY   5
#define ENEMY_ROTATION      45.0f
#define ENEMY_SPEED         64.0f
#define ENEMY_START_BASE    80.0f
#define ENEMY_START_RANGE   50.0f

Game::Game(Renderer& r) : Scene( r ), m_Player( r.load_texture( "spaceship" ), TORPEDO_RELOAD_TIME )
{
	add_renderable( &m_Player );

	float yPos = m_renderer.logical_size().y - m_Player.size().y / 2;

	m_Player.set_move_range( Vector2f( m_Player.center().x, yPos ),
							 Vector2f( m_renderer.logical_size().x - m_Player.center().x, yPos ));
	m_Player.set_position( Vector2f( m_renderer.logical_size().x / 2.0f, yPos ));
}

void Game::update(const InputManager& input, float delta)
{
	for (auto it = m_updatables.begin(); it != m_updatables.end();) {
		bool keep = (*it)->update( delta );
		if (!keep) {
			auto* o = dynamic_cast<Renderable*>(*it);
			++it;
			remove_renderable( o );
		} else {
			++it;
		}
	}

	auto xAxis = input[InputManager::Force_X_AXIS];
	if (xAxis.is_set()) {
		float distance = xAxis.value * delta * PLAYER_X_SPEED;
		m_Player.move_position_by( Vector2f( distance, 0.0f ));
	}
	if (input[InputManager::Force_SHOOT].is_set()) {
		fire_torpedo( m_Player );
	}
	if (input[InputManager::Force_BACK].is_set()) {
		activateScene( "main_menu" );
	}
	check_enemies();
	check_collisions();
}

void Game::add_renderable(Renderable* renderable, int32_t layer)
{
	if (!renderable) return;

	auto* updatable = dynamic_cast<Updatable*>(renderable);
	if (updatable) {
		m_updatables.push_back( updatable );
	}
	m_renderables[layer].push_back( renderable );
}

void Game::remove_renderable(Renderable* renderable, bool updatable_also)
{
	if (renderable == nullptr) return;

	for (auto& m_renderable : m_renderables) {
		m_renderable.remove( renderable );
	}
	if (updatable_also) {
		auto* updatable = dynamic_cast<Updatable*>(renderable);
		if (updatable) {
			m_updatables.remove( updatable );
		}
	}
}

void Game::render()
{
	m_renderer.draw_rect_fill( m_renderer.logical_size(), Color( 100, 50, 20 ));

	for (int32_t l = MAX_LAYERS - 1; l >= 0; --l) {
		for (auto& it : m_renderables[l]) {
			it->render( m_renderer );
		}
	}
}

template<typename T>
bool isActive(const T& o)
{
	return o.active();
}

void Game::fire_torpedo(Player& player)
{
	if (!player.can_shoot() || !player.fire_torpedo()) {
		return;
	}

	auto it = find_or_create( m_projectiles,
							  isActive<decltype(m_projectiles)::value_type>,
							  m_renderer.load_texture( "torpedo" ),
							  Vector2f( 0, -TORPEDO_SPEED ),
							  Recti( m_renderer.logical_size()).as<float>());

	it->set_position( player.position());
	it->activate();

	add_renderable( &(*it), 1 );
}

void Game::check_collisions()
{
	for (auto& m_projectile : m_projectiles) {
		if (!m_projectile.active()) continue;
		for (auto& m_enemy : m_enemies) {
			if (!m_enemy.active()) continue;

			if (m_projectile.collides_with( m_enemy )) {
				m_projectile.deactivate();
				m_enemy.deactivate();

				Rectf bounds = Recti( m_renderer.logical_size()).as<float>();

				auto it = find_or_create( m_explosions,
										  isActive<decltype(m_explosions)::value_type>,
										  m_renderer.load_texture( "explosion" ),
										  bounds );
				it->set_duration( 0.35f );
				it->set_velocity( m_enemy.velocity() * 0.75f );
				it->set_position( m_enemy.position());
				it->activate();
				add_renderable( &(*it), 0 );
			}
		}
		if (!m_projectile.active()) {
			Rectf bounds = Recti( m_renderer.logical_size()).as<float>();

			auto it = find_or_create( m_explosions,
									  isActive<decltype(m_explosions)::value_type>,
									  m_renderer.load_texture( "explosion" ), bounds );
			it->set_duration( 0.2f );
			it->set_velocity( m_projectile.velocity() / 2 );
			it->set_position( m_projectile.position());
			it->activate();
			add_renderable( &(*it), 0 );
		}
	}
}

static std::default_random_engine rand_generator;
static std::uniform_int_distribution<int32_t> rand_percentage( 0, 100 );
static std::uniform_real_distribution<float> rand_angle( 0.0f, 90.0f );
static std::uniform_real_distribution<float> rand_enemy_start( 0, ENEMY_START_RANGE );

void Game::check_enemies()
{
	if (std::count_if( m_enemies.begin(), m_enemies.end(), isActive<Enemy> ) < ENEMY_MAX) {
		if (rand_percentage( rand_generator ) < ENEMY_PROBABILITY) {
			float angle = rand_angle( rand_generator );
			angle -= 45.0f;
			if (rand_percentage( rand_generator ) >= 50.0f) {
				angle += 180.0f;
			}
			angle = Math::to_radians( angle );

			Vector2f velocity( std::cos( angle ), std::sin( angle ));

			auto it = find_or_create( m_enemies,
									  isActive<decltype(m_enemies)::value_type>,
									  m_renderer.load_texture( "ufo" ),
									  velocity * ENEMY_SPEED,
									  Recti( m_renderer.logical_size()).as<float>(),
									  ENEMY_ROTATION );

			Vector2f position( -it->size().x / 2, ENEMY_START_BASE + rand_enemy_start( rand_generator ));
			if (velocity.x < 0) {
				position.x = m_renderer.logical_size().x + it->size().x / 2;
			}
			it->set_position( position );
			it->activate();
			add_renderable( &(*it), 0 );
		}
	}
}
