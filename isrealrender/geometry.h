#pragma once
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

	static Vec3f crossProduct(const Vec3f& v1,const Vec3f& v2)
	{
		return Vec3f(v1.y * v2.z - v1.z * v2.y, v2.x * v1.z - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
	}
	static float dotProduct(const Vec3f& v1, const Vec3f& v2) 
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

};


class Vec2f
{
public:
	float x;
	float y;
	Vec2f()
	{
		x = 0;
		y = 0;
	}
	Vec2f(float x, float y) :x(x), y(y)
	{

	}
	Vec2f(Vec3f v) :x(v.x), y(v.y) 
	{

	}


	Vec2f operator+(const Vec2f& a);
	Vec2f operator-(const Vec2f& a);
	float& operator[](int i);

	static Vec3f crossProduct2d(const Vec2f& v1, const Vec2f& v2)
	{
		return Vec3f( 0,0, v1.x * v2.y - v1.y * v2.x);
	}
	static float dotProduct2d(const Vec2f& v1, const Vec2f& v2)
	{
		return v1.x * v2.x + v1.y * v2.y ;
	}
	static Vec3f barycentric(Vec2f a, Vec2f b, Vec2f c, Vec2f p) 
	{
		Vec2f v0 = b - a, v1 = c - a, v2 = p - a;
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

bool isInside(Vec2f a, Vec2f b, Vec2f c, Vec2f p);