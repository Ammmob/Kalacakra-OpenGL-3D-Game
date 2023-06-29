#include "Camera.h"
#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 wordUp, GLfloat pitch, GLfloat yaw, GLfloat roll)
	: m_position(position), m_wordUp(wordUp), m_pitch(pitch), m_yaw(yaw), m_roll(roll), m_moveSpeed(4.0f), m_mouseSensitivity(0.005f), m_zoom(45) {
	Update();
	m_firstIn = GL_TRUE;
	isSpotLight = GL_FALSE;
}

Camera::~Camera() {
}

void Camera::SetWidthHeight(GLint width, GLint height) {
	windowWidth = (GLfloat)width;
	windowHeight = (GLfloat)height;
	m_lastX = windowWidth / 2.0f;
	m_lastY = windowHeight / 2.0f;
}

glm::mat4 Camera::GetView() {
	return glm::lookAt(m_position, m_position + m_forward, m_up);
}

glm::mat4 Camera::GetProjection(GLfloat zNer, GLfloat zFar) {
	return glm::perspective(glm::radians(m_zoom), windowWidth / windowHeight, zNer, zFar);
}

glm::vec3 Camera::GetPosition() {
	return m_position;
}

glm::vec3 Camera::GetForward() {
	return m_forward;
}

glm::vec3 Camera::GetRight() {
	return m_right;
}

glm::vec3 Camera::GetUp() {
	return m_up;
}


GLfloat Camera::GetZoom() {
	return m_zoom;
}

GLfloat Camera::GetPitch() {
	return m_pitch;
}

GLfloat Camera::GetYaw() {
	return m_yaw;
}

GLfloat Camera::GetRoll() {
	return m_roll;
}

void Camera::MoveForward(GLfloat deltaTime) {
	glm::vec3 delta = m_forward * deltaTime * m_moveSpeed;
	delta.y = 0;
	m_position += delta;
	Update();
}

void Camera::MoveBack(GLfloat deltaTime) {
	glm::vec3 delta = m_forward * deltaTime * m_moveSpeed;
	delta.y = 0;
	m_position -= delta;
	Update();
}

void Camera::MoveRight(GLfloat deltaTime) {
	glm::vec3 delta = m_right * deltaTime * m_moveSpeed;
	delta.y = 0;
	m_position += delta;
	Update();
}

void Camera::MoveLeft(GLfloat deltaTime) {
	glm::vec3 delta = m_right * deltaTime * m_moveSpeed;
	delta.y = 0;
	m_position -= delta;
	Update();
}

void Camera::MoveUp(GLfloat deltaTime) {
	m_position += m_up * deltaTime * m_moveSpeed;
	Update();
}

void Camera::MoveDown(GLfloat deltaTime) {
	m_position -= m_up * deltaTime * m_moveSpeed;
	Update();
}


glm::vec3 Camera::GetMovedForward(GLfloat deltaTime) {
	glm::vec3 delta = m_forward * deltaTime * m_moveSpeed;
	delta.y = 0;
	return m_position + delta;
}

glm::vec3 Camera::GetMovedBack(GLfloat deltaTime) {
	glm::vec3 delta = -m_forward * deltaTime * m_moveSpeed;
	delta.y = 0;
	return m_position + delta;
}

glm::vec3 Camera::GetMovedRight(GLfloat deltaTime) {
	glm::vec3 delta = m_right * deltaTime * m_moveSpeed;
	delta.y = 0;
	return m_position + delta;
}

glm::vec3 Camera::GetMovedLeft(GLfloat deltaTime) {
	glm::vec3 delta = -m_right * deltaTime * m_moveSpeed;
	delta.y = 0;
	return m_position + delta;
}

glm::vec3 Camera::GetMovedUp(GLfloat deltaTime) {
	glm::vec3 delta = m_up * deltaTime * m_moveSpeed;
	delta.y = 0;
	return m_position + delta;
}

glm::vec3 Camera::GetMovedDown(GLfloat deltaTime) {
	glm::vec3 delta = -m_up * deltaTime * m_moveSpeed;
	delta.y = 0;
	return m_position + delta;
}

