#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, float startYaw, float startPitch,
			   float startMoveSpeed, float startTurnSpeed)
{
	Position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;

	front = glm::vec3(0.0f, 0.f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	update();
}

Camera::~Camera()
{
}

void Camera::KeyControl(bool *keys, float deltaTime)
{
	if (keys[GLFW_KEY_W])
	{
		Position += front * moveSpeed * deltaTime;
	}

	if (keys[GLFW_KEY_S])
	{
		Position -= front * moveSpeed * deltaTime;
	}

	if (keys[GLFW_KEY_A])
	{
		Position -= Right * moveSpeed * deltaTime;
	}

	if (keys[GLFW_KEY_D])
	{
		Position += Right * moveSpeed * deltaTime;
	}

	if (keys[GLFW_KEY_Q])
	{
		Position += Up * moveSpeed * deltaTime;
	}

	if (keys[GLFW_KEY_E])
	{
		Position -= Up * moveSpeed * deltaTime;
	}
}

void Camera::MouseControl(float xChange, float yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update();
}

glm::mat4 Camera::getViewMatrix()
{
	//(position + front) for general direction 
	return glm::lookAt(Position, Position + front, Up);
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(front);

	//recalculate right and up vectors
	Right = glm::normalize(glm::cross(front, worldUp));
	Up = glm::normalize(glm::cross(Right, front));
}
