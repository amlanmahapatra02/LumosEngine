#pragma once
#include <iostream>
#include <GL/glew.h>
#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(const char* fileLocation);
	~Texture();

	void LoadTexture();
	void UseTexture();
	void ClearTexture();

private:
	unsigned int textureID;
	int width, height, bitDepth;

	const char* filepath;

};