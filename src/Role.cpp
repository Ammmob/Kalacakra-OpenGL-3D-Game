#include "Role.h"

TextBox* Role::textBox = nullptr;
Role::Role(GLFWwindow* window, Camera* camera, const std::string& modelPath)
	:m_window(window), m_camera(camera) {

	m_scales = 1.0f;
	m_state = 0;
	m_moveSpeed = 1.0f;

	m_talkState = GL_FALSE;
	m_nextActed = GL_FALSE;
	m_orderFinished = GL_FALSE;

	m_talkIndex = 0;
	m_position = glm::vec3(0.0f);
	m_modelMatrix = glm::mat4(1.0f);
	m_rotateParam = glm::vec4(0.0f);
	m_model = new Model(modelPath);
	m_directory = modelPath.substr(0, modelPath.find_last_of('/'));
}

void Role::LoadAnimation() {
	m_stand = new Animation(m_directory + "/" + "stand.dae", m_model);
	m_animator = new Animator(m_stand);
}

void Role::SetPosition(glm::vec3 position) {
	m_position = position;
}

void Role::Move(glm::vec3 direction) {
	m_position += m_moveSpeed * direction;
}

void Role::SetScales(GLfloat scales) {
	m_scales = scales;
}

void Role::SetRotateParam(glm::vec4 param) {
	m_rotateParam = param;
}

glm::vec3 Role::GetPosition() {
	return m_position;
}



void Role::LoadText(const std::string& path) {
	std::ifstream file(path);
	if (!file) {
		std::cout << "ERROR::" << path << " not found" << std::endl;
	}
	else {
		std::stringstream textStream;
		std::string talker, context;
		while (!file.eof()) {
			getline(file, talker);
			getline(file, context);
			m_order.push_back({ talker,context });
		}
	}
}

void Role::LoadLoopText(const std::string& path) {
	std::ifstream file(path);
	if (!file) {
		std::cout << "ERROR::" << path << " not found" << std::endl;
	}
	else {
		std::stringstream textStream;
		std::string talker, context;
		while (!file.eof()) {
			getline(file, talker);
			getline(file, context);
			m_loop.push_back({ talker,context });
		}
	}
}

void Role::CallBack(GLfloat deltaTime) {
	if (GetTalkState()) {
		std::string talker, context;
		if (!m_orderFinished) {
			talker = m_order[m_talkIndex].first;
			context = m_order[m_talkIndex].second;
		}
		else {
			talker = m_loop[m_talkIndex].first;
			context = m_loop[m_talkIndex].second;
		}
		textBox->SetTalker(talker);
		textBox->SetContext(context);
		textBox->Draw();
		if (m_nextActed) {
			if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_RELEASE &&
				glfwGetKey(m_window, GLFW_KEY_ENTER) == GLFW_RELEASE) {
				m_nextActed = GL_FALSE;
				m_talkIndex++;
				if (!m_orderFinished && m_talkIndex == (int)m_order.size()) {
					m_talkIndex = 0;
					textBox->SetOpenState(GL_FALSE);
					m_talkState = GL_FALSE;
					m_orderFinished = GL_TRUE;
				}
				else if (m_talkIndex == (int)m_loop.size()) {
					m_talkIndex = 0;
					textBox->SetOpenState(GL_FALSE);
					m_talkState = GL_FALSE;
				}
			}
		}
		else {
			if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS ||
				glfwGetKey(m_window, GLFW_KEY_ENTER) == GLFW_PRESS) {
				m_nextActed = GL_TRUE;
			}
		}
	}

	m_animator->UpdateAnimation(deltaTime);
}
void Role::Draw(Shader& shader) {
	m_modelMatrix = glm::translate(glm::mat4(1.0f), m_position);
	glm::vec3 axis = glm::vec3(m_rotateParam.y, m_rotateParam.z, m_rotateParam.w);
	if (m_rotateParam.x != 0) {
		m_modelMatrix = glm::rotate(m_modelMatrix, m_rotateParam.x, axis);
	}
	m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(m_scales));

	shader.Bind();
	shader.SetUniform1i("isBlinn", 1);
	shader.SetUniform1i("isPlay", 1);

	shader.SetUniformMatrix4fv("u_model", m_modelMatrix);
	shader.SetUniformMatrix4fv("u_view", m_camera->GetView());
	shader.SetUniformMatrix4fv("u_projection", m_camera->GetProjection());
	std::vector<glm::mat4> transforms = m_animator->GetFinalBoneMatrices();
	for (int i = 0; i < transforms.size(); ++i)
		shader.SetUniformMatrix4fv("finalBonesMatrices[" + std::to_string(i) + "]", glm::value_ptr(transforms[i]));
	m_camera->DrawLight(shader);
	m_model->Draw(shader);

	shader.Bind();
	shader.SetUniform1i("isBlinn", 0);
	shader.SetUniform1i("isPlay", 0);
	shader.UnBind();
}

std::vector<sstring> Role::GetLoop() {
	return m_loop;
}

std::vector<sstring> Role::GetOrder() {
	return m_order;
}



void Role::Talk() {
	if (!GetTalkState()) {
		m_talkState = GL_TRUE;
		textBox->SetOpenState(GL_TRUE);
	}
}

GLboolean Role::GetTalkState() {
	return m_talkState;
}

glm::mat4 Role::GetModelMatrix() {
	return m_modelMatrix;
}

void Role::aStand() {
	m_animator->PlayAnimation(m_stand);
}
