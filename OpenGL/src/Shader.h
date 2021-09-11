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
	void CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation);

	void Validate();

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
	unsigned int GetOmniLightPosLocation();
	unsigned int GetFarPlaneLocation();


	void SetDirectionalLight(DirectionalLight* dLight);
	void SetPointLights(PointLight* pLight, unsigned int lightCount,unsigned int textureUnit, unsigned int offset);
	void SetSpotLights(SpotLight* sLight, unsigned int lightCount,unsigned int textureUnit, unsigned int offset);
	void SetTexture(unsigned int textureUnit);
	void SetDirectionalShadowMap(unsigned int textureUnit);
	void SetDirectionalLightTransform(glm::mat4* lTransform);
	void SetLightMatrices(std::vector<glm::mat4> lightMatrices);

	void UseShader();
	void ClearShader();

	~Shader();

private:
	int pointLightCount;
	int spotLightCount;

	unsigned int shaderID, uniformProjection, uniformModel, uniformView, uniformEyePosition,
		uniformSpecularIntensity, uniformShininess,
		uniformTexture,
		uniformDirectionalLightTransform, uniformDirectionalShadowMap,
		uniformOmniLightPos, uniformFarPlane;

	unsigned int UniformLightMatrices[6];

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

	struct
	{
		unsigned int shadowMap;
		unsigned int farPlane;

	}uniformOmniShadowMap[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode);
	void AddShader(unsigned int theProgram, const char* shaderCode, GLenum shaderType);

	void CompileProgram();
};

