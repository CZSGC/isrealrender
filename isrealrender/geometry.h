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

	static Vec3f crossProduct2d(const Vec3f& v1,const Vec3f& v2)
	{
		return Vec3f(0, 0, v1.x * v2.y - v1.y * v2.x);
	}
	static float dotProduct2d(const Vec3f& v1, const Vec3f& v2) 
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

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
		return Vec3f(v, w, 1.0f - v - w);
	}

};



bool isInside(Vec3f a, Vec3f b, Vec3f c, Vec3f p);



class Vec4f 
{
public:
	float x;
	float y;
	float z;
	float w;
	Vec4f(int x, int y, int z, int w = 1) :x(x), y(y), z(z), w(w) 
	{

	}
	float& operator [](int index);
	const float& operator [](int index) const;
	static Vec4f crossProduct(const Vec4f& v1, const Vec4f& v2);
};



class Matrix4f 
{
public:
	std::vector<std::vector<float>> matrix
	{
		{1,0,0,0},
		{0,1,0,0},
		{0,0,1,0},
		{0,0,0,1}
	};

	Vec4f operator *(Vec4f v);
	std::vector<float>& operator [](int index);
	Matrix4f operator * (Matrix4f matrixc);
	Matrix4f operator + (Matrix4f matrixc);
	Matrix4f inverse();

};