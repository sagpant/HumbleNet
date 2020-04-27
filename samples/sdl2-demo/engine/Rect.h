//
//  Rect.h
//  SimpleGameFramework
//
//  Created by Edward Rudd on 2/20/15.
//
//

#pragma once

#include "Vector2.h"

template<typename T>
struct Rect {
	T x, y;
	T w, h;

	Rect() : x( T()), y( T()), w( T()), h( T()) {}

	Rect(T _x, T _y, T _w, T _h) : x( _x ), y( _y ), w( _w ), h( _h ) {}

	Rect(const Vector2<T>& size) : x( T()), y( T()), w( size.x ), h( size.y ) {}

	Rect(const Vector2<T>& position, const Vector2<T>& size) : x( position.x ), y( position.y ), w( size.x ),
															   h( size.y ) {}

	template<typename O>
	Rect<O> as() const
	{
		return Rect<O>( O( x ), O( y ), O( w ), O( h ));
	}

	Vector2<T> bottom_right() const { return Vector2<T>( x + w, y + h ); }

	Vector2<T> size() const { return Vector2<T>( w, h ); }

	bool empty() const
	{
		return w == T() && h == T();
	}

	bool contains(const Vector2<T>& point) const
	{
		Vector2<T> br = bottom_right();

		return point.x >= x && point.y >= y
			   && point.x < br.x && point.y < br.y;
	}

	bool intersects(const Rect<T>& r) const
	{
		if (empty() || r.empty()) return false;

		if (r.x < x + w && x < r.x + r.w && r.y < y + h)
			return y < r.y + r.h;

		return false;
	}

	Rect<T> expand(const Vector2<T>& delta, bool centered = false) const
	{
		Rect<T> r = *this;
		r.w += delta.x;
		r.h += delta.y;
		if (r.w < 0) r.w = 0;
		if (r.h < 0) r.h = 0;
		if (centered) {
			r.x -= delta.x / 2;
			r.y -= delta.y / 2;
		}
		return r;
	}

	Rect<T> expand(T delta, bool centered = false) const
	{
		return expand( Vector2<T>( delta ), centered );
	}

	Rect<T> center_in(const Rect<T>& outer_rect) const
	{
		Rect<T> r = *this;
		r.x = outer_rect.x + (outer_rect.w - r.w) / 2;
		r.y = outer_rect.y + (outer_rect.h - r.h) / 2;
		return r;
	}

	Rect operator+(const Vector2<T>& offset) const
	{
		Rect<T> r = *this;
		r.x += offset.x;
		r.y += offset.y;
		return r;
	}
};

typedef Rect<int32_t> Recti;
typedef Rect<float> Rectf;
