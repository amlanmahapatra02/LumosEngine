#pragma once
#include <iostream>

#include <GL/glew.h>
#include <glfw3.h>

class window
{
public:
	window();
	window(int windowWidth, int windowHeight);
	int Initialise();

	float getBufferWidth() { return bufferWidth; }
	float getBufferHeight() { return bufferHeight;  }
	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }
	void SwapBuffer() { glfwSwapBuffers(mainWindow); }

	~window();

private:
	GLFWwindow* mainWindow;
	int width, height, bufferWidth, bufferHeight;

};