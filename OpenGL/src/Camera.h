#pragma once
#include <GL/glew.h>

#include <glfw3.h>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();

	Camera(glm::vec3 startPosition, glm::vec3 startUp, float startYaw, 
		   float startPitch, float startMoveSpeed, float startTurnSpeed);

	~Camera();

	void KeyControl(bool* keys, float deltaTime);
	void MouseControl(float xChange, float yChange);

	glm::mat4 getViewMatrix();
	glm::vec3 getCameraPositon();

private:
	glm::vec3 Position;
	glm::vec3 front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 worldUp;

	float yaw,pitch;
	
	float moveSpeed,turnSpeed;

	void update();
};