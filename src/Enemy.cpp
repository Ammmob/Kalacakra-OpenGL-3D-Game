#include "Enemy.h"

Enemy::Enemy(GLFWwindow* window, Camera* camera, const std::string& modelPath)
	: Role(window, camera, modelPath) {
	m_HP = 300.0f;
	m_FHP = 300.0f;
	m_ATK = 30.0f;
	m_DEF = 20.0f;
	m_collideRadius = 2.0f;
	m_attackRaius = 4.0f;

	m_attackDirection = glm::vec3(0.0f);
	m_aState = 0;
	m_moveSpeed = 0.8;

	m_battleState = GL_FALSE;

	srand((unsigned long long)time(0));

}

void Enemy::LoadAnimation() {
	m_stand = new Animation(m_directory + "/" + "stand.dae", m_model);
	m_injured = new Animation(m_directory + "/" + "injured.dae", m_model);
	m_kick = new Animation(m_directory + "/" + "kick.dae", m_model);
	m_swip = new Animation(m_directory + "/" + "swip.dae", m_model);
	m_forward = new Animation(m_directory + "/" + "forward.dae", m_model);
	m_animator = new Animator(m_stand);
}

void Enemy::UpdateAttackParam() {
	Animation* play = m_animator->GetCurrentAnimation();
	if (play == m_swip) {
		m_ATK = 30;
	}
	else if (play == m_kick) {
		m_ATK = 50;
	}
	else {
		m_ATK = 0;
	}
}

GLfloat Enemy::GetATK() {
	return m_ATK;
}

GLboolean Enemy::GetSingleAttackState() {
	return m_singleAttackState;
}

GLboolean Enemy::GetBattleState() {
	return m_battleState;
}

GLboolean Enemy::GetInCastleState() {
	return !m_battleState && m_talkIndex >= 8;
}

GLfloat Enemy::GetRateHP() {
	return m_HP / m_FHP;
}

glm::vec3 Enemy::GetTargetDirection() {
	glm::vec3 direction = glm::normalize(m_targetPosition - m_position);
	direction.y = 0;
	return direction;
}

bool Enemy::TargetDistanceJudgement(GLfloat radius) {
	glm::vec3 tmp = m_position - m_targetPosition;
	tmp.y = 0;
	GLfloat distance = glm::l2Norm(tmp);
	return distance <= radius;
}

void Enemy::SetSingleAttackState(GLboolean state) {
	m_singleAttackState = state;
}

void Enemy::SetTargetPosition(glm::vec3 position) {
	m_targetPosition = position;
}

void Enemy::Talk() {
	if (!GetTalkState() && !m_orderFinished) {
		m_talkState = GL_TRUE;
		textBox->SetOpenState(GL_TRUE);
	}
}
void Enemy::Battle() {
	if (TargetDistanceJudgement(m_attackRaius)) {
		if (m_aState == 0 || m_animator->GetCurrentAnimation() == m_forward) {
			m_aState = 50;
			SetSingleAttackState(GL_FALSE);
			if (GetRateHP() >= 0.3) {
				aSwip();
			}
			else {
				aKick();
			}
		}
	}
	else {
		if (m_aState == 0) {
			m_aState = 50;
			aForward();
		}
	}
	m_aState--;
	if (m_animator->GetCurrentAnimation() == m_forward &&
		!TargetDistanceJudgement(m_collideRadius)) {
		if (m_aState < 20) {
			Move(glm::vec3(0.5f) * GetTargetDirection());
		}
		else {
			Move(GetTargetDirection());
		}
	}
}

void Enemy::CallBack(GLfloat deltaTime) {
	if (GetTalkState() && !m_orderFinished) {
		std::string talker = m_order[m_talkIndex].first;
		std::string context = m_order[m_talkIndex].second;
		textBox->SetTalker(talker);
		textBox->SetContext(context);
		textBox->Draw();
		if (m_nextActed) {
			if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_RELEASE &&
				glfwGetKey(m_window, GLFW_KEY_ENTER) == GLFW_RELEASE) {
				m_nextActed = GL_FALSE;
				m_talkIndex++;
				if (m_talkIndex == 8) {
					textBox->SetOpenState(GL_FALSE);
					m_talkState = GL_FALSE;
					m_battleState = GL_TRUE;
				}
				else if (m_talkIndex == (int)m_order.size()) {
					textBox->SetOpenState(GL_FALSE);
					m_talkState = GL_FALSE;
					m_orderFinished = GL_TRUE;
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
	else {
		UpdateAttackParam();
		if (m_state != 0) {
			m_state--;
			m_position -= glm::vec3(0.05f) * m_attackDirection;
			if (m_state == 0) {
				aStand();
			}
		}
		else if (m_battleState) {
			Battle();
		}
	}
	m_animator->UpdateAnimation(deltaTime);
}

void Enemy::Draw(Shader& shader) {
	if (m_talkIndex <= 0)return;
	if (!m_battleState && m_talkIndex >= 8) {
		m_position = glm::vec3(0.0f, -2.0f, -70.0f);
		return;
	}
	m_modelMatrix = glm::translate(glm::mat4(1.0f), m_position);
	if (m_battleState) {
		glm::vec3 direction = GetTargetDirection();
		GLfloat theta = glm::atan(direction.x, direction.z);
		m_modelMatrix = glm::rotate(m_modelMatrix, theta, glm::vec3(0.0f, 1.0f, 0.0f));
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

void Enemy::aInjured() {
	m_animator->PlayAnimation(m_injured);
}

void Enemy::aKick() {
	m_animator->PlayAnimation(m_kick);
}

void Enemy::aSwip() {
	m_animator->PlayAnimation(m_swip);
}

void Enemy::aForward() {
	m_animator->PlayAnimation(m_forward);
}

void Enemy::Injured(GLfloat ATK) {
	if (m_battleState && m_state == 0) {
		m_state = 30;
		m_aState = 0;
		GLfloat harm = glm::max(0.0f, ATK - m_DEF);
		m_HP -= harm;
		if (m_HP <= 0) {
			m_battleState = GL_FALSE;
			Talk();
			textBox->SetOpenState(GL_TRUE);
			m_HP = 0;
		}
		aInjured();
	}
}