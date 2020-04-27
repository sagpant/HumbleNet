//
//  Color.h
//  SimpleGameFramework
//
//  Created by Edward Rudd on 2/20/15.
//
//

#pragma once

struct Color {
	unsigned char r, g, b, a;

	Color() : r( 0 ), g( 0 ), b( 0 ), a( 255 ) {}

	Color(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a = 255) : r( _r ), g( _g ), b( _b ),
																						  a( _a ) {}
};