#include "geometry.h"
#include "cmath"

Vec3f Vec3f::operator+(const Vec3f& a) 
{
	return Vec3f(x + a.x, y + a.y,z+a.z);
}

Vec3f Vec3f::operator-(const Vec3f& a) 
{
	return Vec3f(x - a.x, y - a.y, z - a.z);
}

float& Vec3f::operator[](int i) 
{
	if (i == 0) 
	{
		return x;
	}
	else if (i == 1) 
	{
		return y;
	}
	else 
	{
		return z;
	}
}

float Vec3f::dotProduct(const Vec3f& v1, const Vec3f& v2) 
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}


bool isInside(Vec3f a, Vec3f b, Vec3f c, Vec3f p) 
{
	Vec3f ab = a - b;
	Vec3f cb = c - b;
	Vec3f N = Vec3f::crossProduct2d(ab, cb);

	Vec3f pb = p - b;
	Vec3f T = Vec3f::crossProduct2d(ab, pb);
	if (Vec3f::dotProduct(T, N) < 0) return false;

	T = Vec3f::crossProduct2d(pb, cb);
	if (Vec3f::dotProduct(T, N) < 0) return false;

	Vec3f pc = p - c;
	Vec3f ac = a - c;
	T = Vec3f::crossProduct2d(pc, ac);
	if (Vec3f::dotProduct(T, N) < 0) return false;

	return true;

}


float Vec3f::operator*(const Vec3f& a) {
	return x * a.x + y * a.y + z * a.z;
}


float& Vec4f::operator [](int index) {
	if (index == 0) 
	{
		return x;
	}
	else if (index == 1) 
	{
		return y;
	}
	else if (index == 2) 
	{
		return z;
	}
	else 
	{
		return w;
	}
}

const float& Vec4f::operator [](int index) const 
{
	if (index == 0) {
		return x;
	}
	else if (index == 1) {
		return y;
	}
	else if (index == 2) {
		return z;
	}
	else {
		return w;
	}
}

std::vector<float>& Matrix4f::operator [](int index) 
{
	return matrix[index];
}

Matrix4f::Matrix4f() 
{
	matrix.push_back({ 1,0,0,0 });
	matrix.push_back({ 0,1,0,0 });
	matrix.push_back({ 0,0,1,0 });
	matrix.push_back({ 0,0,0,1 });
}

Vec4f Matrix4f::operator *(const Vec4f v) 
{
	return Vec4f
	(
		matrix[0][0] * v[0] + matrix[0][1] * v[1] + matrix[0][2] * v[2] + matrix[0][3] * v[3],
		matrix[1][0] * v[0] + matrix[1][1] * v[1] + matrix[1][2] * v[2] + matrix[1][3] * v[3],
		matrix[2][0] * v[0] + matrix[2][1] * v[1] + matrix[2][2] * v[2] + matrix[2][3] * v[3],
		matrix[3][0] * v[0] + matrix[3][1] * v[1] + matrix[3][2] * v[2] + matrix[3][3] * v[3]
	);
}


Matrix4f Matrix4f::operator * (Matrix4f matrixc) 
{
	Matrix4f ret;
	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 4; j++) 
		{
			ret[i][j] = matrix[i][0] * matrixc[0][j] + matrix[i][1] * matrixc[1][j] + matrix[i][2] * matrixc[2][j] + matrix[i][3] * matrixc[3][j];
		}
	}
	return ret;
}

Matrix4f Matrix4f::operator + (Matrix4f matrixc) 
{
	Matrix4f ret;
	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 4; j++) 
		{
			ret[i][j] = matrix[i][j]+matrixc[i][j];
		}
	}
	return ret;
}


Matrix4f Matrix4f::inverse() 
{
	Matrix4f ret;
	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 4; j++) 
		{
			ret[i][j] = matrix[j][i];
		}
	}
	return ret;
}

Vec4f Vec4f::crossProduct(const Vec4f& v1, const Vec4f& v2)
{
	return Vec4f(v1.y * v2.z - v1.z * v2.y, v2.x * v1.z - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

Vec4f Vec4f::operator+(const Vec4f& a) 
{
	return Vec4f(x + a.x, y + a.y, z + a.z);
}
Vec4f Vec4f::operator-(const Vec4f& a) 
{
	return Vec4f(x - a.x, y - a.y, z - a.z);

}

Vec3f Vec4f::toVec3() {
	return Vec3f(x, y, z);
}

Vec4f Vec4f::normalize() {

	float len = std::sqrt(x * x + y * y + z * z);
	return Vec4f(x / len, y / len, z / len);
}

Vec4f Vec4f::operator/(const float& a) {
	return Vec4f(x / a, y / a, z / a, w / a);
}

float Vec4f::operator*(const Vec4f& a) {
	return x * a.x + y * a.y + z * a.z;
}

Vec3f Vec3f::normalize() {

	float len = std::sqrt(x * x + y * y + z * z);
	return Vec3f(x / len, y / len, z / len);
}