#pragma once
#include"Walnut/Image.h"
#include<memory>
#include<glm/glm.hpp>

using namespace std;

class Renderer {


public:

	Renderer() = default;

	void OnResize(uint32_t width, uint32_t height);


	void Render();
	
	shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }


private:
	glm::vec4 PerPixel(glm::vec2 coord);


private:



	shared_ptr<Walnut::Image> m_FinalImage; //Walnut::Image, a smart pointer to our Walnut image

	uint32_t* m_ImageData = nullptr; // image data



};