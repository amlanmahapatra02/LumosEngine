#include "Shader.h"
#include <string>

Shader::Shader()
{
	shaderID = 0;
	uniformProjection = 0;
	uniformModel = 0;
}

Shader::~Shader()
{
	ClearShader();
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);

	const char* vertexCode = vertexString.c_str();
	const char* fragementCode = fragmentString.c_str();

	CompileShader(vertexCode, fragementCode);
}

std::string Shader::ReadFile(const char* filepath)
{
	std::string content;
	std::ifstream fileStream(filepath, std::ios::in);

	if (!fileStream.is_open())
	{
		std::cout << "Failed to Read the file " << filepath << std::endl;
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

unsigned int Shader::GetProjectionLocation()
{
	return uniformProjection;
}

unsigned int Shader::GetModelLocation()
{
	return uniformModel;
}

unsigned int Shader::GetViewLocation()
{
	return uniformView;
}

unsigned int Shader::GetAmbientIntensityLocation()
{
	return uniformAmibentIntensity;
}

unsigned int Shader::GetAmbientColorLocation()
{
	return uniformAmibentColor;
}

unsigned int Shader::GetDiffuseIntensityLocation()
{
	return uniformDiffuseIntensity;
}

unsigned int Shader::GetDirectionLocation()
{
	return uniformDirection;
}

unsigned int Shader::GetEyePositionLocation()
{
	return uniformEyePosition;
}

unsigned int Shader::GetSpecularIntensityLocation()
{
	return uniformSpecularIntensity;
}

unsigned int Shader::GetShininessLocation()
{
	return uniformShininess;
}

void Shader::UseShader()
{
	glUseProgram(shaderID);
}

void Shader::ClearShader()
{
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	uniformModel = 0;
	uniformProjection = 0;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		std::cout << "Failed to create shader " << std::endl;
		return;
	}

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	int result = 0;
	char log[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(log), NULL, log);
		std::cout << "Error Linking Program : " << log << std::endl;
		return;
	}

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(log), NULL, log);
		std::cout << "Error Validating Shaders : " << log << std::endl;
		return;
	}

	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformView = glGetUniformLocation(shaderID, "view");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformAmibentIntensity = glGetUniformLocation(shaderID, "directionalLight.ambientIntensity");
	uniformAmibentColor = glGetUniformLocation(shaderID, "directionalLight.colour");
	uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
	uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.diffuseIntensity");
	uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");
	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
}

void Shader::AddShader(unsigned int theProgram, const char* shaderCode, GLenum shaderType)
{
	unsigned int theShader = glCreateShader(shaderType);

	const char* theCode[1];
	theCode[0] = shaderCode;

	int codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	int result = 0;
	char log[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(log), NULL, log);
		std::cout << "Error Compiling Shader" << shaderType << "  " << log << std::endl;
		return;
	}

	glAttachShader(theProgram, theShader);
}
