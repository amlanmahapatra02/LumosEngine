#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
	Light();
	Light(float red, float green, float blue, float aIntensity,
		  float xDir,float yDir, float zDir, float dIntensity);

	void UseLight(float ambientIntensityLocation, float ambientColorLocation,
				  float diffuseIntensityLocation, float directionLocation);

	~Light();

private:
	glm::vec3 color;
	float ambientIntensity;

	glm::vec3 direction;
	float diffuseIntensity;

};