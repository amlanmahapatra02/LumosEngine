#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>;


class Shader
{
public:
	Shader();
	~Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
	std::string ReadFile(const char* filepath);

	unsigned int GetProjectionLocation();
	unsigned int GetModelLocation();

	void UseShader();
	void ClearShader();

private:
	unsigned int shaderID, uniformProjection, uniformModel;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(unsigned int theProgram, const char* shaderCode, GLenum shaderType);
};