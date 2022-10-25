#pragma once
#include <vector>
#include<glm/glm.hpp>

struct Sphere {
	glm::vec3 Position{0.0f, 0.0f, 0.0f};
	glm::vec3 Albedo{1.0f};//white color
	float radius = 0.5f;
	
};



struct Scene {

	std::vector<Sphere> Spheres;

};