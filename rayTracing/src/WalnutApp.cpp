#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"


using namespace std;
using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{

	shared_ptr<Image> m_Image; //Walnut::Image, a smart pointer to our Walnut image

	//we will be using uint_32 because that is the size of the RGBA format

	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0; //setting them to 0 by default
	uint32_t* m_ImageData = nullptr; // image data
	float m_LastRenderTime = 0.0f;

public:
	virtual void OnUIRender() override //get called every frame
	{
		ImGui::Begin("Settings"); //settings window

		ImGui::Text("Last Render at %.3fms", m_LastRenderTime);

		if (ImGui::Button("Render") ){

			Render(); // render(generate) the image
		}

		ImGui::End();


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)); //for padding viewpoint window


			ImGui::Begin("Viewport");
			m_ViewportWidth=ImGui::GetContentRegionAvail().x; //get available height and width of the viewpoint window 
			m_ViewportHeight=ImGui::GetContentRegionAvail().y;
			
			
			if(m_Image)// imgui will crash if there is nothing to render initially for few pixels
			ImGui::Image(m_Image->GetDescriptorSet(), {(float)m_Image->GetWidth(), (float)m_Image->GetHeight()});// display the image, width and height is the initial width and height of the viewport window

			ImGui::End();

			ImGui::PopStyleVar();
			Render();
	}

	void Render() {//recreate the image

		Timer timer;

		if (!m_Image||m_ViewportWidth!= m_Image->GetWidth()||m_ViewportHeight!=m_Image->GetHeight()) { //img equals nullptr
			m_Image = make_shared<Image>(m_ViewportWidth, m_ViewportHeight, ImageFormat::RGBA);// resize the image to match the viewport dimensions

			delete[] m_ImageData;
			m_ImageData = new uint32_t[m_ViewportWidth* m_ViewportHeight]; //realloacte the memory buffer according to the viewport dimensions
		}



		for (uint32_t i = 0; i < m_ViewportHeight * m_ViewportWidth; i++) { // generate the image 
			//m_ImageData[i] = 0xffff3333;
			m_ImageData[i] = Random::UInt(); //fill the image pixels using random image colors
			//m_ImageData[i] = 0xff000000;// make the alpha channel one to make the pictures look solid
		}

		m_Image->SetData(m_ImageData);

		m_LastRenderTime = timer.ElapsedMillis();
	}

};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Ray Tracing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}