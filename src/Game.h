#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <irrKlang.h>

#include "Enemy.h"
#include "Player.h"
#include "TextBox.h"
#include "Bar.h"

class Game {
private:
	GLboolean isPlayIn;
	GLboolean isExitIn;
	GLboolean isStart;
	GLboolean isAudioLoad;
	GLboolean isSoundPlay;
	GLboolean isFuture;
	GLboolean isInCastle;

	CodeType codeType;

	GLfloat currentFrame;
	GLfloat lastFrame;
	GLfloat deltaTime;

	GLfloat WINDOW_WIDTH;
	GLfloat WINDOW_HEIGHT;
	GLfloat BUTTON_WIDTH;
	GLfloat BUTTON_HEIGHT;
	GLfloat TITLE_WIDTH;
	GLfloat TITLE_HEIGHT;
	GLfloat TITLE_POSX;
	GLfloat TITLE_POSY;
	GLfloat PLAY_POSY;
	GLfloat PLAY_POSX;
	GLfloat EXIT_POSY;
	GLfloat EXIT_POSX;
	GLfloat BAR_WIDTH;
	GLfloat BAR_HEIGHT;
	GLfloat HPBAR_POSX;
	GLfloat HPBAR_POSY;
	GLfloat SPBAR_POSX;
	GLfloat SPBAR_POSY;

	std::vector<std::string> faces{
		"./res/texture/skybox/posx.jpg",
		"./res/texture/skybox/negx.jpg",
		"./res/texture/skybox/posy.jpg",
		"./res/texture/skybox/negy.jpg",
		"./res/texture/skybox/posz.jpg",
		"./res/texture/skybox/negz.jpg"
	};

	GLfloat skyboxVertices[108] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	GLfloat floorVertices[30] = {
		// positions           // texture Coords 
		 5.0f, 0.0f, -5.0f,  100.0f, 100.0f,
		-5.0f, 0.0f, -5.0f,  0.0f,   100.0f,
		-5.0f, 0.0f,  5.0f,  0.0f,   0.0f,
		-5.0f, 0.0f,  5.0f,  0.0f,   0.0f,
		 5.0f, 0.0f,  5.0f,  100.0f, 0.0f,
		 5.0f, 0.0f, -5.0f,  100.0f, 100.0f,
	};
	std::vector<glm::vec3> treePositions;

	std::string CodeTypeString[2];

	GLFWwindow* window;
	Camera* camera;
	Renderer* skybox;
	Renderer* floor;
	Text* text;
	Shader* textShader;
	TextBox* textBox;
	TextBox* initHint;
	TextBox* timeHint;
	std::string timeElapsedHint;
	sstring futureText;
	irrklang::ISoundEngine* soundEngine;
	irrklang::ISound* sound;

	Shader* modelShader;

	GLfloat timeTravelTimer;
	GLfloat timeElapsedTimer;
	GLfloat startAnimationTimer;
	GLfloat startAnimationPosy;
	Shader* startAnimationShader;

	Rectangle* partUp;
	Rectangle* partDown;
	Rectangle* partBack;
	Rectangle* partText;

	Rectangle* timeBack;

	Rectangle* startBackground;
	Rectangle* title;
	Rectangle* play;
	Rectangle* exit;
	Rectangle* end;
	Bar* HPBar;
	Bar* SPBar;
	Bar* BOSSHPBar;

	Player* player;
	Role* older;
	Enemy* boss;
	Model* tree;
	Model* house;
	Model* hotel;
	Model* castle;
	Model* well;
	Model* bed;
	std::vector<Model*> tests;

public:
	Game(GLFWwindow* Window, GLint width, GLint height);
	// init and load
	void InitCamera(); // 必须先初始化摄像头
	void InitRoles();
	void InitEnemys();
	void InitPlayer();
	void InitTrees();
	void InitStartAnimation();
	void InitTimeTravel();
	void InitTimeElapsed();
	void LoadTextBox();
	void LoadFloor();
	void LoadSkyBox();
	void LoadAudio();
	void LoadStartScene();
	void LoadPanel();

	// draw
	void DrawRoles();
	void DrawEnemys();
	void DrawPlayer();
	void DrawFloor();
	void DrawSkyBox();
	void DrawTrees();
	void DrawInitHint();
	void DrawTimeHint();
	void DrawStartScene();
	void DrawPanel();
	void DrawHouse();
	void DrawHotel();
	void DrawCastle();
	void DrawBed();
	void DrawWell();
	void Draw();

	// sound
	void SetSoundVolume(GLfloat volume);
	void TurnSoundState(GLboolean state);

	void TimeTravelPlay();
	void TimeElapsedPlay();
	void StartAnimationPlay();
	void StartCallBack();
	void ProcessKeyInput();
};