#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include<glm/gtc/type_ptr.hpp>
#include "Walnut/Image.h"
#include "Walnut/Timer.h"
#include"Renderer.h"
#include"Camera.h"

using namespace std;
using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
	Scene m_Scene;
	Camera m_Camera;
	Renderer m_Renderer;
	//we will be using uint_32 because that is the size of the RGBA format

	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0; //setting them to 0 by default
	float m_LastRenderTime = 0.0f;

public:
	ExampleLayer() :m_Camera(45.0f, 0.1f, 100.0f) {

		Material& pinkSphere= m_Scene.materials.emplace_back();
		pinkSphere.Albedo = { 1.0f, 0.0f, 1.0f };
		pinkSphere.roughness = 0.0f;
		Material& blueSphere = m_Scene.materials.emplace_back();
		blueSphere.Albedo = { 0.2f, 0.3f, 1.0f };
		blueSphere.roughness = 0.1f;

		{Sphere sphere;
		sphere.Position = { 0.0f, 0.0f,0.0f };
		sphere.radius = 1.0f;
		//sphere.mat.Albedo = { 1.0f, 0.0f, 1.0f };
		sphere.materialindex = 0;
		m_Scene.Spheres.push_back(sphere);
		}
		{		Sphere sphere;
		sphere.Position = { 1.0f, -101.0f,0.0f };
		sphere.radius = 100.0f;
		//sphere.mat.Albedo = { 0.2f, 0.3f, 1.0f };
		sphere.materialindex = 1;
		m_Scene.Spheres.push_back(sphere); }

	};

	virtual void OnUpdate(float ts) override
	{
		m_Camera.OnUpdate(ts);
	}
	virtual void OnUIRender() override //get called every frame
	{
		ImGui::Begin("Settings"); //settings window

		ImGui::Text("Last Render at %.3fms", m_LastRenderTime);

		if (ImGui::Button("Render") ){

			Render(); // render(generate) the image
		}

		ImGui::End();

		ImGui::Begin("Scene"); //scene window


		for(size_t i=0; i< m_Scene.Spheres.size(); i++){
			ImGui::PushID(i);
Sphere& sphere=	m_Scene.Spheres[i];

			ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.1f);
		ImGui::DragFloat("Radius", &sphere.radius, 0.1f);
		ImGui::DragInt("Material", &sphere.materialindex, 1.0f, 0, (int)m_Scene.materials.size()-1);
		
		
		ImGui::Separator();
		ImGui::PopID();
		}
		for (size_t i = 0; i < m_Scene.materials.size(); i++) {
			ImGui::PushID(i);
			Material& material = m_Scene.materials[i];

			ImGui::ColorEdit3("Albedo", glm::value_ptr(material.Albedo));
			ImGui::DragFloat("Roughness", &material.roughness, 0.05f, 0.0f, 1.0f);
			ImGui::DragFloat("Metallic Value", &material.Metallic, 0.05f, 0.0f, 1.0f);

			ImGui::Separator();
			ImGui::PopID();



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
		m_Camera.OnResize(m_ViewportHeight, m_ViewportHeight);
		m_Renderer.Render(m_Scene, m_Camera);


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