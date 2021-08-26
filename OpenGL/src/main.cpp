#define STB_IMAGE_IMPLEMENTATION


#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "window.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "CommonValues.h"
#include "PointLight.h"

//Conversion 
const float toRadians = 3.14159265f / 180.0f;

float deltaTime = 0.0f;
float lastTime = 0.0f;

glm::vec3 positionVector = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
float yaw = -90.0f;
float pitch = 0.0f;
float moveSpeed = 4.0f;
float sensitivity = 0.5f;

window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;
Material material;
Material material2;

Texture brickTexture;
Texture dirtTexture;


DirectionalLight mainLight;
PointLight pointLight[MAX_POINT_LIGHTS];

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void calculateAverageNormals(unsigned int* indices, unsigned int indiceCount, float *vertices,
							 unsigned int verticesCount, unsigned int vLenght, unsigned int normalOffset)
{
	for (unsigned int i = 0; i < indiceCount; i+= 3)
	{
		unsigned int in0 = indices[i] * vLenght;
		unsigned int in1 = indices[i + 1] * vLenght;
		unsigned int in2 = indices[i + 2] * vLenght;

		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);

		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;

		vertices[in0] += normal.x;
		vertices[in0 + 1] += normal.y;
		vertices[in0 + 2] += normal.z;

		vertices[in1] += normal.x;
		vertices[in1 + 1] += normal.y;
		vertices[in1 + 2] += normal.z;

		vertices[in2] += normal.x;
		vertices[in2 + 1] += normal.y;
		vertices[in2 + 2] += normal.z;
	}

	for (unsigned int i = 0; i < verticesCount / vLenght; i++)
	{
		unsigned int nOffset = i * vLenght + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);

		vertices[nOffset] = vec.x;
		vertices[nOffset + 1] = vec.y;
		vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3,
	};

	float vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
		   -1.0f, -1.0f,  -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f,   1.0f,	0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f,  -0.6f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f,  1.0f,   0.0f,	0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	float floorVertices[] = {
		-10.0f, 0.0f, -10.0f,  0.0f, 0.0f,   0.0f, -1.0f, 0.0f,
		 10.0f, 0.0f, -10.0f,  10.0f,0.0f,   0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,   0.0f, 10.0f,  0.0f, -1.0f, 0.0f,
		 10.0f, 0.0f, 10.0f,   10.0f,10.0f,  0.0f, -1.0f, 0.0f,
	};

	calculateAverageNormals(indices, 12, vertices, 32, 8, 5);

	
	Mesh *mesh = new Mesh();
	mesh->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(mesh);

	Mesh* mesh2 = new Mesh();
	mesh2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(mesh2);

	Mesh* floor = new Mesh();
	floor->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(floor);
}


void CreateShaders()
{
	Shader* shader = new Shader;
	shader->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader);
}

int main()
{
	mainWindow = window(1366, 786);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(positionVector, upVector, yaw, pitch, moveSpeed, sensitivity);

	brickTexture = Texture("resources/brick.png");
	brickTexture.LoadTexture();

	dirtTexture = Texture("resources/dirt.png");
	dirtTexture.LoadTexture();

	

	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
							     0.23f, 0.3f, 0.0f,
								 0.0f, -1.0f);

	unsigned int pointLightCount = 0;
	pointLight[0] = PointLight(0.0f, 0.0f, 1.0f,
							   0.0f, 1.0f,
							   0.0f, 0.0f, 0.0f,
							   0.3f, 0.2f, 0.1f);
	pointLightCount++;
	pointLight[1] = PointLight(0.0f, 1.0f, 0.0f,
							   0.0f, 1.0f,
							  -4.0f, 2.0f, 0.0f,
							   0.3f, 0.1f, 0.1f);
	pointLightCount++;

	material = Material(1.0f, 32.0f);
	material2 = Material(0.5f, 4.0f);

	unsigned int uniformProjection = 0, uniformModel = 0, uniformView = 0,
				 uniformSpecularIntensity = 0, uniformShiniess = 0, uniformEyePosition = 0;

	glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		// Get + Handle user input events
		glfwPollEvents();

		camera.KeyControl(mainWindow.getKeys(), deltaTime);
		camera.MouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShiniess = shaderList[0].GetShininessLocation();

		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLight, pointLightCount);
		
		

		//Camera Position
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPositon().x, camera.getCameraPositon().y, camera.getCameraPositon().z);


		//Triangle 1
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, -2.5f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brickTexture.UseTexture();
		material.useMaterial(uniformSpecularIntensity, uniformShiniess);
		meshList[0]->RenderMesh();

		
		//Floor
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.UseTexture();
		material.useMaterial(uniformSpecularIntensity, uniformShiniess);
		meshList[2]->RenderMesh();


		glUseProgram(0);
		mainWindow.SwapBuffer();
	}

	return 0;
}