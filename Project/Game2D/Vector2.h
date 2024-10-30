#pragma once

class Vec2 final
{
public:
	Vec2() = default;
	Vec2(float value);
	Vec2(const Vec2& rhs);
	Vec2(float X, float Y);
	Vec2(const float* xy);
	
	Vec2& operator=(const Vec2& rhs);

	bool        operator==(const Vec2& rhs) const;
	bool        operator!=(const Vec2& rhs) const;
	Vec2        operator+(const Vec2& rhs) const;
	const Vec2& operator+=(const Vec2& rhs);
	const Vec2& operator-=(const Vec2& rhs);
	Vec2        operator-(const Vec2& rhs) const;
	Vec2        operator*(float rhs) const;
	const Vec2& operator*=(float rhs);
	const Vec2& operator/=(float rhs);
	float       operator[](size_t idx) const;
	float&      operator[](size_t idx);

	const Vec2& Normalize();
	float GetMagnitude() const;

	float Dot(const Vec2& rhs) const { return x * rhs.x + y * rhs.y; }

	const float* ToPtr() const { return &x; }

	float x{0.0f};
	float y{0.0f};
};

inline Vec2::Vec2(float value)
	: x(value)
	, y(value)
{
}

inline Vec2::Vec2(const Vec2& rhs)
	: x(rhs.x)
	, y(rhs.y)
{
}

inline Vec2::Vec2(float X, float Y)
	: x(X)
	, y(Y)
{
}

inline Vec2::Vec2(const float* xy)
	: x(xy[0])
	, y(xy[1])
{
}

inline Vec2& Vec2::operator=(const Vec2& rhs)
{
	x = rhs.x;
	y = rhs.y;
	return *this;
}

inline bool Vec2::operator==(const Vec2& rhs) const
{
	if (x != rhs.x) return false;
	if (y != rhs.y) return false;
	return true;
}

inline bool Vec2::operator!=(const Vec2& rhs) const
{
	if (*this == rhs) return false;
	return true;
}

inline Vec2 Vec2::operator+(const Vec2& rhs) const
{
	Vec2 temp;
	temp.x = x + rhs.x;
	temp.y = y + rhs.y;
	return temp;
}

inline const Vec2& Vec2::operator+=(const Vec2& rhs)
{
	x += rhs.x;
	y += rhs.y;
	return *this;
}

inline const Vec2& Vec2::operator-=(const Vec2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

inline Vec2 Vec2::operator-(const Vec2& rhs) const
{
	Vec2 temp;
	temp.x = x - rhs.x;
	temp.y = y - rhs.y;
	return temp;
}

inline Vec2 Vec2::operator*(float rhs) const
{
	Vec2 temp;
	temp.x = x * rhs;
	temp.y = y * rhs;
	return temp;
}

inline const Vec2& Vec2::operator*=(float rhs)
{
	x *= rhs;
	y *= rhs;
	return *this;
}

inline const Vec2& Vec2::operator/=(float rhs)
{
	x /= rhs;
	y /= rhs;
	return *this;
}

inline float Vec2::operator[](size_t idx) const
{
	assert(idx >= 0 && idx < 2);
	return (&x)[idx];
}

inline float& Vec2::operator[](size_t idx)
{
	assert(idx >= 0 && idx < 2);
	return (&x)[idx];
}

inline const Vec2& Vec2::Normalize()
{
	float mag = GetMagnitude();
	float invMag = 1.0f / mag;
	if (0.0f * invMag == 0.0f * invMag)
	{
		x = x * invMag;
		y = y * invMag;
	}

	return *this;
}

inline float Vec2::GetMagnitude() const
{
	float mag;

	mag = x * x + y * y;
	mag = sqrtf(mag);

	return mag;
}