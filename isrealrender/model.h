#pragma once
#include <vector>
#include "geometry.h"
#include <string>
#include <cmath>

class Model
{
public:
	std::string filename;
	std::vector<Vec3f> faceVertexIndexVector;
	std::vector<Vec3f> vertexVector;
	Model(const std::string filename);
};
