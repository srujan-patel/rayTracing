#pragma once
#include"Walnut/Image.h"
#include<memory>
#include<glm/glm.hpp>
# include "Camera.h"
#include "Ray.h"
#include"Scene.h"

using namespace std;

class Renderer {


public:

	Renderer() = default;

	void OnResize(uint32_t width, uint32_t height);


	void Render(const Scene& scene, const Camera& Camera);
	
	shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }


private:

	struct HitPayload
	{
		float HitDistance;
		glm::vec3 WorldPosition;
		glm::vec3 WorldNormal;

		int ObjectIndex;
	};

	glm::vec4 PerPixel(uint32_t x, uint32_t y); // RayGen

	HitPayload TraceRay(const Ray& ray);
	HitPayload ClosestHit(const Ray& ray, float hitDistance, int objectIndex);
	HitPayload Miss(const Ray& ray);


private:

	const Scene* m_ActiveScene = nullptr;
	const Camera* m_ActiveCamera = nullptr;


	shared_ptr<Walnut::Image> m_FinalImage; //Walnut::Image, a smart pointer to our Walnut image

	uint32_t* m_ImageData = nullptr; // image data



};