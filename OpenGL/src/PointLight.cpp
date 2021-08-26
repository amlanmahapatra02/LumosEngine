#include "PointLight.h"

PointLight::PointLight() : Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f; // to avoid divison by zero
	linear = 0.0f;
	exponent = 0.0f;
}

PointLight::PointLight(float red, float green, float blue, 
					   float aIntensity, float dIntensity, 
					   float xPos, float yPos, float zPos, float con, float lin, float exp) : Light(red, green, blue, aIntensity, dIntensity)
{
	position = glm::vec3(xPos, yPos, zPos);
	constant = con;
	linear = lin;
	exponent = exp;
}

void PointLight::UseLight(float ambientIntensityLocation, float ambientcolourLocation,
						  float diffuseIntensityLocation, float positionLocation,
						  float constantLocation, float linearLocation, float exponentLocation)
{
	glUniform3f(ambientcolourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
}

PointLight::~PointLight()
{
}
