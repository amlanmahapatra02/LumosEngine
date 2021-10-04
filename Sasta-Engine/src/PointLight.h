#pragma once
#include "Light.h"
#include "OmniShadowMap.h"
#include <vector>

class PointLight : public Light
{
public:
	PointLight();

	PointLight(unsigned int shadowWidth, unsigned int shadowHeight,
			   float near, float far,
			   float red, float green, float blue,
			   float aIntensity, float dIntensity,
			   float xPos, float yPos, float zPos,
			   float con, float lin, float exp);

	void UseLight(unsigned int ambientIntensityLocation, unsigned int ambientColorLocation,
				  unsigned int diffuseIntensityLocation, unsigned int positionLocation,
				  unsigned int constantLocation, unsigned int linearLocation, unsigned int exponentLocation);

	std::vector<glm::mat4> CalculateLightTransform();

	float GetFarPlane();

	glm::vec3 GetPosition();


	~PointLight();

protected:
	glm::vec3 position;

	float constant, linear, exponent;

	float farPlane;
};