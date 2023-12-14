#pragma once

#include <type_traits>
#include "Math/Vec3.h"
#include "Math/Vec4.h"
#include <numbers>

namespace NGN::Math
{
	enum MatOrder
	{
		ROW_MAJOR,
		COLUMN_MAJOR
	};

	template <typename T, MatOrder order>
	requires std::is_arithmetic_v<T>
	class Mat4
	{};

	template <typename T>
	class Mat4<T, COLUMN_MAJOR>
	{
	public:
		static constexpr Mat4<T, COLUMN_MAJOR> Identity()
		{
			Mat4<T, COLUMN_MAJOR> mat;

			for (int i = 0; i < 4; i++)
				mat[i][i] = 1;

			return mat;
		}

		static constexpr Mat4<T, COLUMN_MAJOR> Zero()
		{
			return Mat4<T, COLUMN_MAJOR>();
		}

		static constexpr Mat4<T, COLUMN_MAJOR> Translation(const Vec3<T>& translation)
		{
			Mat4<T, COLUMN_MAJOR> mat = Identity();

			mat[3][0] = translation.x;
			mat[3][1] = translation.y;
			mat[3][2] = translation.z;

			return mat;
		}

		static constexpr Mat4<T, COLUMN_MAJOR> Rotation(const Vec3<T>& rotation)
		{
			Mat4<T, COLUMN_MAJOR> mat = Identity();

			constexpr T toRadians = std::numbers::pi_v<T> / 180;

			T cosX = cos(rotation.x * toRadians);
			T sinX = sin(rotation.x * toRadians);
			T cosY = cos(rotation.y * toRadians);
			T sinY = sin(rotation.y * toRadians);
			T cosZ = cos(rotation.z * toRadians);
			T sinZ = sin(rotation.z * toRadians);

			mat[0][0] = cosY * cosZ;
			mat[0][1] = cosY * sinZ;
			mat[0][2] = -sinY;

			mat[1][0] = sinX * sinY * cosZ - cosX * sinZ;
			mat[1][1] = sinX * sinY * sinZ + cosX * cosZ;
			mat[1][2] = sinX * cosY;

			mat[2][0] = cosX * sinY * cosZ + sinX * sinZ;
			mat[2][1] = cosX * sinY * sinZ - sinX * cosZ;
			mat[2][2] = cosX * cosY;

			return mat;
		}

		static constexpr Mat4<T, COLUMN_MAJOR> RotationRadians(const Vec3<T>& rotation)
		{
			Mat4<T, COLUMN_MAJOR> mat = Identity();

			T cosX = cos(rotation.x);
			T sinX = sin(rotation.x);
			T cosY = cos(rotation.y);
			T sinY = sin(rotation.y);
			T cosZ = cos(rotation.z);
			T sinZ = sin(rotation.z);

			mat[0][0] = cosY * cosZ;
			mat[0][1] = cosY * sinZ;
			mat[0][2] = -sinY;

			mat[1][0] = sinX * sinY * cosZ - cosX * sinZ;
			mat[1][1] = sinX * sinY * sinZ + cosX * cosZ;
			mat[1][2] = sinX * cosY;

			mat[2][0] = cosX * sinY * cosZ + sinX * sinZ;
			mat[2][1] = cosX * sinY * sinZ - sinX * cosZ;
			mat[2][2] = cosX * cosY;

			return mat;
		}

		static constexpr Mat4<T, COLUMN_MAJOR> RotationX(T angle)
		{
			Mat4<T, COLUMN_MAJOR> mat = Identity();

			constexpr T toRadians = std::numbers::pi_v<T> / 180;

			T cosA = cos(angle * toRadians);
			T sinA = sin(angle * toRadians);

			mat[1][1] = cosA;
			mat[1][2] = sinA;
			mat[2][1] = -sinA;
			mat[2][2] = cosA;

			return mat;
		}

		static constexpr Mat4<T, COLUMN_MAJOR> RotationY(T angle)
		{
			Mat4<T, COLUMN_MAJOR> mat = Identity();

			constexpr T toRadians = std::numbers::pi_v<T> / 180;

			T cosA = cos(angle * toRadians);
			T sinA = sin(angle * toRadians);

			mat[0][0] = cosA;
			mat[0][2] = -sinA;
			mat[2][0] = sinA;
			mat[2][2] = cosA;

			return mat;
		}

