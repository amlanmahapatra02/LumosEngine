#pragma once
#include "CommonValues.h"
#include <iostream>
#include <GL/glew.h>


class Texture
{
public:
	Texture();
	Texture(const char* fileLocation);
	~Texture();

	bool LoadTexture();
	bool LoadTextureA();

	void UseTexture();
	void ClearTexture();

private:
	unsigned int textureID;
	int width, height, bitDepth;

	const char* filepath;

};