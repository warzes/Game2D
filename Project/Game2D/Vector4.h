#pragma once

// TODO: delete

class Vec4 final
{
public:
	Vec4() = default;
	Vec4(float value);
	Vec4(const Vec4& rhs);
	Vec4(float X, float Y, float Z, float W);
	Vec4(const float* rhs);
	Vec4& operator=(const Vec4& rhs);

	bool        operator==(const Vec4& rhs) const;
	bool        operator!=(const Vec4& rhs) const;
	Vec4        operator+(const Vec4& rhs) const;
	const Vec4& operator+=(const Vec4& rhs);
	const Vec4& operator-=(const Vec4& rhs);
	const Vec4& operator*=(const Vec4& rhs);
	const Vec4& operator/=(const Vec4& rhs);
	Vec4        operator-(const Vec4& rhs) const;
	Vec4        operator*(float rhs) const;
	float       operator[](size_t idx) const;
	float&      operator[](size_t idx);

	float Dot(const Vec4& rhs) const;
	const Vec4& Normalize();
	float GetMagnitude() const;

	const float* ToPtr() const { return &x; }
	float* ToPtr() { return &x; }

	float x{0.0f};
	float y{0.0f};
	float z{0.0f};
	float w{0.0f};
};

inline Vec4::Vec4(float value)
	: x(value)
	, y(value)
	, z(value)
	, w(value)
{
}

inline Vec4::Vec4(const Vec4& rhs)
	: x(rhs.x)
	, y(rhs.y)
	, z(rhs.z)
	, w(rhs.w)
{
}

inline Vec4::Vec4(float X, float Y, float Z, float W)
	: x(X)
	, y(Y)
	, z(Z)
	, w(W)
{
}

inline Vec4::Vec4(const float* rhs)
{
	x = rhs[0];
	y = rhs[1];
	z = rhs[2];
	w = rhs[3];
}

inline Vec4& Vec4::operator=(const Vec4& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;
	return *this;
}

inline bool Vec4::operator==(const Vec4& rhs) const
{
	if (x != rhs.x) return false;
	if (y != rhs.y) return false;
	if (z != rhs.z) return false;
	if (w != rhs.w) return false;

	return true;
}

inline bool Vec4::operator!=(const Vec4& rhs) const
{
	if (*this == rhs) return false;
	return true;
}

inline Vec4 Vec4::operator+(const Vec4& rhs) const
{
	Vec4 temp;
	temp.x = x + rhs.x;
	temp.y = y + rhs.y;
	temp.z = z + rhs.z;
	temp.w = w + rhs.w;
	return temp;
}

inline const Vec4& Vec4::operator+=(const Vec4& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;
	return *this;
}

inline const Vec4& Vec4::operator-=(const Vec4& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w -= rhs.w;
	return *this;
}

inline const Vec4& Vec4::operator*=(const Vec4& rhs)
{
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;
	w *= rhs.w;
	return *this;
}

inline const Vec4& Vec4::operator/=(const Vec4& rhs)
{
	x /= rhs.x;
	y /= rhs.y;
	z /= rhs.z;
	w /= rhs.w;
	return *this;
}

inline Vec4 Vec4::operator-(const Vec4& rhs) const
{
	Vec4 temp;
	temp.x = x - rhs.x;
	temp.y = y - rhs.y;
	temp.z = z - rhs.z;
	temp.w = w - rhs.w;
	return temp;
}

inline Vec4 Vec4::operator*(float rhs) const
{
	Vec4 temp;
	temp.x = x * rhs;
	temp.y = y * rhs;
	temp.z = z * rhs;
	temp.w = w * rhs;
	return temp;
}

inline float Vec4::operator[](size_t idx) const
{
	assert(idx >= 0 && idx < 4);
	return (&x)[idx];
}

inline float& Vec4::operator[](size_t idx)
{
	assert(idx >= 0 && idx < 4);
	return (&x)[idx];
}

inline float Vec4::Dot(const Vec4& rhs) const
{
	float xx = x * rhs.x;
	float yy = y * rhs.y;
	float zz = z * rhs.z;
	float ww = w * rhs.w;
	return (xx + yy + zz + ww);
}

inline const Vec4& Vec4::Normalize()
{
	float mag = GetMagnitude();
	float invMag = 1.0f / mag;
	if (0.0f * invMag == 0.0f * invMag)
	{
		x *= invMag;
		y *= invMag;
		z *= invMag;
		w *= invMag;
	}

	return *this;
}

inline float Vec4::GetMagnitude() const
{
	float mag;
	mag = x * x + y * y + z * z + w * w;
	mag = sqrtf(mag);
	return mag;
}