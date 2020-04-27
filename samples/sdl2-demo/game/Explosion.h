//
//  Explosion.h
//  SimpleGameFramework
//
//  Created by Edward Rudd on 3/4/15.
//
//

#pragma once

#include "Entity.h"

class Explosion : public Entity {
	float m_duration;
public:
	Explosion(TextureRef texture, const Rectf& bounds);

	float duration() { return m_duration; }

	void set_duration(float duration) { m_duration = duration; }

	bool update(float delta) override;
};