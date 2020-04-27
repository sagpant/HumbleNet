//
//  Texture.h
//  SimpleGameFramework
//
//  Created by Edward Rudd on 2/25/15.
//
//

#pragma once

#include "Vector2.h"

struct Color;

class Texture;

typedef Texture* TextureRef;

class Texture {
protected:
	Vector2i m_size;
	int8_t m_depth;

	Texture() noexcept: m_depth( 0 ) {}

public:
	virtual ~Texture() = default;

	int32_t width() { return m_size.x; }

	int32_t height() { return m_size.y; }

	int32_t depth() { return m_depth; }

	const Vector2i& size() { return m_size; }

	virtual void set_color_mod(const Color& c) = 0;

	static TextureRef Null;
};
