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



	for (uint32_t i = 0; i < m_FinalImage->GetWidth() * m_FinalImage->GetHeight(); i++) { // generate the image 
		//m_ImageData[i] = 0xffff3333;
		m_ImageData[i] = Walnut::Random::UInt(); //fill the image pixels using random image colors
		//m_ImageData[i] = 0xff000000;// make the alpha channel one to make the pictures look solid
	}

	m_FinalImage->SetData(m_ImageData);


}