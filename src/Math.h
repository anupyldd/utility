/*
 * Some useful math structures and operations.
 * Structures include a variety of methods, like 
 * calculating average, dot product, magnitude, 
 * and most of arithmetic operations 
 * (excluding those which make no sense)
 *
 * Templated structures:
 * - Vec2 
 * - Vec3
 * 
 * There are also some aliases like position, point, color,
 * etc. already defined in the format <name><dimensions><data type>
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
#include <numbers>

namespace util
{
namespace math
{
/*****************************************************/
//				Constants
/*****************************************************/

	constexpr double PI = 3.1415926535897932;
	constexpr double PI2 = PI * 2;

	constexpr double PI_2 = PI / 2;
	constexpr double PI_3 = PI / 3;
	constexpr double PI_4 = PI / 4;
	constexpr double PI_6 = PI / 6;

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

		double Dot(const Vec2& v) { return x * v.x + y * v.y; }

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

	template<class T>
	struct Vec3
	{
		T x, y, z;

	public:
		Vec3() = default;
		Vec3(T v) : x(x), y(x), z(x) {}
		Vec3(T x, T y, T z) : x(x), y(y), z(z) {}
		Vec3(const Vec3<T>& src) : x(src.x), y(src.y), z(src.z) {}

		T Sum() const { return x + y + z; }
		T Mul() const { return x * y * z; }
		T Avg() const { return (x + y + z) / 3; }

		T Min() const { return std::min({ x,y,z }); }
		T Max() const { return std::max({ x,y,z }); }
		double MagSq() const { return x * x + y * y + z * z; }
		double Mag() const { return std::sqrt(MagSq()); }

		double Dot(const Vec3& v) { return x * v.x + y * v.y + z * v.z; }

		void Zero() { x = y = z = 0; }

		Vec3<double> Normalize() const { double mag = Mag(); return (mag == 0) ? Vec3<double>(x, y, z) : *this / mag; }

		std::string ToStr() const { return std::format("{}, {}, {}", x, y, z); }

	public:
		template<class NT>
		operator Vec3<NT>() const { return Vec3<NT>{(NT)x, (NT)y}; }

		Vec3& operator+=(T v) { x += v; y += v; z += v; return *this; }
		Vec3& operator-=(T v) { x -= v; y -= v; z -= v; return *this; }
		Vec3& operator*=(T v) { x *= v; y *= v; z *= v; return *this; }
		Vec3& operator/=(T v) { x /= v; y /= v; z /= v; return *this; }

		Vec3& operator+=(const Vec3& v) { x += v.x; y += v.y; z += v.z; return *this; }
		Vec3& operator-=(const Vec3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
		Vec3& operator*=(const Vec3& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
		Vec3& operator/=(const Vec3& v) { x /= v.x; y /= v.y; z /= v.z; return *this; }
															  
		bool operator==(const Vec3& v) { return x == v.x && y == v.y && z == v.z; }
		bool operator!=(const Vec3& v) { return x != v.x || y != v.y || z != v.z; }
		bool operator>(const Vec3& v) { return MagSq() > v.MagSq(); }
		bool operator>=(const Vec3& v) { return MagSq() >= v.MagSq(); }
		bool operator<(const Vec3& v) { return MagSq() < v.MagSq(); }
		bool operator<=(const Vec3& v) { return MagSq() <= v.MagSq(); }

		template<class C>
		friend Vec3<C> operator+(const Vec3<T>& lhs, const C& rhs) { return Vec3<C>(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs); }

		template<class C>
		friend Vec3<C> operator-(const Vec3<T>& lhs, const C& rhs) { return Vec3<C>(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs); }

		template<class C>
		friend Vec3<C> operator*(const Vec3<T>& lhs, const C& rhs) { return Vec3<C>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs); }

		template<class C>
		friend Vec3<C> operator/(const Vec3<T>& lhs, const C& rhs) { return Vec3<C>(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs); }

		friend std::ostream& operator<<(std::ostream& os, const Vec3<T>& v)
		{
			os << v.x << ", " << v.y << ", " << v.z;
			return os;
		}
	};

	template<class T>
	struct Vec4
	{
		T x, y, z, w;

	public:
		Vec4() = default;
		Vec4(T v) : x(x), y(x), z(x), w(x) {}
		Vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
		Vec4(const Vec4<T>& src) : x(src.x), y(src.y), z(src.z), w(src.w) {}

		T Sum() const { return x + y + z + w; }
		T Mul() const { return x * y * z * w; }
		T Avg() const { return (x + y + z + w) / 4; }

		T Min() const { return std::min({ x,y,z,w }); }
		T Max() const { return std::max({ x,y,z,w }); }
		double MagSq() const { return x * x + y * y + z * z + w * w; }
		double Mag() const { return std::sqrt(MagSq()); }

		double Dot(const Vec4& v) { return x * v.x + y * v.y + z * v.z + w * v.w; }

		void Zero() { x = y = z = w = 0; }

		Vec4<double> Normalize() const { double mag = Mag(); return (mag == 0) ? Vec4<double>(x, y, z, w) : *this / mag; }

		std::string ToStr() const { return std::format("{}, {}, {}, {}", x, y, z, w); }

	public:
		template<class NT>
		operator Vec4<NT>() const { return Vec4<NT>{(NT)x, (NT)y}; }

		Vec4& operator+=(T v) { x += v; y += v; z += v; w += v; return *this; }
		Vec4& operator-=(T v) { x -= v; y -= v; z -= v; w -= v; return *this; }
		Vec4& operator*=(T v) { x *= v; y *= v; z *= v; w *= v; return *this; }
		Vec4& operator/=(T v) { x /= v; y /= v; z /= v; w /= v; return *this; }

		Vec4& operator+=(const Vec4& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
		Vec4& operator-=(const Vec4& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
		Vec4& operator*=(const Vec4& v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
		Vec4& operator/=(const Vec4& v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }

		bool operator==(const Vec4& v) { return x == v.x && y == v.y && z == v.z && w == v.w; }
		bool operator!=(const Vec4& v) { return x != v.x || y != v.y || z != v.z || w != v.w; }
		bool operator>(const Vec4& v) { return MagSq() > v.MagSq(); }
		bool operator>=(const Vec4& v) { return MagSq() >= v.MagSq(); }
		bool operator<(const Vec4& v) { return MagSq() < v.MagSq(); }
		bool operator<=(const Vec4& v) { return MagSq() <= v.MagSq(); }

		template<class C>
		friend Vec4<C> operator+(const Vec4<T>& lhs, const C& rhs) { return Vec4<C>(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs); }

		template<class C>
		friend Vec4<C> operator-(const Vec4<T>& lhs, const C& rhs) { return Vec4<C>(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs); }

		template<class C>
		friend Vec4<C> operator*(const Vec4<T>& lhs, const C& rhs) { return Vec4<C>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs); }

		template<class C>
		friend Vec4<C> operator/(const Vec4<T>& lhs, const C& rhs) { return Vec4<C>(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs); }

		friend std::ostream& operator<<(std::ostream& os, const Vec4<T>& v)
		{
			os << v.x << ", " << v.y << ", " << v.z << ", " << v.w;
			return os;
		}
	};

/*****************************************************/
//				Aliases
/*****************************************************/

	using Vec2I = Vec2<int>;
	using Vec2F = Vec2<float>;
	using Vec2D = Vec2<double>;

	using Point2I = Vec2<int>;
	using Point2F = Vec2<float>;
	using Point2D = Vec2<double>;

	using Pos2I = Vec2<int>;
	using Pos2F = Vec2<float>;
	using Pos2D = Vec2<double>;


	using Vec3I = Vec3<int>;
	using Vec3F = Vec3<float>;
	using Vec3D = Vec3<double>;

	using Point3I = Vec3<int>;
	using Point3F = Vec3<float>;
	using Point3D = Vec3<double>;

	using Pos3I = Vec3<int>;
	using Pos3F = Vec3<float>;
	using Pos3D = Vec3<double>;


	using Vec4I = Vec4<int>;
	using Vec4F = Vec4<float>;
	using Vec4D = Vec4<double>;

	using Point4I = Vec4<int>;
	using Point4F = Vec4<float>;
	using Point4D = Vec4<double>;

	using Pos4I = Vec4<int>;
	using Pos4F = Vec4<float>;
	using Pos4D = Vec4<double>;


	using Color3B = Vec3<uint8_t>;
	using Color3F = Vec3<float>;

	using Color4B = Vec4<uint8_t>;
	using Color4F = Vec4<float>;

/*****************************************************/
//				Functions
/*****************************************************/
}
}
