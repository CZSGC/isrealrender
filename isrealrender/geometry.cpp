#include "geometry.h"
#include "cmath"

Vec3f Vec3f::operator+(const Vec3f& a) 
{
	return Vec3f(x + a.x, y + a.y,z+a.z);
}

Vec3f Vec3f::operator-(const Vec3f& a) {
	return Vec3f(x - a.x, y - a.y, z - a.z);
}

float& Vec3f::operator[](int i) {
	if (i == 0) {
		return x;
	}
	else if (i == 1) {
		return y;
	}
	else {
		return z;
	}
}


bool isInside(Vec2f a, Vec2f b, Vec2f c, Vec2f p) {
	Vec2f ab = a - b;
	Vec2f cb = c - b;
	Vec3f N = Vec2f::crossProduct2d(ab, cb);

	Vec2f pb = p - b;
	Vec3f T = Vec2f::crossProduct2d(ab, pb);
	if (Vec3f::dotProduct(T, N) < 0) return false;

	T = Vec2f::crossProduct2d(pb, cb);
	if (Vec3f::dotProduct(T, N) < 0) return false;

	Vec2f pc = p - c;
	Vec2f ac = a - c;
	T = Vec2f::crossProduct2d(pc, ac);
	if (Vec3f::dotProduct(T, N) < 0) return false;

	return true;

}

Vec2f Vec2f::operator+(const Vec2f& a)
{
	return Vec2f(x + a.x, y + a.y);
}

Vec2f Vec2f::operator-(const Vec2f& a) {
	return Vec2f(x - a.x, y - a.y);
}

float& Vec2f::operator[](int i) {
	if (i == 0) {
		return x;
	}
	else if (i == 1) {
		return y;
	}
}