void Camera::MoveView(GLfloat offsetX, GLfloat offsetY) {
	offsetX *= m_mouseSensitivity;
	offsetY *= m_mouseSensitivity;

	m_pitch += offsetY;
	m_yaw += offsetX;

	if (m_pitch > glm::radians(45.0f))
		m_pitch = glm::radians(45.0f);
	if (m_pitch < glm::radians(-60.0f))
		m_pitch = glm::radians(-60.0f);

	Update();
}

void Camera::MouseScroll(GLfloat offset) {
	m_zoom += offset;
	if (m_zoom > 45.0f) {
		m_zoom = 45.0f;
	}
	if (m_zoom < 1.0f) {
		m_zoom = 1.0f;
	}
}

void Camera::SetPosition(glm::vec3 position) {
	m_position = position;
}

void Camera::SetMoveSpeed(GLfloat speed) {
	m_moveSpeed = speed;
}


void Camera::SetMouseSensitivity(GLfloat sensitivity) {
	m_mouseSensitivity = sensitivity;
}


void Camera::Update() {
	GLfloat x, y, z;
	x = glm::cos(m_pitch) * glm::sin(m_yaw);
	y = -glm::sin(m_pitch);
	z = glm::cos(m_pitch) * glm::cos(m_yaw);

	/*x = glm::cos(m_pitch) * glm::sin(m_yaw) * glm::cos(m_roll) + glm::sin(m_pitch) * glm::sin(m_roll);
	y = -glm::sin(m_pitch) * glm::cos(m_roll) + glm::cos(m_pitch) * glm::sin(m_yaw) * glm::sin(m_roll);
	z = glm::cos(m_pitch) * glm::cos(m_yaw);*/

	m_forward = glm::normalize(glm::vec3(x, y, z));
	m_right = glm::normalize(glm::cross(m_forward, m_wordUp));
	m_up = glm::normalize(cross(m_right, m_forward));
}

void Camera::TurnSpotLight() {
	isSpotLight = 1 - isSpotLight;
}

bool Camera::GetSpotLightState() {
	return isSpotLight;
}

void Camera::DrawLight(Shader& shader) {
	shader.Bind();

	shader.SetUniform3f("u_view_position", m_position);
	shader.SetUniform1f("shininess", 32.0f);

	shader.SetUniform3f("u_dirLight.ambient", 0.2f, 0.2f, 0.2f);
	shader.SetUniform3f("u_dirLight.diffuse", 1.5f, 1.5f, 1.5f);
	shader.SetUniform3f("u_dirLight.specular", 1.0f, 1.0f, 1.0f);
	shader.SetUniform3f("u_dirLight.direction", -1.0f, -1.0f, -1.0f);

	shader.SetUniform1i("isSpotLight", isSpotLight);
	shader.SetUniform3f("u_spotLight.position", m_position - glm::vec3(0.0f, 0.6f, 0.8f));
	shader.SetUniform3f("u_spotLight.direction", m_forward);
	shader.SetUniform3f("u_spotLight.ambient", 0.2f, 0.2f, 0.2f);
	shader.SetUniform3f("u_spotLight.diffuse", 1.4f, 1.4f, 1.4f);
	shader.SetUniform3f("u_spotLight.specular", 1.0f, 1.0f, 1.0f);
	shader.SetUniform1f("u_spotLight.constant", 1.0f);
	shader.SetUniform1f("u_spotLight.linear", 0.09f);
	shader.SetUniform1f("u_spotLight.quadratic", 0.032f);
	shader.SetUniform1f("u_spotLight.innerCutOff", glm::cos(glm::radians(12.5f)));
	shader.SetUniform1f("u_spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

	shader.UnBind();
}


void Camera::ProcessKeyInput(GLFWwindow* window, GLfloat deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		deltaTime *= 3;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		MoveForward(deltaTime);

	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		MoveBack(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		MoveRight(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		MoveLeft(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		MoveUp(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		MoveDown(deltaTime);
	}
}

void Camera::ProcessCursorMove(GLFWwindow* window, GLboolean enable) {
	GLdouble posX, posY;
	glfwGetCursorPos(window, &posX, &posY);
	if (m_firstIn) {
		m_lastX = posX;
		m_lastY = posY;
		m_firstIn = GL_FALSE;
	}
	GLfloat offsetX = GLfloat(posX - m_lastX);
	GLfloat offsetY = GLfloat(m_lastY - posY);

	m_lastX = posX;
	m_lastY = posY;
	if (enable) {
		MoveView(offsetX, offsetY);
	}
}