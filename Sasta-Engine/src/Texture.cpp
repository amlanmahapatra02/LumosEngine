#include "Texture.h"

Texture::Texture()
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	filepath = "";
}

Texture::Texture(const char* fileLocation)
{
	filepath = fileLocation;
}

Texture::~Texture()
{
	ClearTexture();
}

// Loads Textures which does a have Alpha channel
bool Texture::LoadTextureA()
{
	unsigned char* textData = stbi_load(filepath, &width, &height, &bitDepth, 0);

	if (!textData)
	{
		std::cout << "Failed to find " << filepath << std::endl;
		return false;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//smooth look while zooming
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textData);
	glGenerateMipmap(GL_TEXTURE_2D);

	//unbind
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(textData);

	return true;
}

// Loads Textures which doesnot have a Alpha channel
bool Texture::LoadTexture()
{
	unsigned char* textData = stbi_load(filepath, &width, &height, &bitDepth, 0);

	if (!textData)
	{
		std::cout << "Failed to find " << filepath << std::endl;
		return false;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//smooth look while zooming
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textData);
	glGenerateMipmap(GL_TEXTURE_2D);

	//unbind
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(textData);

	return true;
}

void Texture::UseTexture()
{
	glActiveTexture(GL_TEXTURE1); //Texture unit
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &textureID);

	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	filepath = "";
}
