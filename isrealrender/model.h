#pragma once
#include <vector>
#include "geometry.h"
#include <string>
#include <cmath>

class Model
{
public:
	std::string filename;
	std::vector<int> faceP;
	Model(const std::string filename);
};


std::vector<float> normalization(std::vector<float> n) {
	float len = std::sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
	n[0]=
}