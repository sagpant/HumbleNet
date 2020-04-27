//
//  Renderer.h
//  SimpleGameFramework
//
//  Created by Edward Rudd on 2/20/15.
//
//

#pragma once

#include "Color.h"
#include "Rect.h"
#include "Texture.h"
#include "BakedFont.h"

class Engine;

#include <string>
#include <unordered_map>

class Renderer {
protected:
	Vector2i m_size, m_logical_size;
	bool m_logical_set = false;
	Engine* m_engine = nullptr;

	std::unordered_map<std::string, TextureRef> m_textureCache;
	std::unordered_map<std::string, BakedFontRef> m_bakedFontCache;

	void set_engine(Engine* engine) { m_engine = engine; }

	friend class Engine;

public:
	Renderer() noexcept {};

	virtual ~Renderer();

	virtual void set_logical_size(Vector2i size, bool keepAspect = true) = 0;

	virtual void set_swap_interval(int32_t swap) = 0;

	virtual void update_window_size() = 0;

	virtual void clear(const Color& c = Color()) = 0;

	virtual void present() = 0;

	virtual void draw_rect(const Recti& r, const Color& c) = 0;

	virtual void draw_rect_fill(const Recti& r, const Color& c) = 0;

	virtual TextureRef load_texture(const std::string& name) = 0;

	virtual BakedFontRef load_baked_font(const std::string& name);

	void draw_text(BakedFontRef baked, const Vector2i& offset, const std::string& text, uint32_t flags = 0,
				   const Color& c = Color( 255, 255, 255, 255 ));

	void draw_texture(TextureRef texture, const Vector2i& dst, float angle = 0.0f);

	virtual void draw_texture(TextureRef texture, const Recti& src, const Recti& dst, float angle = 0.0f) = 0;

	const Vector2i& size() const { return m_size; }

	int32_t width() const { return m_size.x; }

	int32_t height() const { return m_size.y; }

	const Vector2i& logical_size() const { return m_logical_size; }

	Engine* engine() const { return m_engine; }

	static Renderer* Null;
};