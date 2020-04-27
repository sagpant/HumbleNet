//
//  Math.h
//  SimpleGameFramework
//
//  Created by Edward Rudd on 4/6/15.
//
//

#pragma once

namespace Math {
	const double PI = 3.14159265358979323846264338327950288;

	template<typename T>
	T to_radians(T deg_angle)
	{
		return deg_angle * PI / 180.0f;
	}

	template<typename T>
	T to_degrees(T rad_angle)
	{
		return rad_angle * 180.0f / PI;
	}

	template<typename T>
	T clamp(T val, T min, T max)
	{
		if (val < min) return min;
		if (val > max) return max;
		return val;
	}

	template<typename T>
	T abs(T val)
	{
		if (val < T()) return -val;
		return val;
	}
}