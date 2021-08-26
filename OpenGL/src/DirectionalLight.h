#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();
	DirectionalLight(float red, float green, float blue, float aIntensity, float dIntensity,
					 float xDir, float yDir, float zDir);

	void UseLight(unsigned int ambientIntensityLocation, unsigned int ambientColorLocation,
				  unsigned int diffuseIntensityLocation, unsigned int directionLocation);

	~DirectionalLight();

private:
	glm::vec3 direction;
};

