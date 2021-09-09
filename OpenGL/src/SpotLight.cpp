#include "SpotLight.h"

SpotLight::SpotLight() : PointLight()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	edge = 0.0f;
	processedEdge = cosf(glm::radians(edge));
}

SpotLight::SpotLight(unsigned int shadowWidth, unsigned int shadowHeight,
					float near, float far,
					float red, float green, float blue,
					float aIntensity, float dIntensity,
					float xPos, float yPos, float zPos,
					float xDir, float yDir, float zDir,
					float con, float lin, float exp,
					float edg) : PointLight(shadowWidth, shadowHeight, near, far, red, green, blue, aIntensity, dIntensity, xPos, yPos, zPos, con, lin, exp)
{
	direction = glm::vec3(xDir, yDir, zDir);
	direction = glm::normalize(direction);
	edge = edg;
	processedEdge = cosf(glm::radians(edge));
}

void SpotLight::UseLight(unsigned int ambientIntensityLocation, unsigned int ambientColourLocation, unsigned int diffuseIntensityLocation,
						 unsigned int positionLocation, unsigned int directionLocation,
						 unsigned int constantLocation, unsigned int linearLocation, unsigned int exponentLocation,
						 unsigned int edgeLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(edgeLocation, processedEdge);
}

SpotLight::~SpotLight()
{
}

void SpotLight::SetFlash(glm::vec3 pos, glm::vec3 dir)
{
	position = pos;
	direction = dir;
}
