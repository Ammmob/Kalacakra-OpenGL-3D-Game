#pragma once

#include <sstream>
#include <fstream>
#include "Animator.h"
#include "Camera.h"
#include "TextBox.h"

typedef std::pair<std::string, std::string> sstring;

class Role {
public:
	Role(GLFWwindow* window, Camera* camera, const std::string& initAnimationPath);
	void LoadAnimation();

	// set attribute
	void SetPosition(glm::vec3 position);
	void Move(glm::vec3 direction);
	void SetScales(GLfloat scales);
	void SetRotateParam(glm::vec4 param);

	// get attribute
	glm::vec3 GetPosition();
	glm::mat4 GetModelMatrix();

	void aStand();

	void LoadText(const std::string& path);
	void LoadLoopText(const std::string& path);
	void CallBack(GLfloat deltaTime);
	void Draw(Shader& shader);
	std::vector<sstring> GetLoop();
	std::vector<sstring> GetOrder();

	void Talk();
	GLboolean GetTalkState();

	static TextBox* textBox;

protected:
	GLFWwindow* m_window;
	Model* m_model;
	Animator* m_animator;
	Animation* m_stand;

	Camera* m_camera;

	GLfloat m_moveSpeed;
	GLfloat m_scales;
	glm::mat4 m_modelMatrix;
	glm::vec3 m_position;
	glm::vec4 m_rotateParam;

	std::vector<sstring> m_loop;
	std::vector<sstring> m_order;
	GLboolean m_orderFinished;
	GLboolean m_talkState;


	GLint m_state;

	GLint m_talkIndex;
	GLboolean m_nextActed;
	std::string m_directory;
};