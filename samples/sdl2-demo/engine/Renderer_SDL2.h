//
//  Renderer_SDL2.h
//  SimpleGameFramework
//
//  Created by Edward Rudd on 2/20/15.
//
//

#pragma once

#include "Renderer.h"

#include <SDL.h>

class Renderer_SDL2 : public Renderer {
	friend class Texture_SDL2;

	SDL_Window* m_win;
	SDL_Renderer* m_rend;
public:
	Renderer_SDL2(SDL_Window* win, SDL_Renderer* rend);

	void set_logical_size(Vector2i size, bool keepAspect) override;

	void set_swap_interval(int32_t swap) override;

	void update_window_size() override;

	void clear(const Color& c) override;

	void draw_rect(const Recti& r, const Color& c) override;

	void draw_rect_fill(const Recti& r, const Color& c) override;

	void present() override;

	TextureRef load_texture(const std::string& name) override;

	void draw_texture(TextureRef texture, const Recti& src, const Recti& dst, float angle) override;

private:
	void set_draw_color(const Color& c);
};
