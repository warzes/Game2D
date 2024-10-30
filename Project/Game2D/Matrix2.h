#pragma once

#include "Vector2.h"

class Mat2 final
{
public:
	Mat2() = default;
	Mat2(const Mat2& rhs);
	Mat2(const float* mat);
	Mat2(const Vec2& row0, const Vec2& row1);
	Mat2& operator=(const Mat2& rhs);

	const Mat2& operator*=(float rhs);
	const Mat2& operator+=(const Mat2& rhs);

	float Determinant() const { return rows[0].x * rows[1].y - rows[0].y * rows[1].x; }

	Vec2 rows[2];
};

inline Mat2::Mat2(const Mat2& rhs)
{
	rows[0] = rhs.rows[0];
	rows[1] = rhs.rows[1];
}

inline Mat2::Mat2(const float* mat)
{
	rows[0] = mat + 0;
	rows[1] = mat + 2;
}

inline Mat2::Mat2(const Vec2& row0, const Vec2& row1)
{
	rows[0] = row0;
	rows[1] = row1;
}

inline Mat2& Mat2::operator=(const Mat2& rhs)
{
	rows[0] = rhs.rows[0];
	rows[1] = rhs.rows[1];
	return *this;
}

inline const Mat2& Mat2::operator*=(float rhs)
{
	rows[0] *= rhs;
	rows[1] *= rhs;
	return *this;
}

inline const Mat2& Mat2::operator+=(const Mat2& rhs)
{
	rows[0] += rhs.rows[0];
	rows[1] += rhs.rows[1];
	return *this;
}