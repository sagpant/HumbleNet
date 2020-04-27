//
//  Texture_SDL2.h
//  SimpleGameFramework
//
//  Created by Edward Rudd on 2/25/15.
//
//

#pragma once

#include "Renderer_SDL2.h"

#include <SDL_render.h>

class Texture_SDL2 : public Texture {
	friend class Renderer_SDL2;

	SDL_Texture* m_texture;
public:
	void set_color_mod(const Color& c) override;

	Texture_SDL2(Renderer_SDL2& rend, const std::string& filename);

	Texture_SDL2(Renderer_SDL2& rend, int32_t width, int32_t height);

	Texture_SDL2(Renderer_SDL2& rend, SDL_Surface* texture);

	~Texture_SDL2() override;
};