#pragma once

class Vec3 final
{
public:
	Vec3() = default;
	Vec3(float value);
	Vec3(const Vec3& rhs);
	Vec3(float X, float Y, float Z);
	Vec3(const float* xyz);
	Vec3& operator=(const Vec3& rhs);
	Vec3& operator=(const float* rhs);

	bool        operator==(const Vec3& rhs) const;
	bool        operator!=(const Vec3& rhs) const;
	Vec3        operator+(const Vec3& rhs) const;
	const Vec3& operator+=(const Vec3& rhs);
	const Vec3& operator-=(const Vec3& rhs);
	Vec3        operator-(const Vec3& rhs) const;
	Vec3        operator*(float rhs) const;
	Vec3        operator/(float rhs) const;
	const Vec3& operator*=(float rhs);
	const Vec3& operator/=(float rhs);
	float       operator[](size_t idx) const;
	float&      operator[](size_t idx);

	Vec3 Cross(const Vec3& rhs) const;
	float Dot(const Vec3& rhs) const;

	const Vec3& Normalize();
	float GetMagnitude() const;
	float GetLengthSqr() const { return Dot(*this); }

	const float* ToPtr() const { return &x; }

	float x{0.0f};
	float y{0.0f};
	float z{0.0f};
};

inline Vec3::Vec3(float value)
	: x(value)
	, y(value)
	, z(value)
{
}

inline Vec3::Vec3(const Vec3& rhs) 
	: x(rhs.x)
	, y(rhs.y)
	, z(rhs.z)
{
}

inline Vec3::Vec3(float X, float Y, float Z)
	: x(X)
	, y(Y)
	, z(Z)
{
}

inline Vec3::Vec3(const float* xyz)
	: x(xyz[0])
	, y(xyz[1])
	, z(xyz[2])
{
}

inline Vec3& Vec3::operator=(const Vec3& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	return *this;
}

inline Vec3& Vec3::operator=(const float* rhs)
{
	x = rhs[0];
	y = rhs[1];
	z = rhs[2];
	return *this;
}

inline bool Vec3::operator==(const Vec3& rhs) const
{
	if (x != rhs.x) return false;
	if (y != rhs.y) return false;
	if (z != rhs.z) return false;
	return true;
}

inline bool Vec3::operator!=(const Vec3& rhs) const
{
	if (*this == rhs) return false;
	return true;
}

inline Vec3 Vec3::operator+(const Vec3& rhs) const
{
	Vec3 temp;
	temp.x = x + rhs.x;
	temp.y = y + rhs.y;
	temp.z = z + rhs.z;
	return temp;
}

inline const Vec3& Vec3::operator+=(const Vec3& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

inline const Vec3& Vec3::operator-=(const Vec3& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

inline Vec3 Vec3::operator-(const Vec3& rhs) const
{
	Vec3 temp;
	temp.x = x - rhs.x;
	temp.y = y - rhs.y;
	temp.z = z - rhs.z;
	return temp;
}

inline Vec3 Vec3::operator*(float rhs) const
{
	Vec3 temp;
	temp.x = x * rhs;
	temp.y = y * rhs;
	temp.z = z * rhs;
	return temp;
}

inline Vec3 Vec3::operator/(float rhs) const
{
	Vec3 temp;
	temp.x = x / rhs;
	temp.y = y / rhs;
	temp.z = z / rhs;
	return temp;
}

inline const Vec3& Vec3::operator*=(float rhs)
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	return *this;
}

inline const Vec3& Vec3::operator/=(float rhs)
{
	x /= rhs;
	y /= rhs;
	z /= rhs;
	return *this;
}

inline float Vec3::operator[](size_t idx) const
{
	assert(idx >= 0 && idx < 3);
	return (&x)[idx];
}

inline float& Vec3::operator[](size_t idx)
{
	assert(idx >= 0 && idx < 3);
	return (&x)[idx];
}

inline Vec3 Vec3::Cross(const Vec3& rhs) const
{
	// This cross product is A x B, where this is A and rhs is B
	Vec3 temp;
	temp.x = (y * rhs.z) - (rhs.y * z);
	temp.y = (rhs.x * z) - (x * rhs.z);
	temp.z = (x * rhs.y) - (rhs.x * y);
	return temp;
}

inline float Vec3::Dot(const Vec3& rhs) const
{
	float temp = (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
	return temp;
}

inline const Vec3& Vec3::Normalize()
{
	float mag = GetMagnitude();
	float invMag = 1.0f / mag;
	if (0.0f * invMag == 0.0f * invMag)
	{
		x *= invMag;
		y *= invMag;
		z *= invMag;
	}
	return *this;
}

inline float Vec3::GetMagnitude() const
{
	float mag;

	mag = x * x + y * y + z * z;
	mag = sqrtf(mag);

	return mag;
}