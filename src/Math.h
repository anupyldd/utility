/*
 * Some useful math structures and operations.
 *
 * Templated structures:
 * - Vec2 
 * - Vec3
 * - Vec4
 * 
 * Anything with "ut_" prefix is only for internal 
 * use by other structures and functions.
 */

#pragma once

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <any>
#include <type_traits>
#include <cmath>
#include <string>
#include <format>

namespace util
{
namespace math
{
/*****************************************************/
//				Structures
/*****************************************************/

	template<class T>
	struct Vec2
	{
		T x, y;

	public:
		Vec2() = default;
		Vec2(T v) : x(x), y(x) {}
		Vec2(T x, T y) : x(x), y(y) {}
		Vec2(const Vec2<T>& src) : x(src.x), y(src.y) {}
		
		T Sum() { return x + y; }
		T Sub() { return x - y; }
		T Mul() { return x * y; }
		T Div() { return x / y; }
		T Avg() { return (x + y) / 2; }

		T Min() { return std::min(x, y); }
		T Max() { return std::max(x, y); }
		double LenSq() { return x * x + y * y; }
		double Len() { return std::sqrt(LenSq()); }

		void Zero() { x = y = 0; }
		
		Vec2<double> Normalize() { double len = Len(); return (len == 0) ? Vec2<double>(x, y) : *this / len; }
		//void NormalizeInplace() { double len = Len(); if(len != 0) *this /= len; }

		std::string ToStr() { return std::format("{}, {}", x, y); }

	public:
		template<class NT>
		operator Vec2<NT>() const { return Vec2<NT>{(NT)x, (NT)y}; }

		Vec2& operator+=(T v) { x += v; y += v; return *this; }
		Vec2& operator-=(T v) { x -= v; y -= v; return *this; }
		Vec2& operator*=(T v) { x *= v; y *= v; return *this; }
		Vec2& operator/=(T v) { x /= v; y /= v; return *this; }
		
		Vec2& operator+=(const Vec2& v) { x += v.x; y += v.y; return *this; }
		Vec2& operator-=(const Vec2& v) { x -= v.x; y -= v.y; return *this; }
		Vec2& operator*=(const Vec2& v) { x *= v.x; y *= v.y; return *this; }
		Vec2& operator/=(const Vec2& v) { x /= v.x; y /= v.y; return *this; }

		template<class C>
		friend Vec2<C> operator+(const Vec2<T>& lhs, const C& rhs) { return Vec2<C>(lhs.x + rhs, lhs.y + rhs); }
		
		template<class C>
		friend Vec2<C> operator-(const Vec2<T>& lhs, const C& rhs) { return Vec2<C>(lhs.x - rhs, lhs.y - rhs); }
		
		template<class C>
		friend Vec2<C> operator*(const Vec2<T>& lhs, const C& rhs) { return Vec2<C>(lhs.x * rhs, lhs.y * rhs); }

		template<class C>
		friend Vec2<C> operator/(const Vec2<T>& lhs, const C& rhs) { return Vec2<C>(lhs.x / rhs, lhs.y / rhs); }

		friend std::ostream& operator<<(std::ostream& os, const Vec2<T>& v)
		{
			os << v.x << ", " << v.y;
			return os;
		}
	};

}
}