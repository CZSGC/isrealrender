#include "model.h"
#include "geometry.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

Model::Model(std::string filename) 
{
	std::ifstream in(filename);
	if (in.fail()) 
	{
		std::cerr << "failed open file";
	}
	char trash;
	int itrash;
	std::string line;
	while (std::getline(in, line)) 
	{
		if (!line.compare(0, 2, "v ")) 
		{
			std::istringstream iss(line);
			char trash;
			iss >> trash;
			Vec3f v;
			for (int i = 0; i < 3; i++) 
			{
				iss >> v[i];
			}
			vertexVector.push_back(v);
		}else if (!line.compare(0, 2, "f ")) 
		{
			std::istringstream iss(line);
			iss >> trash ;
			float f;
			Vec3f v;
			for (int i = 0; i <3; i++) 
			{

				iss >> f >> trash >> itrash >> trash >> itrash;
				v[i] = --f;
			}
			faceVertexIndexVector.push_back(v);
		}
		else if (!line.compare(0, 3, "vt ")) 
		{

		}
		else if (!line.compare(0, 3, "vn "))
		{

		}
	}
}


