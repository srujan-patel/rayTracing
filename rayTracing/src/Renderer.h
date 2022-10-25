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
	glm::vec4 TraceRay(const Scene& scene, const Ray& ray);


private:



	shared_ptr<Walnut::Image> m_FinalImage; //Walnut::Image, a smart pointer to our Walnut image

	uint32_t* m_ImageData = nullptr; // image data



};