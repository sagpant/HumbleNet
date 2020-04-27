//
//  Enemy.h
//  SimpleGameFramework
//
//  Created by Edward Rudd on 2/27/15.
//
//

#pragma once

#include "Entity.h"

class Enemy : public Entity {
	float m_rotate_speed;
public:
	Enemy(TextureRef texture, const Vector2f& vector, const Rectf& bounds, float rotate_speed);

	bool update(float delta) override;
};
