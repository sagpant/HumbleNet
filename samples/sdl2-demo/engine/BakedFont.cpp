//
//  BakedFont.cpp
//  SimpleGameFramework
//
//  Created by Edward Rudd on 4/29/15.
//
//

#include "BakedFont.h"

#include "Renderer.h"
#include "FileSystem.h"

#include <utility>
#include <algorithm>
#include <locale>
#include <codecvt>
#include <cstdio>

class NullBakedFont : public BakedFont {
public:
	NullBakedFont() noexcept: BakedFont( Texture::Null, {}, {}, {} ) {}
};

static NullBakedFont nullBakedFont;

BakedFontRef BakedFont::Null = &nullBakedFont;

const char32_t BakedFont::WhiteSquare = 9633; // U+25A1

BakedFont::BakedFont(TextureRef texture, Config config, BakedPositions positions, Kerning kerning)
		: m_texture( texture ), m_config( std::move( config )), m_positions( std::move( positions )),
		  m_kerning( std::move( kerning ))
{
}

BakedFont::~BakedFont() = default;

BakedFontRef BakedFont::load(const std::string& file_name, TextureRef texture)
{
	auto fp = FileSystem::open( file_name, "rb" );
	int8_t height = 0;
	bool good = true;
	BakedFont::Config config;
	BakedFont::BakedPositions positions;
	BakedFont::Kerning kerning;
	if (fp) {
		char buff[1024];
		while (!feof( fp )) {
			std::fgets( buff, sizeof( buff ), fp );
			auto len = std::strlen( buff );
			if (len == 0) continue;
			if (len == 1 && std::isspace( buff[0] )) continue;
			if (buff[0] == '#') continue;

			if (std::strncmp( buff, "BAKED ", 6 ) == 0) {
				int a, d, l;
				auto found = std::sscanf( buff + 6, "%d,%d,%d", &a, &d, &l );
				if (found == 3) {
					config.ascender = a;
					config.descender = d;
					config.lineGap = l;
				}
			} else if (std::strncmp( buff, "POS ", 4 ) == 0) {
				int ch, a, x, y, w, h;
				auto found = std::sscanf( buff + 4, "%d,%d,%d,%d,%d,%d", &ch, &a, &x, &y, &w, &h );
				if (found == 6) {
					positions.emplace( std::piecewise_construct,
									   std::forward_as_tuple( ch ),
									   std::forward_as_tuple( Recti( x, y, w, h ), a ));
				}
			} else if (std::strncmp( buff, "KERN ", 5 ) == 0) {
				int c1, c2, a;
				auto found = std::sscanf( buff + 5, "%d,%d,%d", &c1, &c2, &a );
				if (found == 3) {
					kerning.add( c1, c2, a );
				}
			}
		}
		fclose( fp );
	} else {
		good = false;
	}
	if (good) {
		return new BakedFont( texture, config, positions, kerning );
	}
	return BakedFont::Null;
}

void BakedFont::Kerning::add(char32_t g1, char32_t g2, int16_t kern)
{
	emplace( std::make_tuple( g1, g2 ), kern );
}

int16_t BakedFont::Kerning::get(char32_t g1, char32_t g2)
{
	auto it = find( {g1, g2} );
	if (it != end()) {
		return it->second;
	}
	return 0;
}
