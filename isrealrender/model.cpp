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
			std::vector<Vec3f> temp;
			float vnf, vtf,f;
			Vec3f v, vn, vt;
			for (int i = 0; i <3; i++) 
			{

				iss >> f >> trash >> vtf >> trash >> vnf;
				v[i] = --f;
				vn[i] = --vnf;
				vt[i] = --vtf;
			}
			temp.push_back(v);
			temp.push_back(vn);
			temp.push_back(vt);
			faceIndexVector.push_back(temp);
		}
		else if (!line.compare(0, 3, "vt ")) 
		{
			std::istringstream iss(line);
			iss >> trash >> trash;
			Vec3f v;
			for (int i = 0; i < 3; i++) 
			{
				iss >> v[i];
			}
			textureVertexVector.push_back(v);
		}
		else if (!line.compare(0, 3, "vn "))
		{
			std::istringstream iss(line);
			iss >> trash >> trash;
			Vec3f v;
			for (int i = 0; i < 3; i++) 
			{
				iss >> v[i];
			}
			normalVertexVector.push_back(v);
		}
	}
}


