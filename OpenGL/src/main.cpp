#include "Engine.h"


const float toRadians = 3.14159265f / 180.0f;

unsigned int uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
			 uniformSpecularIntensity = 0, uniformShininess = 0,
			 uniformDirectionalLightTransform = 0, uniformOmniLightPos = 0, uniformFarPlane = 0;

window mainWindow;
std::vector<Mesh*> meshList;

std::vector<Shader> shaderList;
Shader directionalShadowShader;
Shader OmniShadowShader;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;

Material shinyMaterial;
Material mediumMaterial;
Material dullMaterial;

Model xwing;
Model blackhawk;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

Skybox skybox;


unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

float deltaTime = 0.0f;
float lastTime = 0.0f;

float blackhawkAngle = 0.0f;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, float* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (unsigned int i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (unsigned int i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
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

	float vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	float floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);
	
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);
}

//Shader Files for Shadow Map
void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);


	directionalShadowShader.CreateFromFiles("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");

	OmniShadowShader.CreateFromFiles("Shaders/omni_shadow_map.vert","Shaders/omni_shadow_map.geom", "Shaders/omni_shadow_map.frag");
}

void RenderScene()
{
	glm::mat4 model(1.0f);

	//Brick Triangle
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//brickTexture.UseTexture();
	//dullMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
	//meshList[0]->RenderMesh();

	//Floor 
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dirtTexture.UseTexture();
	mediumMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	//xWing Model
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 10.0f));
	model = glm::scale(model, glm::vec3(0.006f, 0.006f, 0.006f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
	xwing.RenderModel();

	blackhawkAngle -= 0.1f;
	if (blackhawkAngle > 360.0f)
	{
		blackhawkAngle = 0.1f;
	}
	
	// BlackHaw model
	model = glm::mat4(1.0f);
	//model = glm::rotate(model, blackhawkAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 1.0f));
	//model = glm::rotate(model, 20.0f * toRadians, glm::vec3(0.0f, 0.0f, -1.0f));
	model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	mediumMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
	blackhawk.RenderModel();
}

void DirectionalShadowMapPass(DirectionalLight* light)
{
	directionalShadowShader.UseShader();

	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = directionalShadowShader.GetModelLocation();
	glm::mat4 lightTransform = light->CalculateLightTransform();
	directionalShadowShader.SetDirectionalLightTransform(&lightTransform);

	directionalShadowShader.Validate();
	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmniShadowMapPass(PointLight* light)
{
	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	OmniShadowShader.UseShader();
	uniformModel = OmniShadowShader.GetModelLocation();
	uniformOmniLightPos = OmniShadowShader.GetOmniLightPosLocation();
	uniformFarPlane = OmniShadowShader.GetFarPlaneLocation();

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
	glUniform1f(uniformFarPlane, light->GetFarPlane());
	OmniShadowShader.SetLightMatrices(light->CalculateLightTransform());

	OmniShadowShader.Validate();
	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Get Uniforms From Shader Code
void RenderPass(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	glViewport(0, 0, 1366, 768);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	skybox.DrawSkybox(viewMatrix, projectionMatrix);

	shaderList[0].UseShader();

	uniformModel = shaderList[0].GetModelLocation();
	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformView = shaderList[0].GetViewLocation();
	uniformModel = shaderList[0].GetModelLocation();
	uniformEyePosition = shaderList[0].GetEyePositionLocation();
	uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
	uniformShininess = shaderList[0].GetShininessLocation();

	

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

	shaderList[0].SetDirectionalLight(&mainLight);
	shaderList[0].SetPointLights(pointLights, pointLightCount, 3, 0);
	shaderList[0].SetSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);

	glm::mat4 lightTransform = mainLight.CalculateLightTransform();
	shaderList[0].SetDirectionalLightTransform(&lightTransform);

	mainLight.GetShadowMap()->Read(GL_TEXTURE2);
	shaderList[0].SetTexture(1);
	shaderList[0].SetDirectionalShadowMap(2);

	glm::vec3 lowerLight = camera.getCameraPosition();
	lowerLight.y -= 0.3f;
	spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

	shaderList[0].Validate();
	RenderScene();
}

void CalculateOmniShadowmapPass()
{
	for (unsigned int i = 0; i < pointLightCount; i++)
	{
		OmniShadowMapPass(&pointLights[i]);
	}

	for (unsigned int i = 0; i < spotLightCount; i++)
	{
		OmniShadowMapPass(&spotLights[i]);
	}
}

void LoadSkybox1()
{
	std::vector<std::string> skyboxFaces;

	//It has to be in that order
	skyboxFaces.push_back("Textures/skybox2/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/skybox2/cupertin-lake_lf.tga");

	skyboxFaces.push_back("Textures/skybox2/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/skybox2/cupertin-lake_dn.tga");

	skyboxFaces.push_back("Textures/skybox2/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/skybox2/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);
}


int main()
{
	mainWindow = window(1366, 768); 
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();

	shinyMaterial = Material(4.0f, 256);
	mediumMaterial = Material(2.0f, 128);
	dullMaterial = Material(0.3f, 4);

	xwing = Model();
	xwing.LoadModel("Models/x-wing.obj");

	blackhawk = Model();
	blackhawk.LoadModel("Models/uh60.obj");

	//Rendering mainLight, SpotLight, PointLight 

	mainLight = DirectionalLight(2048, 2048,
								0.8f, 0.52f, 0.0f,
								0.02f, 0.6f,
								0.0f, -15.0f, -10.0f);

	pointLights[0] = PointLight(1024,1024,
							    0.01f, 100.0f,
								0.0f, 0.0f, 1.0f,
								0.01f, 0.001f,
								1.0f, 2.0f, 0.0f,
								0.02f, 0.01f, 0.01f);

	//pointLightCount++;

	pointLights[1] = PointLight(1024,1024,
								0.1f, 100.0f,
								0.0f, 1.0f, 0.0f,
								0.01f, 0.001f,
								-4.0f, 3.0f, 0.0f,
								0.02f, 0.01f, 0.01f);
	//pointLightCount++;


	spotLights[0] = SpotLight(1024, 1024,
							  0.01f, 100.0f,
							  1.0f, 1.0f, 1.0f,
							  0.0f, 2.0f,
							  0.0f, 0.0f, 0.0f,
							  0.0f, -1.0f, 0.0f,
							  1.0f, 0.0f, 0.0f,
							  20.0f);

	//spotLightCount++;

	spotLights[1] = SpotLight(1024, 1024,
							  0.01f, 100.0f,
							  1.0f, 1.0f, 1.0f,
							  0.0f, 0.3f,
							  0.0f, -1.5f, 0.0f,
							 -100.0f, -1.0f, 0.0f,
							  1.0f, 0.0f, 0.0f,
							  20.0f);
	spotLightCount++;

	LoadSkybox1();

	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		//Delta time
		float now = glfwGetTime(); 
		deltaTime = now - lastTime; 
		lastTime = now;

		// Get + Handle User Input
		glfwPollEvents();

		camera.MouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		camera.KeyControl(mainWindow.getKeys(), deltaTime);

		if (mainWindow.getKeys()[GLFW_KEY_L])
		{
			spotLights[0].Toggle();
			mainWindow.getKeys()[GLFW_KEY_L] = false;
		}


		DirectionalShadowMapPass(&mainLight);

		CalculateOmniShadowmapPass();

		RenderPass(camera.getViewMatrix(), projection);

		mainWindow.SwapBuffer();
	}

	return 0;
}