#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "CommonValues.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class Shader
{
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

	std::string ReadFile(const char* fileLocation);

	unsigned int GetProjectionLocation();
	unsigned int GetModelLocation();
	unsigned int GetViewLocation();
	unsigned int GetAmbientIntensityLocation();
	unsigned int GetAmbientColourLocation();
	unsigned int GetDiffuseIntensityLocation();
	unsigned int GetDirectionLocation();
	unsigned int GetSpecularIntensityLocation();
	unsigned int GetShininessLocation();
	unsigned int GetEyePositionLocation();


	void SetDirectionalLight(DirectionalLight* dLight);
	void SetPointLights(PointLight* pLight, unsigned int lightCount);
	void SetSpotLights(SpotLight* sLight, unsigned int lightCount);
	void SetTexture(unsigned int textureUnit);
	void SetDirectionalShadowMap(unsigned int textureUnit);
	void SetDirectionalLightTransform(glm::mat4* lTransform);

	void UseShader();
	void ClearShader();

	~Shader();

private:
	int pointLightCount;
	int spotLightCount;

	unsigned int shaderID, uniformProjection, uniformModel, uniformView, uniformEyePosition,
		uniformSpecularIntensity, uniformShininess,
		uniformTexture,
		uniformDirectionalLightTransform, uniformDirectionalShadowMap;

	struct {
		unsigned int uniformColour;
		unsigned int uniformAmbientIntensity;
		unsigned int uniformDiffuseIntensity;
		unsigned int uniformDirection;

	} uniformDirectionalLight;

	unsigned int uniformPointLightCount;
	

	struct {
		unsigned int uniformColour;
		unsigned int uniformAmbientIntensity;
		unsigned int uniformDiffuseIntensity;

		unsigned int uniformPosition;
		unsigned int uniformConstant;
		unsigned int uniformLinear;
		unsigned int uniformExponent;

	} uniformPointLight[MAX_POINT_LIGHTS];

	unsigned int uniformSpotLightCount;

	struct 
	{
		unsigned int uniformColour;
		unsigned int uniformAmbientIntensity;
		unsigned int uniformDiffuseIntensity;

		unsigned int uniformPosition;
		unsigned int uniformConstant;
		unsigned int uniformLinear;
		unsigned int uniformExponent;

		unsigned int uniformDirection;
		unsigned int uniformEdge;

	}uniformSpotLight[MAX_SPOT_LIGHTS];

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(unsigned int theProgram, const char* shaderCode, GLenum shaderType);
};

