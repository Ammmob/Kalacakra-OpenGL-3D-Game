#pragma once

#include "Enemy.h"

class Player {
public:
	Player(GLFWwindow* window, Camera* camera, const std::string& modelPath);
	void LoadAnimation();
	// play animation
	void aStand();
	void aWalk();
	void aSlowRun();
	void aFastRun();
	void aPunch();
	void aSleep();

	// set attribute
	void SetScales(GLfloat scales);
	void SetBattleState(GLboolean state);
	void SetFixedState(GLboolean state);

	// get attribute
	std::vector<Role*>& GetRoles();
	std::vector<Enemy*>& GetEnemys();
	GLfloat GetRateHP();
	GLfloat GetRateSP();
	GLboolean GetSleepState();
	GLboolean GetFixedState();

	// callback and draw
	void ProcessMove(GLfloat deltaTime);
	void CallBack(GLfloat deltaTime);
	void Draw(Shader& shader);

	GLboolean DirectionJudgment(glm::vec3 apos, glm::vec3 pos);
	GLboolean CollideJudgement(glm::vec3 apos);
	void TalkJudgment();
	void AttackJudgment();
	void InjuredJudgement();
	void AddRoleList(Role* role);
	void AddEnemyList(Enemy* enemy);

	static TextBox* textBox;
private:
	GLFWwindow* m_window;
	Camera* m_camera;
	Model* m_model;
	Animator* m_animator;
	GLfloat m_scales;

	Animation* m_sleep;
	Animation* m_stand;
	Animation* m_walk;
	Animation* m_slowRun;
	Animation* m_fastRun;
	Animation* m_punch;

	GLfloat m_HP;
	GLfloat m_FHP;
	GLfloat m_SP;
	GLfloat m_FSP;
	GLfloat m_ATK;
	GLfloat m_DEF;
	GLint m_state;
	GLboolean m_talkState;
	GLboolean m_runState;
	GLboolean m_battleState;
	GLboolean m_singleAttackState;
	GLboolean m_fixedState;

	GLfloat m_talkRadius;
	GLfloat m_collideRadius;
	GLfloat m_attackRadius;
	GLfloat m_attackAngle;

	std::string m_directory;
	std::vector<Role*>m_roles;
	std::vector<Enemy*>m_enemys;
};