		static constexpr Mat4<T, COLUMN_MAJOR> RotationZ(T angle)
		{
			Mat4<T, COLUMN_MAJOR> mat = Identity();

			constexpr T toRadians = std::numbers::pi_v<T> / 180;

			T cosA = cos(angle * toRadians);
			T sinA = sin(angle * toRadians);

			mat[0][0] = cosA;
			mat[0][1] = sinA;
			mat[1][0] = -sinA;
			mat[1][1] = cosA;

			return mat;
		}

		static constexpr Mat4<T, COLUMN_MAJOR> RotationXRadians(T angle)
		{
			Mat4<T, COLUMN_MAJOR> mat = Identity();

			T cosA = cos(angle);
			T sinA = sin(angle);

			mat[1][1] = cosA;
			mat[1][2] = sinA;
			mat[2][1] = -sinA;
			mat[2][2] = cosA;

			return mat;
		}

		static constexpr Mat4<T, COLUMN_MAJOR> RotationYRadians(T angle)
		{
			Mat4<T, COLUMN_MAJOR> mat = Identity();

			T cosA = cos(angle);
			T sinA = sin(angle);

			mat[0][0] = cosA;
			mat[0][2] = -sinA;
			mat[2][0] = sinA;
			mat[2][2] = cosA;

			return mat;
		}

		static constexpr Mat4<T, COLUMN_MAJOR> RotationZRadians(T angle)
		{
			Mat4<T, COLUMN_MAJOR> mat = Identity();

			T cosA = cos(angle);
			T sinA = sin(angle);

			mat[0][0] = cosA;
			mat[0][1] = sinA;
			mat[1][0] = -sinA;
			mat[1][1] = cosA;

			return mat;
		}

		static constexpr Mat4<T, COLUMN_MAJOR> Scale(const Vec3<T>& scale)
		{
			Mat4<T, COLUMN_MAJOR> mat = Identity();

			mat[0][0] = scale.x;
			mat[1][1] = scale.y;
			mat[2][2] = scale.z;

			return mat;
		}

		static constexpr Mat4<T, COLUMN_MAJOR> Orthographic(T left, T right, T bottom, T top, T near, T far)
		{
			Mat4<T, COLUMN_MAJOR> mat = Identity();

			mat[0][0] = 2 / (right - left);
			mat[1][1] = 2 / (top - bottom);
			mat[2][2] = -2 / (far - near);

			mat[3][0] = -(right + left) / (right - left);
			mat[3][1] = -(top + bottom) / (top - bottom);
			mat[3][2] = -(far + near) / (far - near);

			return mat;
		}

		static constexpr Mat4<T, COLUMN_MAJOR> Perspective(T fov, T aspectRatio, T near, T far)
		{
			Mat4<T, COLUMN_MAJOR> mat = Identity();

			T tanHalfFov = tan(fov / 2);

			mat[0][0] = 1 / (aspectRatio * tanHalfFov);
			mat[1][1] = 1 / tanHalfFov;
			mat[2][2] = -(far + near) / (far - near);
			mat[2][3] = -1;
			mat[3][2] = -(2 * far * near) / (far - near);
			mat[3][3] = 0;

			return mat;
		}

		static constexpr Mat4<T, COLUMN_MAJOR> LookAt(const Vec3<T>& eye, const Vec3<T>& target, const Vec3<T>& up)
		{
			Mat4<T, COLUMN_MAJOR> mat = Identity();

			Vec3<T> zAxis = (eye - target).Normalize();
			Vec3<T> xAxis = up.Cross(zAxis).Normalize();
			Vec3<T> yAxis = zAxis.Cross(xAxis);

			mat[0][0] = xAxis.x;
			mat[0][1] = yAxis.x;
			mat[0][2] = zAxis.x;

			mat[1][0] = xAxis.y;
			mat[1][1] = yAxis.y;
			mat[1][2] = zAxis.y;

			mat[2][0] = xAxis.z;
			mat[2][1] = yAxis.z;
			mat[2][2] = zAxis.z;

			mat[3][0] = -xAxis.Dot(eye);
			mat[3][1] = -yAxis.Dot(eye);
			mat[3][2] = -zAxis.Dot(eye);

			return mat;
		}

