#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"
#include"Renderer.h"

using namespace std;
using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{

	Renderer m_Renderer;
	//we will be using uint_32 because that is the size of the RGBA format

	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0; //setting them to 0 by default
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
			
			shared_ptr<Walnut::Image> image = m_Renderer.GetFinalImage();
			
			if(image)// imgui will crash if there is nothing to render initially for few pixels
			ImGui::Image(image->GetDescriptorSet(), {(float)image->GetWidth(), (float)image->GetHeight() }, ImVec2(0,1), ImVec2(1,0));// display the image, width and height is the initial width and height of the viewport window

			ImGui::End();

			ImGui::PopStyleVar();
			Render();
	}

	void Render() {//recreate the image

		Timer timer;

		m_Renderer.OnResize(m_ViewportHeight, m_ViewportHeight);
		m_Renderer.Render();


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