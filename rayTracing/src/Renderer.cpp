#include "Renderer.h"
#include"Walnut/Random.h"



namespace Utils {

	static uint32_t ConvertToRGBA(const glm::vec4& color) {
		uint8_t r= uint8_t (color.r * 255.0f);// each channel holds 8 bits
		uint8_t g = uint8_t(color.g * 255.0f);// each channel holds 8 bits
		uint8_t b = uint8_t(color.b * 255.0f);// each channel holds 8 bits
		uint8_t a = uint8_t(color.a * 255.0f);// each channel holds 8 bits
		uint32_t result = (a<<24) | (b<<16) | (g<<8) | (r);
		return result;

	}
}

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


void Renderer::Render(const Scene& scene, const Camera& Camera) {

	m_ActiveScene = &scene;
	m_ActiveCamera = &Camera;

	const glm::vec3& rayOrigin = Camera.GetPosition();
	

	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++){

	for (uint32_t x = 0; x <m_FinalImage->GetWidth(); x++) {//iterate through every pixel

		//m_ImageData[i] = 0xffff3333;

		//m_ImageData[i] = Walnut::Random::UInt(); //fill the image pixels using random image colors
		//m_ImageData[i] = 0xff000000;// make the alpha channel one to make the pictures look solid

		glm::vec4 color = PerPixel(x,y);
		color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
		m_ImageData[x+y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(color);

		}

	}

	m_FinalImage->SetData(m_ImageData);

	}

glm::vec4 Renderer::PerPixel(uint32_t x,uint32_t y)
{
	Ray ray;
	ray.Origin = m_ActiveCamera->GetPosition();
	ray.Direction = m_ActiveCamera->GetRayDirections()[x + y * m_FinalImage->GetWidth()];


	glm::vec3 color(0.0f);
	float multiplier = 1.0f;
	int bounces = 2;
	for (int i =0; i < bounces; i++) {

		Renderer::HitPayload payload = TraceRay(ray);
		if (payload.HitDistance < 0.0f) {

			glm::vec3 skyColor = glm::vec3(0.0f, 0.0f, 0.0f);
			color += skyColor * multiplier;
			break;
		
		
		}

		glm::vec3 lightDirection(-1.0f, -1.0f, -1.0f);
		glm::vec3 lightDir = glm::normalize(lightDirection);//normalised light


		float dotp = glm::max(glm::dot(payload.WorldNormal, -lightDir), 0.0f); //==cos angle ranges from -1 to 1 we are clamping it to 0 to 1 just from one side


		const Sphere& sphere = m_ActiveScene->Spheres[payload.ObjectIndex];

		glm::vec3 sphereColor = sphere.Albedo; //ignoring the alpha channel for the sphere color
		//			sphereColor = normal*0.5f +0.5f;//a normal vector ranges from -1 to 1 but our color range is from 0 to 1 so we shift the color to start from 1

		sphereColor *= dotp;

		color += sphereColor* multiplier;
		multiplier *= 0.7f;


		ray.Origin = payload.WorldPosition+ payload.WorldNormal *0.001f;
		ray.Direction = glm::reflect(ray.Direction, payload.WorldPosition);
	}
	
	return glm::vec4(color, 1.0f);
	
	}






Renderer::HitPayload Renderer::TraceRay(const Ray& ray)
{

	//uint8_t r = (uint8_t)(coord.x * 255.0f);
	//uint8_t g = (uint8_t)(coord.y * 255.0f);



	//equation of ray intersecting a sphere is given by
	//(bx^2+by^2+bz^2)t^2 +2(axbx+ayby+azbz)t +(ax^2+ay^2+az^2-r^2)=0

	//we solve for t distance along the ray
	//a = origin of the ray
	//b = direction of the ray
	//r is radius of sphere
	//t=hit distance

	// this can be written in form of quad equation ax^2 + by^2+c=0

	float radius = 0.5f;



	int closestSphere = -1;
	float hitDistance = std::numeric_limits<float>::max();
	for (size_t i = 0; i < m_ActiveScene->Spheres.size(); i++) {// iterate through the spheres

		//const Sphere& sphere = scene.Spheres[0];

		const Sphere& sphere = m_ActiveScene->Spheres[i];

		glm::vec3 origin = ray.Origin - sphere.Position;

		float a = glm::dot(ray.Direction, ray.Direction);
		float b = 2.0f * glm::dot(origin, ray.Direction);
		float c = glm::dot(origin, origin) - sphere.radius * sphere.radius;

		float d = b * b - 4.0f * a * c;

		//-b+_sqrt(d)/2a

		if (d < 0)
			//			return glm::vec4(0, 0, 0, 1); //return black color
			continue;

		//			float t0 = (-b + glm::sqrt(d)) / (2.0f * a);//bigger solution 
		float closestT = (-b - glm::sqrt(d)) / (2.0f * a);//smaller solution 
		if (closestT>0.0f && closestT < hitDistance) {
			hitDistance = closestT;
			closestSphere = (int)i;

		}

	}

	if (closestSphere < 0) {
		return Miss(ray);

	}

	return ClosestHit(ray, hitDistance, closestSphere);




}





Renderer::HitPayload Renderer::ClosestHit(const Ray& ray, float hitDistance, int objectIndex)
{
	
	Renderer::HitPayload payload;
	payload.HitDistance = hitDistance;
	payload.ObjectIndex = objectIndex;

	
	const Sphere& closestSphere = m_ActiveScene->Spheres[objectIndex];

	glm::vec3 origin = ray.Origin - closestSphere.Position;

	payload.WorldPosition = origin + ray.Direction * hitDistance;
	payload.WorldNormal = glm::normalize(payload.WorldPosition);
	//glm::vec3 h0 = rayOrigin + rayDirection * t0; //other hitpoint 
	payload.WorldPosition += closestSphere.Position;


	return payload;

}


Renderer::HitPayload Renderer::Miss(const Ray& ray)
{
	Renderer::HitPayload payload;
	payload.HitDistance = -1.0f;
	return payload;


}

