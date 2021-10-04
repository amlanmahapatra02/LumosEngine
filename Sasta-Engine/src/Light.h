#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShadowMap.h"

class Light
{
public:
	Light();
	Light(float shadowWidth, float shadowHeight,
		  float red, float green, float blue,
		  float aIntensity, float dIntensity);

	ShadowMap* GetShadowMap() { return shadowMap; }

	~Light();

protected:
	glm::vec3 colour;
	float ambientIntensity;
	float diffuseIntensity;

	glm::mat4 lightProj;

	ShadowMap* shadowMap;

};