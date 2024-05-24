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
	std::vector<Vec3f> normalVertexVector;
	std::vector<Vec3f> TextureVertexVector;


	Model(const std::string filename);
};
