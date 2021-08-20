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

	bool* getKeys(){ return keys; }

	float getXChange();
	float getYChange();

	~window();

private:
	GLFWwindow* mainWindow;
	int width, height, bufferWidth, bufferHeight;
	
	bool keys[1024];

	float lastX, lastY, xChange, yChange;

	bool mouseFirstMoved;

	void createCallbacks();
	static void handlekeys(GLFWwindow *Window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* Window, double xPos, double yPos);
};