#pragma once
#include <vector>
#include<glm/glm.hpp>

struct Material {

	glm::vec3 Albedo{ 1.0f };//white color
	float roughness=1.0f;
	float Metallic = 0.0f;

};

struct Sphere {
	glm::vec3 Position{0.0f, 0.0f, 0.0f};
	
	float radius = 0.5f;
	int materialindex = 0;

	
};



struct Scene {

	std::vector<Sphere> Spheres;
	std::vector<Material> materials;

};