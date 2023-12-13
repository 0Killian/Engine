#pragma once

namespace NGN::Math
{
	template <typename T>
	struct Vec3
	{
		T x, y, z;

		Vec3() = default;
		Vec3(T x, T y, T z) : x(x), y(y), z(z) {}

		Vec3<T> operator+(const Vec3<T>& other) const { return Vec3<T>(x + other.x, y + other.y, z + other.z); }
		Vec3<T> operator-(const Vec3<T>& other) const { return Vec3<T>(x - other.x, y - other.y, z - other.z); }
		Vec3<T> operator*(const Vec3<T>& other) const { return Vec3<T>(x * other.x, y * other.y, z * other.z); }
		Vec3<T> operator/(const Vec3<T>& other) const { return Vec3<T>(x / other.x, y / other.y, z / other.z); }

		Vec3<T>& operator+=(const Vec3<T>& other) { x += other.x; y += other.y; z += other.z; return *this; }
		Vec3<T>& operator-=(const Vec3<T>& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
		Vec3<T>& operator*=(const Vec3<T>& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }
		Vec3<T>& operator/=(const Vec3<T>& other) { x /= other.x; y /= other.y; z /= other.z; return *this; }

		Vec3<T> operator+(T scalar) const { return Vec3<T>(x + scalar, y + scalar, z + scalar); }
		Vec3<T> operator-(T scalar) const { return Vec3<T>(x - scalar, y - scalar, z - scalar); }
		Vec3<T> operator*(T scalar) const { return Vec3<T>(x * scalar, y * scalar, z * scalar); }
		Vec3<T> operator/(T scalar) const { return Vec3<T>(x / scalar, y / scalar, z / scalar); }

		Vec3<T>& operator+=(T scalar) { x += scalar; y += scalar; z += scalar; return *this; }
		Vec3<T>& operator-=(T scalar) { x -= scalar; y -= scalar; z -= scalar; return *this; }
		Vec3<T>& operator*=(T scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
		Vec3<T>& operator/=(T scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }

		Vec3<T> operator-() const { return Vec3<T>(-x, -y, -z); }

		std::strong_ordering operator<=>(const Vec3<T>& other) const
		{
			return DistSquared() <=> other.DistSquared();
		}

		T DistSquared() const { return x * x + y * y + z * z; }
		T Dist() const { return std::sqrt(DistSquared()); }

		T Dot(const Vec3<T>& other) const { return x * other.x + y * other.y + z * other.z; }
		Vec3<T> Cross(const Vec3<T>& other) const { return Vec3<T>(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x); }

		Vec3<T> Normalized() const { return *this / Dist(); }
		Vec3<T>& Normalize() { return *this /= Dist(); }

		Vec3<T> Lerp(const Vec3<T>& other, T t) const { return *this + (other - *this) * t; }
		Vec3<T> Slerp(const Vec3<T>& other, T t) const
		{
			T dot = Dot(other);
			T theta = std::acos(dot) * t;
			Vec3<T> relative = (other - *this * dot).Normalized();
			return *this * std::cos(theta) + relative * std::sin(theta);
		}

		Vec3<T> Project(const Vec3<T>& other) const { return other * (Dot(other) / other.DistSquared()); }
		Vec3<T> Reject(const Vec3<T>& other) const { return *this - Project(other); }

		Vec3<T> Reflect(const Vec3<T>& normal) const { return *this - normal * 2 * Dot(normal); }
	};
}