		Mat4<T, ROW_MAJOR> Transpose()
		{
			Mat4<T, ROW_MAJOR> result = Mat4<T, ROW_MAJOR>::Zero();

			for (int row = 0; row < 4; row++)
				for (int col = 0; col < 4; col++)
					result[row][col] = m_Data[col * 4 + row];

			return result;
		}

		const T* GetData() const { return m_Data; }
		T* GetData() { return m_Data; }

		const T* operator[](int index) const { return &m_Data[index * 4]; }
		T* operator[](int index) { return &m_Data[index * 4]; }

		Mat4<T, COLUMN_MAJOR> operator*(const Mat4<T, COLUMN_MAJOR>& other)
		{
			Mat4<T, COLUMN_MAJOR> result = Mat4<T, COLUMN_MAJOR>::Zero();

			for (int row = 0; row < 4; row++)
				for (int col = 0; col < 4; col++)
					for (int i = 0; i < 4; i++)
						result[row][col] += m_Data[row * 4 + i] * other[i][col];

			return result;
		}

		Mat4<T, COLUMN_MAJOR>& operator*=(const Mat4<T, COLUMN_MAJOR>& other)
		{
			Mat4<T, COLUMN_MAJOR> result = Mat4<T, COLUMN_MAJOR>::Zero();

			for (int row = 0; row < 4; row++)
				for (int col = 0; col < 4; col++)
					for (int i = 0; i < 4; i++)
						result[row][col] += m_Data[row * 4 + i] * other[i][col];

			*this = result;

			return *this;
		}

		Vec4<T> operator*(const Vec4<T>& other)
		{
			Vec4<T> result;

			for (int row = 0; row < 4; row++)
				for (int col = 0; col < 4; col++)
					result[row] += m_Data[row + col * 4] * other[col];

			return result;
		}

		Mat4<T, COLUMN_MAJOR>& operator=(const Mat4<T, COLUMN_MAJOR>& other)
		{
			for (int i = 0; i < 16; i++)
				m_Data[i] = other.m_Data[i];

			return *this;
		}

	private:
		Mat4() = default;

		T m_Data[16] = { 0 };
	};

	template <typename T>
	class Mat4<T, ROW_MAJOR>
	{
	public:
		static constexpr Mat4<T, ROW_MAJOR> Identity()
		{
			Mat4<T, ROW_MAJOR> mat;

			for (int i = 0; i < 4; i++)
				mat[i][i] = 1;

			return mat;
		}

		static constexpr Mat4<T, ROW_MAJOR> Zero()
		{
			return Mat4<T, ROW_MAJOR>();
		}

		static constexpr Mat4<T, ROW_MAJOR> Translation(const Vec3<T>& translation)
		{
			Mat4<T, ROW_MAJOR> mat = Identity();

			mat[0][3] = translation.x;
			mat[1][3] = translation.y;
			mat[2][3] = translation.z;

			return mat;
		}

		static constexpr Mat4<T, ROW_MAJOR> Rotation(const Vec3<T>& rotation)
		{
			Mat4<T, ROW_MAJOR> mat = Identity();

			constexpr T toRadians = std::numbers::pi_v<T> / 180;

			T cosX = cos(rotation.x * toRadians);
			T sinX = sin(rotation.x * toRadians);
			T cosY = cos(rotation.y * toRadians);
			T sinY = sin(rotation.y * toRadians);
			T cosZ = cos(rotation.z * toRadians);
			T sinZ = sin(rotation.z * toRadians);

			mat[0][0] = cosY * cosZ;
			mat[0][1] = -cosY * sinZ;
			mat[0][2] = sinY;

			mat[1][0] = sinX * sinY * cosZ + cosX * sinZ;
			mat[1][1] = cosX * cosZ - sinX * sinY * sinZ;
			mat[1][2] = sinX * -cosY;

			mat[2][0] = -cosX * sinY * cosZ + sinX * sinZ;
			mat[2][1] = cosX * sinY * sinZ + sinX * cosZ;
			mat[2][2] = cosX * cosY;

			return mat;
		}

