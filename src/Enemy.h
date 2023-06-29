#pragma once

#include "Role.h"

class Enemy : public Role {
public:
	Enemy(GLFWwindow* window, Camera* camera, const std::string& modelPath);
	void LoadAnimation();
	void Injured(GLfloat ATK);

	void aInjured();
	void aKick();
	void aSwip();
	void aForward();

	void UpdateAttackParam();
	GLfloat GetATK();
	GLboolean GetSingleAttackState();
	GLboolean GetBattleState();
	GLboolean GetInCastleState();
	GLfloat GetRateHP();
	glm::vec3 GetTargetDirection();

	void SetSingleAttackState(GLboolean state);
	void SetTargetPosition(glm::vec3 position);
	bool TargetDistanceJudgement(GLfloat radius);
	void Battle();
	void Talk();
	void CallBack(GLfloat deltaTime);
	void Draw(Shader& shader);

	glm::vec3 m_attackDirection;
private:
	Animation* m_injured;
	Animation* m_kick;
	Animation* m_swip;
	Animation* m_forward;

	GLint m_aState;

	GLfloat m_HP;
	GLfloat m_FHP;
	GLfloat m_ATK;
	GLfloat m_DEF;

	GLfloat m_collideRadius;
	GLfloat m_attackRaius;

	GLboolean m_singleAttackState;
	GLboolean m_battleState;
	glm::vec3 m_targetPosition;
};