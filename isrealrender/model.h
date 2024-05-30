#pragma once
#include <vector>
#include "geometry.h"
#include <string>
#include <cmath>

class BoundingBox 
{
public:
	float left;
	float right;
	float top;
	float bottom;
	float front;
	float back;
	Vec3f midpoint;

	float getLengthX();
	float getLengthY();
	float getLengthZ();

	Vec3f getMidpoint() {
		return midpoint;
	}

	void setMidpoint() {
		midpoint = Vec3f((left + right / 2), (top + bottom) / 2, (front + back) / 2);
	}

	BoundingBox();
};

class Model
{
public:
	std::string filename;
	std::vector<std::vector<Vec3f>> faceIndexVector;
	std::vector<Vec3f> vertexVector;
	std::vector<Vec3f> normalVertexVector;
	std::vector<Vec3f> textureVertexVector;
	BoundingBox boudingbox;


	Model(const std::string filename);
};
