#include "Player.h"

TextBox* Player::textBox = nullptr;

Player::Player(GLFWwindow* window, Camera* cameara, const std::string& modelPath)
	: m_window(window), m_camera(cameara) {
	m_HP = 100.0f;
	m_FHP = 100.0f;
	m_SP = 100.0f;
	m_FSP = 100.0f;
	m_ATK = 50.0f;
	m_DEF = 20.0f;
	m_state = 0;
	m_scales = 1.0f;

	m_talkState = GL_FALSE;
	m_runState = GL_FALSE;
	m_battleState = GL_FALSE;
	m_singleAttackState = GL_FALSE;
	m_fixedState = GL_FALSE;
	m_talkRadius = 5.0f;
	m_collideRadius = 2.0f;
	m_attackRadius = 5.0f;
	m_attackAngle = glm::cos(glm::radians(25.0f));

	m_model = new Model(modelPath);
	m_directory = modelPath.substr(0, modelPath.find_last_of('/'));

}

void Player::LoadAnimation() {
	m_sleep = new Animation(m_directory + "/" + "sleep.dae", m_model);
	m_stand = new Animation(m_directory + "/" + "stand.dae", m_model);
	m_walk = new Animation(m_directory + "/" + "walk.dae", m_model);
	m_slowRun = new Animation(m_directory + "/" + "slowRun.dae", m_model);
	m_fastRun = new Animation(m_directory + "/" + "fastRun.dae", m_model);
	m_punch = new Animation(m_directory + "/" + "punch.dae", m_model);
	m_animator = new Animator(m_sleep);
}

void Player::aStand() {
	m_animator->PlayAnimation(m_stand);
}

void Player::aWalk() {
	m_animator->PlayAnimation(m_walk);
}

void Player::aSlowRun() {
	m_animator->PlayAnimation(m_slowRun);
}

void Player::aFastRun() {
	m_animator->PlayAnimation(m_fastRun);
}

void Player::aPunch() {
	m_animator->PlayAnimation(m_punch);
}

void Player::aSleep() {
	m_animator->PlayAnimation(m_sleep);
}


void Player::ProcessMove(GLfloat deltaTime) {
	if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		m_SP > 10) {
		deltaTime *= 3;
	}
	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
		glm::vec3 apos = m_camera->GetMovedForward(deltaTime);
		if (!CollideJudgement(apos)) {
			m_camera->MoveForward(deltaTime);
		}
	}
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
		glm::vec3 apos = m_camera->GetMovedBack(deltaTime);
		if (!CollideJudgement(apos)) {
			m_camera->MoveBack(deltaTime);
		}
	}
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
		glm::vec3 apos = m_camera->GetMovedRight(deltaTime);
		if (!CollideJudgement(apos)) {
			m_camera->MoveRight(deltaTime);
		}
	}
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
		glm::vec3 apos = m_camera->GetMovedLeft(deltaTime);
		if (!CollideJudgement(apos)) {
			m_camera->MoveLeft(deltaTime);
		}
	}
	if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS) {
		m_camera->MoveUp(deltaTime);
	}
	if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS) {
		m_camera->MoveDown(deltaTime);
	}
}

void Player::CallBack(GLfloat deltaTime) {
	if (m_animator->GetCurrentAnimation() == m_sleep) {
		if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS ||
			glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS ||
			glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS ||
			glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
			if (m_state == 0) {
				m_state = 50;
				aStand();
			}
		}
	}
	else {
		if (!textBox->GetOpenState()) {
			if (glfwGetKey(m_window, GLFW_KEY_T) == GLFW_PRESS) {
				m_camera->TurnSpotLight();
			}

			if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS ||
				glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS ||
				glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS ||
				glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
				if (m_state == 0) {
					if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
						m_SP > 10) {
						aSlowRun();
						m_runState = GL_TRUE;
					}
					else {
						aWalk();
						m_runState = GL_FALSE;
					}
				}
				else {
					if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
						if (m_animator->GetCurrentAnimation() != m_slowRun &&
							m_SP > 10) {
							aSlowRun();
						}
					}
					else {
						if (m_animator->GetCurrentAnimation() != m_walk) {
							aWalk();
						}
					}
				}
				m_state = 10;
			}
			else {
				if (m_state != 0) {
					m_state--;
					if (m_state == 0) {
						aStand();
					}
				}
				else {
					if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
						// std::cout << "mouse left press\n";
						m_singleAttackState = GL_FALSE;
						aPunch();
						m_state = 100;
					}
					else if (!m_battleState && glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
						TalkJudgment();
					}
				}
			}
			if (!m_fixedState) {
				ProcessMove(deltaTime);
			}
			m_camera->ProcessCursorMove(m_window);
			if (m_battleState) {
				InjuredJudgement();
				if (m_animator->GetCurrentAnimation() == m_punch && !m_singleAttackState) {
					AttackJudgment();
				}
			}
		}
		else {
			if (m_animator->GetCurrentAnimation() != m_stand) {
				aStand();
			}
			m_camera->ProcessCursorMove(m_window, GL_FALSE);
		}
	}
	if (m_animator->GetCurrentAnimation() == m_slowRun) {
		if (m_SP > 0)
			m_SP -= 0.25;
		if (m_SP == 0) {
			aWalk();
		}
	}
	else if (m_SP < 100) {
		m_SP += 0.25;
	}

	m_animator->UpdateAnimation(deltaTime);

}

