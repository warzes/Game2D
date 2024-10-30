#pragma once

#include "Vector4.h"

// TODO: delete

class Mat4 final
{
public:
	Mat4() = default;
	Mat4(const Mat4& rhs);
	Mat4(const float* mat);
	Mat4(const Vec4& row0, const Vec4& row1, const Vec4& row2, const Vec4& row3);
	Mat4& operator=(const Mat4& rhs);

	void Zero();
	void Identity();

	float Trace() const;
	float Determinant() const;
	Mat4 Transpose() const;
	Mat4 Inverse() const;
	Mat3 Minor(int i, int j) const;
	float Cofactor(int i, int j) const;

	void Orient(Vec3 pos, Vec3 fwd, Vec3 up);
	void LookAt(Vec3 pos, Vec3 lookAt, Vec3 up);
	void Ortho(float xmin, float xmax, float ymin, float ymax, float znear, float zfar);

	const float* ToPtr() const { return rows[0].ToPtr(); }
	float* ToPtr() { return rows[0].ToPtr(); }

	Vec4 operator*(const Vec4& rhs) const;
	Mat4 operator*(float rhs) const;
	Mat4 operator*(const Mat4& rhs) const;
	const Mat4& operator*=(float rhs);

	Vec4 rows[4];
};

inline Mat4::Mat4(const Mat4& rhs)
{
	rows[0] = rhs.rows[0];
	rows[1] = rhs.rows[1];
	rows[2] = rhs.rows[2];
	rows[3] = rhs.rows[3];
}

inline Mat4::Mat4(const float* mat)
{
	rows[0] = mat + 0;
	rows[1] = mat + 4;
	rows[2] = mat + 8;
	rows[3] = mat + 12;
}

inline Mat4::Mat4(const Vec4& row0, const Vec4& row1, const Vec4& row2, const Vec4& row3)
{
	rows[0] = row0;
	rows[1] = row1;
	rows[2] = row2;
	rows[3] = row3;
}

inline Mat4& Mat4::operator=(const Mat4& rhs)
{
	rows[0] = rhs.rows[0];
	rows[1] = rhs.rows[1];
	rows[2] = rhs.rows[2];
	rows[3] = rhs.rows[3];
	return *this;
}

inline const Mat4& Mat4::operator*=(float rhs)
{
	rows[0] *= rhs;
	rows[1] *= rhs;
	rows[2] *= rhs;
	rows[3] *= rhs;
	return *this;
}

inline void Mat4::Zero()
{
	rows[0] = {0.0f};
	rows[1] = {0.0f};
	rows[2] = {0.0f};
	rows[3] = {0.0f};
}

inline void Mat4::Identity()
{
	rows[0] = Vec4(1, 0, 0, 0);
	rows[1] = Vec4(0, 1, 0, 0);
	rows[2] = Vec4(0, 0, 1, 0);
	rows[3] = Vec4(0, 0, 0, 1);
}

inline float Mat4::Trace() const
{
	const float xx = rows[0][0] * rows[0][0];
	const float yy = rows[1][1] * rows[1][1];
	const float zz = rows[2][2] * rows[2][2];
	const float ww = rows[3][3] * rows[3][3];
	return (xx + yy + zz + ww);
}

inline float Mat4::Determinant() const
{
	float det = 0.0f;
	float sign = 1.0f;
	for (int j = 0; j < 4; j++)
	{
		Mat3 minor = Minor(0, j);

		det += rows[0][j] * minor.Determinant() * sign;
		sign = sign * -1.0f;
	}
	return det;
}

inline Mat4 Mat4::Transpose() const
{
	Mat4 transpose;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			transpose.rows[i][j] = rows[j][i];
		}
	}
	return transpose;
}

inline Mat4 Mat4::Inverse() const
{
	Mat4 inv;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			inv.rows[j][i] = Cofactor(i, j);	// Perform the transpose while calculating the cofactors
		}
	}
	float det = Determinant();
	float invDet = 1.0f / det;
	inv *= invDet;
	return inv;
}

