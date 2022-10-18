#include "Renderer.h"
#include"Walnut/Random.h"

void Renderer::OnResize(uint32_t width, uint32_t height) {

	if(m_FinalImage){
		// if no resize is neccessary
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
			return;
		m_FinalImage->Resize(width, height); //keeping the same object but reallocating some memory 
		
	}
	else {
		m_FinalImage = make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);// if there is no image create an image with the width and the height and the image format


	}
	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height]; //realloacte the memory buffer according to the viewport dimensions

}


void Renderer::Render() {


	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++){

	for (uint32_t x = 0; x <m_FinalImage->GetWidth(); x++) {//iterate through every pixel

		//m_ImageData[i] = 0xffff3333;

		//m_ImageData[i] = Walnut::Random::UInt(); //fill the image pixels using random image colors
		//m_ImageData[i] = 0xff000000;// make the alpha channel one to make the pictures look solid

		glm::vec2 coord = { (float)x / (float)m_FinalImage->GetWidth(), (float)y/(float)m_FinalImage->GetHeight() };// iterating through every pixel
		
		m_ImageData[x+y * m_FinalImage->GetWidth()] = PerPixel(coord);

		}

	}

	m_FinalImage->SetData(m_ImageData);

	}



uint32_t Renderer::PerPixel(glm::vec2 coord)
{

	uint8_t r = (uint8_t)(coord.x * 255.0f);
	uint8_t g = (uint8_t)(coord.y * 255.0f);

	return 0xff000000 | (g<<8) |r;
}

