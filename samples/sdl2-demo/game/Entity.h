//
//  Entity.h
//  SimpleGameFramework
//
//  Created by Edward Rudd on 3/3/15.
//
//

#pragma once

#include "engine/Sprite.h"
#include "engine/Updatable.h"
#include "engine/Rect.h"

class Entity : public Sprite, public Updatable {
	bool m_active;
	Vector2f m_velocity;
	Rectf m_bounds;
public:
	Entity(TextureRef texture, const Vector2f& vector, const Rectf& bounds);

	bool active() const { return m_active; }

	void activate() { m_active = true; }

	void deactivate() { m_active = false; }

	Vector2f velocity() const { return m_velocity; }

	void set_velocity(const Vector2f& velocity) { m_velocity = velocity; }

	bool update(float delta) override;

	bool collides_with(const Entity& other);
};