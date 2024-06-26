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
		
		T Sum() const { return x + y; }
		T Sub() const { return x - y; }
		T Mul() const { return x * y; }
		T Div() const { return x / y; }
		T Avg() const { return (x + y) / 2; }

		T Min() const { return std::min(x, y); }
		T Max() const { return std::max(x, y); }
		double MagSq() const { return x * x + y * y; }
		double Mag() const { return std::sqrt(MagSq()); }

		void Zero() { x = y = 0; }
		
		Vec2<double> Normalize() const { double mag = Mag(); return (mag == 0) ? Vec2<double>(x, y) : *this / mag; }

		std::string ToStr() const { return std::format("{}, {}", x, y); }

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

		bool operator==(const Vec2& v) { return x == v.x && y == v.y; }
		bool operator!=(const Vec2& v) { return x != v.x || y != v.y; }
		bool operator>(const Vec2& v) { return MagSq() > v.MagSq(); }
		bool operator>=(const Vec2& v) { return MagSq() >= v.MagSq(); }
		bool operator<(const Vec2& v) { return MagSq() < v.MagSq(); }
		bool operator<=(const Vec2& v) { return MagSq() <= v.MagSq(); }

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