		static constexpr Mat4<T, ROW_MAJOR> RotationX(T angle)
		{
			Mat4<T, ROW_MAJOR> mat = Identity();

			constexpr T toRadians = std::numbers::pi_v<T> / 180;

			T cosA = cos(angle * toRadians);
			T sinA = sin(angle * toRadians);

			mat[1][1] = cosA;
			mat[2][1] = sinA;
			mat[1][2] = -sinA;
			mat[2][2] = cosA;

			return mat;
		}

		static constexpr Mat4<T, ROW_MAJOR> RotationY(T angle)
		{
			Mat4<T, ROW_MAJOR> mat = Identity();

			constexpr T toRadians = std::numbers::pi_v<T> / 180;

			T cosA = cos(angle * toRadians);
			T sinA = sin(angle * toRadians);

			mat[0][0] = cosA;
			mat[2][0] = -sinA;
			mat[0][2] = sinA;
			mat[2][2] = cosA;

			return mat;
		}
		
		static constexpr Mat4<T, ROW_MAJOR> RotationZ(T angle)
		{
			Mat4<T, ROW_MAJOR> mat = Identity();

			constexpr T toRadians = std::numbers::pi_v<T> / 180;

			T cosA = cos(angle * toRadians);
			T sinA = sin(angle * toRadians);

			mat[0][0] = cosA;
			mat[1][0] = sinA;
			mat[0][1] = -sinA;
			mat[1][1] = cosA;

			return mat;
		}

		static constexpr Mat4<T, ROW_MAJOR> RotationRadians(const Vec3<T>& rotation)
		{
			Mat4<T, ROW_MAJOR> mat = Identity();

			T cosX = cos(rotation.x);
			T sinX = sin(rotation.x);
			T cosY = cos(rotation.y);
			T sinY = sin(rotation.y);
			T cosZ = cos(rotation.z);
			T sinZ = sin(rotation.z);

			mat[0][0] = cosY * cosZ;
			mat[0][1] = -cosY * sinZ;
			mat[0][2] = sinY;

			mat[1][0] = sinX * sinY * cosZ + cosX * sinZ;
			mat[1][1] = cosX * cosZ - sinX * sinY * sinZ;
			mat[1][2] = sinX * -cosY;

			mat[2][0] = -cosX * sinY * cosZ + sinX * sinZ;
			mat[2][1] = cosX * sinY * sinZ + sinX * cosZ;
			mat[2][2] = cosX * cosY;

			return mat;
		}

		static constexpr Mat4<T, ROW_MAJOR> RotationXRadians(T angle)
		{
			Mat4<T, ROW_MAJOR> mat = Identity();

			T cosA = cos(angle);
			T sinA = sin(angle);

			mat[1][1] = cosA;
			mat[2][1] = sinA;
			mat[1][2] = -sinA;
			mat[2][2] = cosA;

			return mat;
		}

		static constexpr Mat4<T, ROW_MAJOR> RotationYRadians(T angle)
		{
			Mat4<T, ROW_MAJOR> mat = Identity();

			T cosA = cos(angle);
			T sinA = sin(angle);

			mat[0][0] = cosA;
			mat[2][0] = -sinA;
			mat[0][2] = sinA;
			mat[2][2] = cosA;

			return mat;
		}

		static constexpr Mat4<T, ROW_MAJOR> RotationZRadians(T angle)
		{
			Mat4<T, ROW_MAJOR> mat = Identity();

			T cosA = cos(angle);
			T sinA = sin(angle);

			mat[0][0] = cosA;
			mat[1][0] = sinA;
			mat[0][1] = -sinA;
			mat[1][1] = cosA;

			return mat;
		}

		static constexpr Mat4<T, ROW_MAJOR> Scale(const Vec3<T>& scale)
		{
			Mat4<T, ROW_MAJOR> mat = Identity();

			mat[0][0] = scale.x;
			mat[1][1] = scale.y;
			mat[2][2] = scale.z;

			return mat;
		}

