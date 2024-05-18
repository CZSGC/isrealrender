#include "geometry.h"
#include "cmath"

Vec3f Vec3f::operator+(const Vec3f& a) 
{
	return Vec3f(x + a.x, y + a.y,z+a.z);
}

Vec3f Vec3f::operator-(const Vec3f& a) {
	return Vec3f(x - a.x, y - a.y, z - a.z);
}

void Vec3f::setzOne() {
	x /= z;
	y /= z;
	z /= z;
}

bool isInside(Vec3f a, Vec3f b, Vec3f c, Vec3f p) {
	a.setzOne();
	b.setzOne();
	c.setzOne();
	p.setzOne();
	Vec3f ab = a - b;
	Vec3f cb = c - b;
	Vec3f N = Vec3f::crossProduct(ab, cb);

	Vec3f pb = p - b;
	Vec3f T = Vec3f::crossProduct(ab, pb);
	if (Vec3f::dotProduct(T, N) < 0) return false;

	T = Vec3f::crossProduct(pb, cb);
	if (Vec3f::dotProduct(T, N) < 0) return false;

	Vec3f pc = p - c;
	Vec3f ac = a - c;
	T = Vec3f::crossProduct(pc, ac);
	if (Vec3f::dotProduct(T, N) < 0) return false;

	return true;

}
void Vec3f::normalization() {
	float len = std::sqrt(x * x + y * y );
	x = x / len;
	y = y / len;
	z = z / len;
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