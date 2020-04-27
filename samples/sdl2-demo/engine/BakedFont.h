//
//  BakedFont.h
//  SimpleGameFramework
//
//  Created by Edward Rudd on 4/29/15.
//
//

#pragma once

#include "Rect.h"
#include "Texture.h"
#include "Font.h"

#include <unordered_map>

class BakedFont;

typedef BakedFont* BakedFontRef;

class Renderer;

class BakedFont {
public:
    static const char32_t WhiteSquare;

	struct Config {
		int16_t ascender = 0;
		int16_t descender = 0;
		int16_t lineGap = 0;
	};

	struct BakedData {
		Recti rect;
		int16_t advance;

		BakedData(const Recti& r, int16_t a) : rect( r ), advance( a ) {}
	};

	typedef std::unordered_map<char32_t, BakedData> BakedPositions;

	typedef std::tuple<char32_t, char32_t> kerning_key;

	struct kerning_key_hash : public std::unary_function<kerning_key, std::size_t> {
		std::size_t operator()(const kerning_key& k) const
		{
			return std::hash<uint64_t>{}((uint64_t( std::get<0>( k )) << 32u) + uint64_t( std::get<1>( k )));
		}
	};

	class Kerning : public std::unordered_map<kerning_key, int16_t, kerning_key_hash> {
	public:
		void add(char32_t g1, char32_t g2, int16_t kern);

		int16_t get(char32_t g1, char32_t g2);
	};

protected:
	TextureRef m_texture = Texture::Null;
	BakedPositions m_positions;
	Config m_config;
	Kerning m_kerning;
public:
	BakedFont(TextureRef texture, Config config, BakedPositions positions, Kerning kerning);

	~BakedFont();

	static BakedFontRef load(const std::string& file_name, TextureRef texture);

	TextureRef texture() { return m_texture; }

	BakedPositions& positions() { return m_positions; }

	int16_t get_kerning(char32_t prev_glyph, char32_t glyph)
	{
		return m_kerning.get( prev_glyph, glyph );
	}

	bool get_metrics(int16_t& ascender, int16_t& descender, int16_t& lineGap)
	{
		ascender = m_config.ascender;
		descender = m_config.descender;
		lineGap = m_config.lineGap;
		return true;
	}

	int16_t height() const { return m_config.ascender - m_config.descender; }

	static BakedFontRef Null;
};
