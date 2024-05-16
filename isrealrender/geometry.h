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
	Vec3f(const float& x, const float& y, const float& z) :x(x), y(y),z(z)
	{

	}
	Vec3f operator+(const Vec3f& a);
	static Vec3f crossProduct(const Vec3f& v1,const Vec3f& v2)
	{
		return Vec3f(v1.y * v2.z - v1.z * v2.y, v2.x * v1.z - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
	}
	static float dotProduct(const Vec3f& v1, const Vec3f& v2) 
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
};
bool isInside(Vec3f a, Vec3f b, Vec3f v);