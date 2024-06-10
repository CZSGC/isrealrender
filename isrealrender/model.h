#pragma once
#include <vector>
#include "geometry.h"
#include <string>
#include <cmath>
#include "texture.h"


class Model
{
public:
	std::string filename;
	std::vector<std::vector<Vec3f>> faceIndexVector;
	std::vector<Vec3f> vertexVector;
	std::vector<Vec3f> normalVertexVector;
	std::vector<Vec3f> textureVertexVector;

	Model(const std::string filename);
};
