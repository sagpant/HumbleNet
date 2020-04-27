//
//  Texture_SDL2.cpp
//  SimpleGameFramework
//
//  Created by Edward Rudd on 2/25/15.
//
//

#include "Texture_SDL2.h"

#include <SDL_LodePNG.h>

Texture_SDL2::Texture_SDL2(Renderer_SDL2& rend, const std::string& filename) : m_texture( nullptr )
{
	SDL_Surface* surface = SDL_LoadPNG( filename.c_str());
	if (surface) {
		m_texture = SDL_CreateTextureFromSurface( rend.m_rend, surface );
		if (m_texture) {
			m_size.x = surface->w;
			m_size.y = surface->h;
			m_depth = SDL_BYTESPERPIXEL( surface->format->format );
		}
		SDL_FreeSurface( surface );
	}
}

Texture_SDL2::Texture_SDL2(Renderer_SDL2& rend, int32_t width, int32_t height) : m_texture( nullptr )
{
	m_texture = SDL_CreateTexture( rend.m_rend,
								   SDL_PIXELFORMAT_RGBA8888,
								   SDL_TEXTUREACCESS_STATIC,
								   width, height );
	m_size.x = width;
	m_size.y = height;
	m_depth = SDL_BYTESPERPIXEL( SDL_PIXELFORMAT_RGBA8888 );
}

Texture_SDL2::Texture_SDL2(Renderer_SDL2& rend, SDL_Surface* surface) : m_texture( nullptr )
{
	m_texture = SDL_CreateTextureFromSurface( rend.m_rend, surface );
	m_size.x = surface->w;
	m_size.y = surface->h;
	m_depth = SDL_BYTESPERPIXEL( surface->format->format );
}

Texture_SDL2::~Texture_SDL2()
{
	if (m_texture) {
		SDL_DestroyTexture( m_texture );
	}
}

void Texture_SDL2::set_color_mod(const Color& c)
{
	SDL_SetTextureColorMod( m_texture, c.r, c.g, c.b );
	SDL_SetTextureAlphaMod( m_texture, c.a );
}
