#include "window.h"

window::window()
{
	width = 800;
	height = 600;

	xChange = 0.0f;
	yChange = 0.0f;

	for (unsigned int i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

window::window(int windowWidth, int windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	xChange = 0.0f;
	yChange = 0.0f;

	for (unsigned int i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

int window::Initialise()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		std::cout << "GLFW initialisation failed !" << std::endl;
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core Profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow Forward Compatbility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create the window
	mainWindow = glfwCreateWindow(width, height, "OpenGL-Renderer Test", NULL, NULL);
	if (!mainWindow)
	{
		std::cout << "GLFW window creation failed " << std::endl;
		glfwTerminate();
		return 1;
	}

	// Get Buffer Size information
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	//Handle key and mouse
	createCallbacks();

	//lock the mouse
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Glew initialisation failed ! " << std::endl;
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	// Setup Viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);
	
	glfwSetWindowUserPointer(mainWindow, this);
}

void window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, handlekeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}

void window::handlekeys(GLFWwindow *Window, int key, int code, int action, int mode)
{
	window *theWindow = static_cast<window*>(glfwGetWindowUserPointer(Window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(Window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;	
		}

		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
		}
	}
}

void window::handleMouse(GLFWwindow* Window, double xPos, double yPos)
{
	window* theWindow = static_cast<window*>(glfwGetWindowUserPointer(Window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;

	//to prevent invert y-mouse controls
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}

float window::getXChange()
{
	float theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

float window::getYChange()
{
	float theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

window::~window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}


