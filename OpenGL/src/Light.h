#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
	Light();
	Light(float red, float green, float blue, float aIntensity, float dIntensity);


	~Light();

protected:
	glm::vec3 colour;
	float ambientIntensity;
	float diffuseIntensity;

};