		static constexpr Mat4<T, ROW_MAJOR> Orthographic(T left, T right, T bottom, T top, T near, T far)
		{
			Mat4<T, ROW_MAJOR> mat = Identity();

			mat[0][0] = 2 / (right - left);
			mat[1][1] = 2 / (top - bottom);
			mat[2][2] = -2 / (far - near);

			mat[0][3] = -(right + left) / (right - left);
			mat[1][3] = -(top + bottom) / (top - bottom);
			mat[2][3] = -(far + near) / (far - near);

			return mat;
		}

		static constexpr Mat4<T, ROW_MAJOR> Perspective(T fov, T aspectRatio, T near, T far)
		{
			Mat4<T, ROW_MAJOR> mat = Identity();

			T tanHalfFov = tan(fov / 2);

			mat[0][0] = 1 / (aspectRatio * tanHalfFov);
			mat[1][1] = 1 / tanHalfFov;
			mat[2][2] = -(far + near) / (far - near);
			mat[3][2] = -1;
			mat[2][3] = -(2 * far * near) / (far - near);
			mat[3][3] = 0;

			return mat;
		}

		static constexpr Mat4<T, ROW_MAJOR> LookAt(const Vec3<T>& eye, const Vec3<T>& target, const Vec3<T>& up)
		{
			Mat4<T, ROW_MAJOR> mat = Identity();

			Vec3<T> zAxis = (eye - target).Normalize();
			Vec3<T> xAxis = up.Cross(zAxis).Normalize();
			Vec3<T> yAxis = zAxis.Cross(xAxis);

			mat[0][0] = xAxis.x;
			mat[0][1] = yAxis.x;
			mat[0][2] = zAxis.x;

			mat[1][0] = xAxis.y;
			mat[1][1] = yAxis.y;
			mat[1][2] = zAxis.y;

			mat[2][0] = xAxis.z;
			mat[2][1] = yAxis.z;
			mat[2][2] = zAxis.z;

			mat[0][3] = -xAxis.Dot(eye);
			mat[1][3] = -yAxis.Dot(eye);
			mat[2][3] = -zAxis.Dot(eye);

			return mat;
		}

		Mat4<T, COLUMN_MAJOR> Transpose()
		{
			Mat4<T, COLUMN_MAJOR> result = Mat4<T, COLUMN_MAJOR>::Zero();

			for (int row = 0; row < 4; row++)
				for (int col = 0; col < 4; col++)
					result[row][col] = m_Data[col * 4 + row];

			return result;
		}

		const T* GetData() const { return m_Data; }
		T* GetData() { return m_Data; }

		const T* operator[](int index) const { return &m_Data[index * 4]; }
		T* operator[](int index) { return &m_Data[index * 4]; }

		Mat4<T, ROW_MAJOR> operator*(const Mat4<T, ROW_MAJOR>& other)
		{
			Mat4<T, ROW_MAJOR> result = Mat4<T, ROW_MAJOR>::Zero();

			for (int row = 0; row < 4; row++)
				for (int col = 0; col < 4; col++)
					for (int i = 0; i < 4; i++)
						result[row][col] += m_Data[row * 4 + i] * other[i][col];

			return result;
		}

		Mat4<T, ROW_MAJOR>& operator*=(const Mat4<T, ROW_MAJOR>& other)
		{
			Mat4<T, ROW_MAJOR> result = Mat4<T, ROW_MAJOR>::Zero();

			for (int row = 0; row < 4; row++)
				for (int col = 0; col < 4; col++)
					for (int i = 0; i < 4; i++)
						result[row][col] += m_Data[row * 4 + i] * other[i][col];

			*this = result;

			return *this;
		}

		Mat4<T, ROW_MAJOR>& operator=(const Mat4<T, ROW_MAJOR>& other)
		{
			for (int i = 0; i < 16; i++)
				m_Data[i] = other.m_Data[i];

			return *this;
		}

	private:
		Mat4() = default;

		T m_Data[16] = { 0 };
	};

	template <typename T>
	Vec4<T> operator*(const Mat4<T, ROW_MAJOR> mat, const Vec4<T>& other)
	{
		Vec4<T> result;

		for (int row = 0; row < 4; row++)
			for (int col = 0; col < 4; col++)
				result[row] += mat[row + col * 4] * other[col];

		return result;
	}
}