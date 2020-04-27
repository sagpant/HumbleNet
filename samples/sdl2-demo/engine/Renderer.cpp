//
//  Renderer.cpp
//  SimpleGameFramework
//
//  Created by Edward Rudd on 2/20/15.
//
//

#include "Renderer.h"
#include "AssetManager.h"
#include "FileSystem.h"
#include "Font.h"

#include <codecvt>
#include <locale>

class NullRenderer : public Renderer {
	void set_logical_size(Vector2i size, bool keepAspect) override {}

	void set_swap_interval(int32_t swap) override {}

	void update_window_size() override {}

	void clear(const Color& c) override {}

	void present() override {}

	void draw_rect(const Recti& r, const Color& c) override {}

	void draw_rect_fill(const Recti& r, const Color& c) override {}

	TextureRef load_texture(const std::string& name) override { return Texture::Null; }

	void draw_texture(TextureRef texture, const Recti& src, const Recti& dst, float angle) override {}

public:
	NullRenderer() noexcept = default;

};

static NullRenderer nullRenderer;

Renderer* Renderer::Null = &nullRenderer;

Renderer::~Renderer()
{
	for (auto& it : m_textureCache) {
		delete it.second;
	}
	m_textureCache.clear();
	for (auto& it : m_bakedFontCache) {
		delete it.second;
	}
	m_bakedFontCache.clear();
}

void Renderer::draw_texture(TextureRef texture, const Vector2i& dst_pos, float angle)
{
	const Vector2i& size = texture->size();
	Recti src( size );
	Recti dst( dst_pos, size );

	draw_texture( texture, src, dst, angle );
}

BakedFontRef Renderer::load_baked_font(const std::string& name)
{
	auto it = m_bakedFontCache.find( name );
	if (it != m_bakedFontCache.end()) {
		return it->second;
	}
	std::string asset_file = AssetManager::DefaultManager().find_resource( name, "baked" );
	if (!asset_file.empty()) {
		auto texture = load_texture( name );
		auto font = BakedFont::load( asset_file, texture );
		if (font != BakedFont::Null) {
			m_bakedFontCache.emplace( name, font );
			return font;
		}
	}
	return BakedFont::Null;
}

void
Renderer::draw_text(BakedFontRef baked, const Vector2i& offset, const std::string& text, uint32_t flags, const Color& c)
{
	const BakedFont::BakedPositions& positions = baked->positions();
	auto tex = baked->texture();
	tex->set_color_mod( c );

	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> utf32conv;
	auto glyphs = utf32conv.from_bytes( text );

	Vector2i pen = offset;

	char32_t prev_glyph = -1;

	if ((flags & Font::ALIGN_H_MASK) > 0) {
		int16_t width = 0;
		for (auto glyph : glyphs) {
			auto data = positions.find( glyph );
			if (data == positions.end()) {
				glyph = BakedFont::WhiteSquare;
				data = positions.find( glyph );
			}
			int16_t kern = 0;
			if (prev_glyph != -1) {
				kern = baked->get_kerning( prev_glyph, glyph );
			}
			if (data != positions.end()) {
				width += data->second.advance + kern;
			}
			prev_glyph = glyph;
		}
		switch (flags & Font::ALIGN_H_MASK) {
			case Font::ALIGN_H_CENTER:
				pen.x -= width / 2;
				break;
			case Font::ALIGN_H_RIGHT:
				pen.x -= width;
				break;
			default:
				break;
		}
		prev_glyph = -1;
	}
	if ((flags & Font::ALIGN_V_MASK) > 0) {
		int16_t ascender, descender, lineGap;
		baked->get_metrics( ascender, descender, lineGap );
		switch (flags & Font::ALIGN_V_MASK) {
			case Font::ALIGN_V_CENTER:
				pen.y -= float( ascender - descender ) / 2;
				break;
			case Font::ALIGN_V_BOTTOM:
				pen.y -= float( ascender - descender );
				break;
			default:
				break;
		}
	}

	for (auto glyph : glyphs) {
		auto data = positions.find( glyph );
		if (data == positions.end()) {
			glyph = BakedFont::WhiteSquare;
			data = positions.find( glyph );
		}
		int16_t kern = 0;
		if (prev_glyph != -1) {
			kern = baked->get_kerning( prev_glyph, glyph );
		}
		if (data != positions.end()) {
			const auto& d = data->second;
			draw_texture( tex, d.rect, Recti( pen, d.rect.size()));
			pen.x += d.advance + kern;
		}
		prev_glyph = glyph;
	}
}
