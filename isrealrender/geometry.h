#pragma once
#include <vector>

class Vec3f
{
public:
	float x;
	float y;
	float z;
	Vec3f()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	Vec3f(float x, float y, float z=1) :x(x), y(y),z(z)
	{

	}


	Vec3f operator+(const Vec3f& a);
	Vec3f operator-(const Vec3f& a);
	float& operator[](int i);
	float operator*(const Vec3f& a);

	static Vec3f crossProduct2d(const Vec3f& v1,const Vec3f& v2)
	{
		return Vec3f(0, 0, v1.x * v2.y - v1.y * v2.x);
	}

	static Vec3f crossProduct(const Vec3f& a, const Vec3f& b)
	{
		return Vec3f(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
	}
	
	static float dotProduct2d(const Vec3f& v1, const Vec3f& v2) 
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

	static float dotProduct(const Vec3f& v1, const Vec3f& v2);

	static Vec3f barycentric(Vec3f a, Vec3f b, Vec3f c, Vec3f p)
	{
		Vec3f v0 = b - a, v1 = c - a, v2 = p - a;
		float d00 = dotProduct2d(v0, v0);
		float d01 = dotProduct2d(v0, v1);
		float d11 = dotProduct2d(v1, v1);
		float d20 = dotProduct2d(v2, v0);
		float d21 = dotProduct2d(v2, v1);
		float denom = d00 * d11 - d01 * d01;
		float v = (d11 * d20 - d01 * d21) / denom;
		float w = (d00 * d21 - d01 * d20) / denom;
		return Vec3f(1.0 - v - w, v, w);
	}
	Vec3f normalize();
};



bool isInside(Vec3f a, Vec3f b, Vec3f c, Vec3f p);



class Vec4f 
{
public:
	float x;
	float y;
	float z;
	float w;
	Vec4f(float x, float y, float z, float w = 1) :x(x), y(y), z(z), w(w)
	{

	}
	Vec4f(Vec3f v):Vec4f(v.x,v.y,v.z) 
	{
	}
	Vec4f operator+(const Vec4f& a);
	Vec4f operator-(const Vec4f& a);
	float& operator [](int index);
	Vec4f operator/(const float& a);
	const float& operator [](int index) const;
	static Vec4f crossProduct(const Vec4f& v1, const Vec4f& v2);
	Vec3f toVec3();
	Vec4f normalize();
	float operator*(const Vec4f& a);
};

class Matrix3f
{
public:
	//float matrix[4][4];

	std::vector<std::vector<float>> matrix;

	Matrix3f();
	Matrix3f operator * (Matrix3f matrixc);
	std::vector<float>& operator [](int index);
	Matrix3f Transpose();
	Matrix3f Inverse();
	float determinant();
	Matrix3f adjoint();
	Vec3f operator * (Vec3f vec);
};

class Matrix4f 
{
public:
	//float matrix[4][4];

	std::vector<std::vector<float>> matrix;

	Matrix4f();

	Vec4f operator *(Vec4f v);
	std::vector<float>& operator [](int index);
	Matrix4f operator * (Matrix4f matrixc);
	Matrix4f operator + (Matrix4f matrixc);
	Matrix4f inverse();
	Matrix3f toMatrix3f();

};