void Player::SetScales(GLfloat scales) {
	m_scales = scales;

}

void Player::SetBattleState(GLboolean state) {
	m_battleState = state;
}

void Player::SetFixedState(GLboolean state) {
	m_fixedState = state;
}

std::vector<Role*>& Player::GetRoles() {
	return m_roles;
}

std::vector<Enemy*>& Player::GetEnemys() {
	return m_enemys;
}

GLfloat Player::GetRateHP() {
	return m_HP / m_FHP;
}

GLfloat Player::GetRateSP() {
	return m_SP / m_FSP;
}

GLboolean Player::GetSleepState() {
	return m_animator->GetCurrentAnimation() == m_sleep;
}

GLboolean Player::GetFixedState() {
	return m_fixedState;
}

void Player::Draw(Shader& shader) {
	shader.Bind();
	shader.SetUniform1i("isBlinn", 1);
	shader.SetUniform1i("isPlay", 1);
	bool TurnOn = false;
	if (m_camera->GetSpotLightState()) {
		m_camera->TurnSpotLight();
		TurnOn = true;
	}
	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_camera->GetPosition() + m_camera->GetForward() - glm::vec3(0, 3.9f, 0));
	model = glm::rotate(model, m_camera->GetYaw(), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(m_scales));
	shader.SetUniformMatrix4fv("u_model", model);
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
	if (TurnOn) {
		m_camera->TurnSpotLight();
	}
	shader.UnBind();
}

GLboolean Player::DirectionJudgment(glm::vec3 apos, glm::vec3 pos) {
	glm::vec3 direction = pos - apos;
	direction.y = 0;
	direction = glm::normalize(direction);
	glm::vec3 forward = m_camera->GetForward();
	forward.y = 0;
	forward = glm::normalize(forward);

	if (glm::dot(direction, forward) >= m_attackAngle) {
		return GL_TRUE;
	}
	return GL_FALSE;
}

GLboolean Player::CollideJudgement(glm::vec3 apos) {
	apos.y = 0;
	for (Role* role : m_roles) {
		glm::vec3 pos = role->GetPosition();
		pos.y = 0;
		GLfloat dist = glm::l2Norm(apos - pos);
		if (dist <= m_collideRadius) {
			return GL_TRUE;
		}
	}
	for (Enemy* enemy : m_enemys) {
		glm::vec3 pos = enemy->GetPosition();
		pos.y = 0;
		GLfloat dist = glm::l2Norm(apos - pos);
		if (dist <= m_collideRadius) {
			return GL_TRUE;
		}
	}
	return GL_FALSE;
}

void Player::TalkJudgment() {
	for (Role* role : m_roles) {
		glm::vec3 apos = m_camera->GetPosition();
		apos.y = 0;
		glm::vec3 pos = role->GetPosition();
		pos.y = 0;
		if (DirectionJudgment(apos, pos)) {
			std::cout << " in radius! \n";
			GLfloat dist = glm::l2Norm(apos - pos);
			if (dist <= m_talkRadius) {
				role->Talk();
			}

		}
		else {
			std::cout << "No in radius! \n";
		}

	}
	for (Enemy* enemy : m_enemys) {
		glm::vec3 apos = m_camera->GetPosition();
		apos.y = 0;
		glm::vec3 pos = enemy->GetPosition();
		pos.y = 0;
		if (DirectionJudgment(apos, pos)) {
			std::cout << " in radius! \n";
			GLfloat dist = glm::l2Norm(apos - pos);
			if (dist <= m_talkRadius) {
				enemy->Talk();
			}
		}
		else {
			std::cout << "No in radius! \n";
		}

	}
}

void Player::AttackJudgment() {
	glm::vec3 apos = m_camera->GetPosition();
	apos.y = 0;
	for (Enemy* enemy : m_enemys) {
		glm::vec3 pos = enemy->GetPosition();
		pos.y = 0;
		if (DirectionJudgment(apos, pos)) {
			// std::cout << " In angle radius! \n";
			GLfloat dist = glm::l2Norm(apos - pos);
			if (dist <= m_attackRadius) {
				enemy->Injured(m_ATK);
				enemy->m_attackDirection = glm::normalize(apos - pos);
				m_singleAttackState = GL_TRUE;
			}
		}
		else {
			// std::cout << "No in angle radius! \n";
		}

	}
}

void Player::InjuredJudgement() {
	glm::vec3 apos = m_camera->GetPosition();
	apos.y = 0;
	for (Enemy* enemy : m_enemys) {
		if (enemy->GetSingleAttackState())continue;
		GLfloat atk = enemy->GetATK();
		glm::vec3 pos = enemy->GetPosition();
		pos.y = 0;
		GLfloat dist = glm::l2Norm(apos - pos);
		if (dist <= m_attackRadius) {
			GLfloat harm = glm::max(0.0f, atk - m_DEF);
			m_HP -= glm::min(m_HP, harm);
			enemy->SetSingleAttackState(GL_TRUE);
		}
	}
}

void Player::AddRoleList(Role* role) {
	m_roles.push_back(role);
}

void Player::AddEnemyList(Enemy* enemy) {
	m_enemys.push_back(enemy);
}
