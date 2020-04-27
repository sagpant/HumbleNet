//
//  Texture.cpp
//  SimpleGameFramework
//
//  Created by Edward Rudd on 2/25/15.
//
//

#include "Texture.h"

class NullTexture : public Texture {
public:
	NullTexture() noexcept = default;

	void set_color_mod(const Color& c) override {}
};

static NullTexture nullTexture;

TextureRef Texture::Null = &nullTexture;
