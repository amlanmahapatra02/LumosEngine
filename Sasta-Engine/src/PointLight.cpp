#include "PointLight.h"

PointLight::PointLight() : Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f; // to avoid divison by zero
	linear = 0.0f;
	exponent = 0.0f;
}

PointLight::PointLight(unsigned int shadowWidth, unsigned int shadowHeight,
					   float near, float far,
	                   float red, float green, float blue, 
					   float aIntensity, float dIntensity, 
					   float xPos, float yPos, float zPos, float con, float lin, float exp) : Light(shadowWidth,shadowHeight,red, green, blue, aIntensity, dIntensity)
{
	position = glm::vec3(xPos, yPos, zPos);
	constant = con;
	linear = lin;
	exponent = exp;

	farPlane = far;

	float aspectRatio = (float)shadowWidth / (float)shadowHeight;
	lightProj = glm::perspective(glm::radians(90.0f), aspectRatio, near, far);

	shadowMap = new OmniShadowMap();
	shadowMap->Init(shadowWidth, shadowHeight);
}

void PointLight::UseLight(unsigned int ambientIntensityLocation, unsigned int ambientColourLocation,
						  unsigned int diffuseIntensityLocation, unsigned int positionLocation,
						  unsigned int constantLocation, unsigned int linearLocation, unsigned int exponentLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
}

std::vector<glm::mat4> PointLight::CalculateLightTransform()
{
	std::vector<glm::mat4> lightMatrices;

	// +x , -x
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));

	//+y, -y
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));

	//z, -z
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));


	return lightMatrices;

}

float PointLight::GetFarPlane()
{
	return farPlane;
}

glm::vec3 PointLight::GetPosition()
{
	return position;
}

PointLight::~PointLight()
{
}
