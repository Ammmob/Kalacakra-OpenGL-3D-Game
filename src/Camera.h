#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

#define PI glm::acos(-1)


class Camera {
private:
	glm::vec3 m_position;
	glm::vec3 m_forward;
	glm::vec3 m_right;
	glm::vec3 m_up;
	glm::vec3 m_wordUp;
	GLfloat m_pitch;
	GLfloat m_yaw;
	GLfloat m_roll;
	GLfloat m_moveSpeed;
	GLfloat m_mouseSensitivity;
	GLfloat m_zoom;

	GLdouble m_lastX;
	GLdouble m_lastY;
	GLboolean m_firstIn;

	GLfloat windowWidth;
	GLfloat windowHeight;

	GLboolean isSpotLight;
	void Update();
public:
	Camera(glm::vec3 position, glm::vec3 wordUp = glm::vec3(0.0f, 1.0f, 0.0f),
		GLfloat pitch = glm::radians(30.0f), GLfloat yaw = -PI, GLfloat roll = 0.0f);
	~Camera();
	void SetWidthHeight(GLint width, GLint height);
	glm::mat4 GetView();
	glm::mat4 GetProjection(GLfloat zNer = 0.1f, GLfloat zFar = 100.0f);
	glm::vec3 GetPosition();
	glm::vec3 GetForward();
	glm::vec3 GetRight();
	glm::vec3 GetUp();
	GLfloat GetZoom();
	GLfloat GetPitch();
	GLfloat GetYaw();
	GLfloat GetRoll();


	void MoveForward(GLfloat deltaTime);
	void MoveBack(GLfloat deltaTime);
	void MoveRight(GLfloat deltaTime);
	void MoveLeft(GLfloat deltaTime);
	void MoveUp(GLfloat deltaTime);
	void MoveDown(GLfloat deltaTime);
	void MoveView(GLfloat offsetX, GLfloat offsetY);
	void MouseScroll(GLfloat offset);

	glm::vec3 GetMovedForward(GLfloat deltaTime);
	glm::vec3 GetMovedBack(GLfloat deltaTime);
	glm::vec3 GetMovedRight(GLfloat deltaTime);
	glm::vec3 GetMovedLeft(GLfloat deltaTime);
	glm::vec3 GetMovedUp(GLfloat deltaTime);
	glm::vec3 GetMovedDown(GLfloat deltaTime);

	void SetPosition(glm::vec3 position);
	void SetMoveSpeed(GLfloat speed);
	void SetMouseSensitivity(GLfloat sensitivity);


	// process input
	void ProcessKeyInput(GLFWwindow* window, GLfloat deltaTime);
	void ProcessCursorMove(GLFWwindow* window, GLboolean enable = GL_TRUE);

	void TurnSpotLight();
	bool GetSpotLightState();
	void DrawLight(Shader& shader);

};

