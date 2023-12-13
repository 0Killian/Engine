#pragma once

namespace NGN::Math
{
	template <typename T>
	struct Vec4
	{
		T x, y, z, w;

		Vec4() = default;
		Vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
		Vec4(const Vec3<T>& vec3, T w = 1) : x(vec3.x), y(vec3.y), z(vec3.z), w(w) {}

		Vec4<T> operator+(const Vec4<T>& other) const { return Vec4<T>(x + other.x, y + other.y, z + other.z, w + other.w); }
		Vec4<T> operator-(const Vec4<T>& other) const { return Vec4<T>(x - other.x, y - other.y, z - other.z, w - other.w); }
		Vec4<T> operator*(const Vec4<T>& other) const { return Vec4<T>(x * other.x, y * other.y, z * other.z, w * other.w); }
		Vec4<T> operator/(const Vec4<T>& other) const { return Vec4<T>(x / other.x, y / other.y, z / other.z, w / other.w); }

		Vec4<T>& operator+=(const Vec4<T>& other) { x += other.x; y += other.y; z += other.z; w += other.w; return *this; }
		Vec4<T>& operator-=(const Vec4<T>& other) { x -= other.x; y -= other.y; z -= other.z; w -= other.w; return *this; }
		Vec4<T>& operator*=(const Vec4<T>& other) { x *= other.x; y *= other.y; z *= other.z; w *= other.w; return *this; }
		Vec4<T>& operator/=(const Vec4<T>& other) { x /= other.x; y /= other.y; z /= other.z; w /= other.w; return *this; }

		Vec4<T> operator+(T scalar) const { return Vec4<T>(x + scalar, y + scalar, z + scalar, w + scalar); }
		Vec4<T> operator-(T scalar) const { return Vec4<T>(x - scalar, y - scalar, z - scalar, w - scalar); }
		Vec4<T> operator*(T scalar) const { return Vec4<T>(x * scalar, y * scalar, z * scalar, w * scalar); }
		Vec4<T> operator/(T scalar) const { return Vec4<T>(x / scalar, y / scalar, z / scalar, w / scalar); }

		Vec4<T>& operator+=(T scalar) { x += scalar; y += scalar; z += scalar; w += scalar; return *this; }
		Vec4<T>& operator-=(T scalar) { x -= scalar; y -= scalar; z -= scalar; w -= scalar; return *this; }
		Vec4<T>& operator*=(T scalar) { x *= scalar; y *= scalar; z *= scalar; w *= scalar; return *this; }
		Vec4<T>& operator/=(T scalar) { x /= scalar; y /= scalar; z /= scalar; w /= scalar; return *this; }

		Vec4<T> operator-() const { return Vec4<T>(-x, -y, -z, -w); }

		std::strong_ordering operator<=>(const Vec4<T>& other) const
		{
			return DistSquared() <=> other.DistSquared();
		}

		T DistSquared() const { return x * x + y * y + z * z + w * w; }
		T Dist() const { return std::sqrt(DistSquared()); }

		T Dot(const Vec4<T>& other) const { return x * other.x + y * other.y + z * other.z + w * other.w; }	
		Vec4<T> Cross(const Vec4<T>& other) const { return Vec4<T>(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x, 0); }

		Vec4<T> Normalized() const { return *this / Dist(); }
		Vec4<T>& Normalize() { return *this /= Dist(); }

		Vec4<T> Lerp(const Vec4<T>& other, T t) const { return *this + (other - *this) * t; }
		Vec4<T> Slerp(const Vec4<T>& other, T t) const
		{
			T dot = Dot(other);
			T theta = std::acos(dot) * t;
			Vec4<T> relative = (other - *this * dot).Normalized();
			return *this * std::cos(theta) + relative * std::sin(theta);
		}

		Vec4<T> Project(const Vec4<T>& other) const { return other * (Dot(other) / other.DistSquared()); }
		Vec4<T> Reject(const Vec4<T>& other) const { return *this - Project(other); }

		Vec4<T> Reflect(const Vec4<T>& normal) const { return *this - normal * 2 * Dot(normal); }
	};
}