inline Mat3 Mat4::Minor(int i, int j) const
{
	Mat3 minor;

	int yy = 0;
	for (int y = 0; y < 4; y++)
	{
		if (y == j) continue;

		int xx = 0;
		for (int x = 0; x < 4; x++)
		{
			if (x == i) continue;

			minor.rows[xx][yy] = rows[x][y];
			xx++;
		}

		yy++;
	}
	return minor;
}

inline float Mat4::Cofactor(int i, int j) const
{
	const Mat3 minor = Minor(i, j);
	const float C = float(pow(-1, i + 1 + j + 1)) * minor.Determinant();
	return C;
}

inline void Mat4::Orient(Vec3 pos, Vec3 fwd, Vec3 up)
{
	Vec3 left = up.Cross(fwd);

	// For our coordinate system where:
	// +x-axis = fwd
	// +y-axis = left
	// +z-axis = up
	rows[0] = Vec4(fwd.x, left.x, up.x, pos.x);
	rows[1] = Vec4(fwd.y, left.y, up.y, pos.y);
	rows[2] = Vec4(fwd.z, left.z, up.z, pos.z);
	rows[3] = Vec4(0, 0, 0, 1);
}

inline void Mat4::LookAt(Vec3 pos, Vec3 lookAt, Vec3 up)
{
	Vec3 fwd = pos - lookAt;
	fwd.Normalize();

	Vec3 right = up.Cross(fwd);
	right.Normalize();

	up = fwd.Cross(right);
	up.Normalize();

	// For NDC coordinate system where:
	// +x-axis = right
	// +y-axis = up
	// +z-axis = fwd
	rows[0] = Vec4(right.x, right.y, right.z, -pos.Dot(right));
	rows[1] = Vec4(up.x, up.y, up.z, -pos.Dot(up));
	rows[2] = Vec4(fwd.x, fwd.y, fwd.z, -pos.Dot(fwd));
	rows[3] = Vec4(0, 0, 0, 1);
}

inline void Mat4::Ortho(float xmin, float xmax, float ymin, float ymax, float znear, float zfar)
{
	const float width = xmax - xmin;
	const float height = ymax - ymin;
	const float depth = zfar - znear;

	const float tx = -(xmax + xmin) / width;
	const float ty = -(ymax + ymin) / height;
	const float tz = -(zfar + znear) / depth;

	rows[0] = Vec4(2.0f / width, 0, 0, tx);
	rows[1] = Vec4(0, 2.0f / height, 0, ty);
	rows[2] = Vec4(0, 0, -2.0f / depth, tz);
	rows[3] = Vec4(0, 0, 0, 1);
}

inline Vec4 Mat4::operator*(const Vec4& rhs) const
{
	Vec4 tmp;
	tmp[0] = rows[0].Dot(rhs);
	tmp[1] = rows[1].Dot(rhs);
	tmp[2] = rows[2].Dot(rhs);
	tmp[3] = rows[3].Dot(rhs);
	return tmp;
}

inline Mat4 Mat4::operator*(float rhs) const
{
	Mat4 tmp;
	tmp.rows[0] = rows[0] * rhs;
	tmp.rows[1] = rows[1] * rhs;
	tmp.rows[2] = rows[2] * rhs;
	tmp.rows[3] = rows[3] * rhs;
	return tmp;
}

inline Mat4 Mat4::operator*(const Mat4& rhs) const
{
	Mat4 tmp;
	for (int i = 0; i < 4; i++)
	{
		tmp.rows[i].x = rows[i].x * rhs.rows[0].x + rows[i].y * rhs.rows[1].x + rows[i].z * rhs.rows[2].x + rows[i].w * rhs.rows[3].x;
		tmp.rows[i].y = rows[i].x * rhs.rows[0].y + rows[i].y * rhs.rows[1].y + rows[i].z * rhs.rows[2].y + rows[i].w * rhs.rows[3].y;
		tmp.rows[i].z = rows[i].x * rhs.rows[0].z + rows[i].y * rhs.rows[1].z + rows[i].z * rhs.rows[2].z + rows[i].w * rhs.rows[3].z;
		tmp.rows[i].w = rows[i].x * rhs.rows[0].w + rows[i].y * rhs.rows[1].w + rows[i].z * rhs.rows[2].w + rows[i].w * rhs.rows[3].w;
	}
	return tmp;
}