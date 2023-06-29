#include "Game.h"

Game::Game(GLFWwindow* Window, GLint width, GLint height)
	:window(Window), WINDOW_WIDTH(width), WINDOW_HEIGHT(height) {
	codeType = CodeType::UTF8;
	CodeTypeString[CodeType::ANSI] = "ANSI";
	CodeTypeString[CodeType::UTF8] = "UTF8";

	isStart = GL_TRUE;
	isPlayIn = GL_FALSE;
	isExitIn = GL_FALSE;
	isSoundPlay = GL_FALSE;
	isAudioLoad = GL_FALSE;
	isFuture = GL_FALSE;
	isInCastle = GL_FALSE;

	timeTravelTimer = 0.0f;
	timeElapsedTimer = 0.0f;
	startAnimationTimer = 0.0f;
	startAnimationPosy = 0.0f;
	currentFrame = 0.0f;
	lastFrame = 0.0f;
	deltaTime = 0.0f;

	BUTTON_WIDTH = 300.0f;
	BUTTON_HEIGHT = 100.0f;
	TITLE_WIDTH = 750.0f;
	TITLE_HEIGHT = 400.0f;
	TITLE_POSX = (WINDOW_WIDTH - TITLE_WIDTH) / 2.0f;
	TITLE_POSY = 600.0f;
	PLAY_POSX = (WINDOW_WIDTH - BUTTON_WIDTH) / 2.0f;
	PLAY_POSY = 500.0f;
	EXIT_POSX = (WINDOW_WIDTH - BUTTON_WIDTH) / 2.0f;
	EXIT_POSY = 350.f;
	BAR_WIDTH = 500;
	BAR_HEIGHT = 40;
	HPBAR_POSX = 10;
	HPBAR_POSY = WINDOW_HEIGHT - 50;
	SPBAR_POSX = 10;
	SPBAR_POSY = WINDOW_HEIGHT - 100;

	Rectangle::windowWidth = WINDOW_WIDTH;
	Rectangle::windowHeight = WINDOW_HEIGHT;

	InitCamera();
	InitRoles();
	InitEnemys();
	InitPlayer();
	InitTrees();
	InitStartAnimation();
	InitTimeTravel();
	InitTimeElapsed();

	LoadTextBox();
	LoadSkyBox();
	LoadFloor();
	LoadStartScene();
	LoadPanel();

	modelShader = new Shader("./res/shader/shader.vert", "./res/shader/shader.frag");

	tree = new Model("./res/asset/tree/tree.fbx");

	house = new Model("./res/asset/house/house.fbx");
	hotel = new Model("./res/asset/hotel/hotel.fbx");
	castle = new Model("./res/asset/castle/castle.dae");
	well = new Model("./res/asset/well/well.FBX");
	bed = new Model("./res/asset/bed/bed.fbx");
}

void Game::InitCamera() {
	camera = new Camera(glm::vec3(6.0f, 3.2f, 31.2f));
	camera->SetWidthHeight(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void Game::InitRoles() {
	older = new Role(window, camera, "./res/asset/older/stand.dae");
	older->SetScales(1.5f);
	older->SetRotateParam(glm::vec4(glm::radians(150.0f), 0.0f, 1.0f, 0.0f));
	older->SetPosition(glm::vec3(-63.0f, 0.5f, 19.0f));
	older->LoadText("./res/text/older" + CodeTypeString[codeType] + ".txt");
	older->LoadLoopText("./res/text/olderLoop" + CodeTypeString[codeType] + ".txt");
	older->LoadAnimation();
}

void Game::InitEnemys() {
	boss = new Enemy(window, camera, "./res/asset/boss/stand.dae");
	boss->SetScales(1.5f);
	boss->SetPosition(glm::vec3(0.0f, 0.0f, -50.0f));
	boss->LoadText("./res/text/boss" + CodeTypeString[codeType] + ".txt");
	boss->LoadAnimation();
}

void Game::InitPlayer() {
	player = new Player(window, camera, "./res/asset/remy/sleep.dae");
	player->LoadAnimation();
	player->AddRoleList(older);
	player->AddEnemyList(boss);
}

void Game::InitTrees() {
	srand((unsigned long long)time(0));
	glm::vec3 castlePos = glm::vec3(0.0f, 0.0f, -65.0f);
	glm::vec3 hotelPos = glm::vec3(-60.0f, 0.0f, 20.0f);
	for (int i = 0; i < 300; i++) {
		glm::vec3 tmp;
		tmp.x = rand() % 200 - 100;
		tmp.z = rand() % 200 - 100;
		tmp.y = 0;
		while (tmp.x * tmp.x + tmp.z * tmp.z <= 3000 ||
			glm::l2Norm(tmp - castlePos) <= 18.0f ||
			glm::l2Norm(tmp - hotelPos) <= 24.0f) {
			tmp.x = rand() % 200 - 100;
			tmp.z = rand() % 200 - 100;
		}
		// std::cout << tmp.x << " " << tmp.z << "\n";
		/*if (rand() % 10 > 5)tmp.x = -tmp.x;
		if (rand() % 10 > 5)tmp.z = -tmp.z;*/
		treePositions.push_back(tmp);
	}
}

void Game::InitStartAnimation() {
	partUp = new Rectangle(-1.0f, 0.5f, 2.0f, 0.5f, GL_FALSE, GL_FALSE);
	partUp->SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	partDown = new Rectangle(-1.0f, -1.0f, 2.0f, 0.5f, GL_FALSE, GL_FALSE);
	partDown->SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	partBack = new Rectangle(-1.0f, -1.0f, 2.0f, 2.0f, GL_FALSE, GL_FALSE);
	partBack->SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	partText = new Rectangle(-0.5f, -2.5f, 1.0f, 2.0f, GL_FALSE, GL_TRUE);
	partText->SetTexture("./res/texture/startAnimationText.png");
	startAnimationShader = new Shader("./res/shader/startAnimationText.vert", "./res/shader/startAnimationText.frag");
	partText->SetShader(startAnimationShader);
}

void Game::InitTimeTravel() {
	timeBack = new Rectangle(-1.0f, -1.0f, 2.0f, 2.0f);
	timeBack->SetColor(glm::vec4(1.0f));
}

void Game::InitTimeElapsed() {
	timeElapsedHint;
	std::ifstream file("./res/text/elapsedHintUTF8.txt");
	getline(file, timeElapsedHint);
}

void Game::LoadTextBox() {
	CharacterSet* characterSet = new CharacterSet("./res/fonts/glyph/STSONG.TTF", codeType);
	text = new Text(characterSet);
	text->SetProjection(glm::ortho(0.0f, (GLfloat)WINDOW_WIDTH, 0.0f, (GLfloat)WINDOW_HEIGHT));
	textShader = new Shader("./res/shader/text.vert", "./res/shader/text.frag");
	textBox = new TextBox(text, textShader);
	Role::textBox = textBox;
	Player::textBox = textBox;

	initHint = new TextBox(text, textShader, -0.8f, 0.5f, 1.6f, 0.25f);
	std::ifstream initfile("./res/text/initHintUTF8.txt");
	std::string line;
	getline(initfile, line);
	initHint->SetContext(line);
	initHint->SetContextParam(glm::vec3(400, 860, 1.0f));

	timeHint = new TextBox(text, textShader, -0.8f, 0.5f, 1.6f, 0.25f);
	std::ifstream timefile("./res/text/timeHintUTF8.txt");
	getline(timefile, line);
	timeHint->SetContext(line);
	timeHint->SetContextParam(glm::vec3(300, 860, 0.6f));

	std::ifstream futurefile("./res/text/futureUTF8.txt");
	getline(futurefile, futureText.first);
	getline(futurefile, futureText.second);
}

void Game::LoadFloor() {
	floor = new Renderer();
	floor->m_shader = new Shader("./res/shader/floor.vert", "./res/shader/floor.frag");
	floor->m_shader->Bind();
	floor->m_shader->SetUniform1i("texture_diffuse0", 0);

	Texture* floorTexture = new Texture();
	floorTexture->Bind();
	Texture::LoadTexture("./res/texture/skybox/negy.jpg");
	floorTexture->SetFilter(GL_LINEAR, GL_LINEAR);
	floorTexture->SetWrap(GL_REPEAT, GL_REPEAT);

	VertexArray* floorVAO = new VertexArray;
	Buffer* floorVBO = new Buffer(GL_ARRAY_BUFFER);
	floorVAO->Bind();
	floorVBO->Bind();
	floorVBO->SetData(sizeof(floorVertices), &floorVertices);
	floorVAO->AddAttribute(3, GL_FLOAT, GL_FALSE);
	floorVAO->AddAttribute(2, GL_FLOAT, GL_FALSE);
	floorVAO->EnableAllAttribute();

	floor->AddTexture(floorTexture);
	floor->SetVertexBuffer(floorVBO);
	floor->SetVertexArray(floorVAO);

	floor->UnBind();
}

void Game::LoadSkyBox() {
	skybox = new Renderer();
	skybox->m_shader = new Shader("./res/shader/sky.vert", "./res/shader/sky.frag");
	skybox->m_shader->Bind();
	skybox->m_shader->SetUniform1i("u_skybox", 0);

	Texture* skyTexture = new Texture(GL_TEXTURE_CUBE_MAP);
	skyTexture->Bind();
	skyTexture->SetFilpVertically(GL_FALSE);
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		Texture::LoadTexture(faces[i], GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
	}
	skyTexture->SetFilter(GL_LINEAR, GL_LINEAR);
	skyTexture->SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

	VertexArray* skyVAO = new VertexArray;
	Buffer* skyVBO = new Buffer(GL_ARRAY_BUFFER);
	skyVAO->Bind();
	skyVBO->Bind();
	skyVBO->SetData(sizeof(skyboxVertices), skyboxVertices);
	skyVAO->AddAttribute(3, GL_FLOAT);
	skyVAO->EnableAllAttribute();

	skybox->AddTexture(skyTexture);
	skybox->SetVertexArray(skyVAO);
	skybox->SetVertexBuffer(skyVBO);

	skybox->UnBind();
}

void Game::LoadAudio() {
	soundEngine = irrklang::createIrrKlangDevice();
	sound = soundEngine->play2D("./res/audio/middle ground.mp3", GL_TRUE);
}

void Game::LoadStartScene() {
	// startBackground = new Rectangle(-1.0f, -1.0f, 2.0f, 2.0f, GL_FALSE, GL_TRUE);
	startBackground = new Rectangle(0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT, GL_TRUE, GL_TRUE);
	startBackground->SetTexture("./res/texture/start.png");

	title = new Rectangle(TITLE_POSX, TITLE_POSY, TITLE_WIDTH, TITLE_HEIGHT, GL_TRUE, GL_TRUE);
	title->SetTexture("./res/texture/title.png");

	play = new Rectangle(PLAY_POSX, PLAY_POSY, BUTTON_WIDTH, BUTTON_HEIGHT, GL_TRUE, GL_TRUE);
	play->SetTexture("./res/texture/play0.png");

	exit = new Rectangle(EXIT_POSX, EXIT_POSY, BUTTON_WIDTH, BUTTON_HEIGHT, GL_TRUE, GL_TRUE);
	exit->SetTexture("./res/texture/exit0.png");

	end = new Rectangle(-0.5f, 0.3f, 1.0f, 0.7f, GL_FALSE, GL_TRUE);
	end->SetTexture("./res/texture/end.png");
}

void Game::LoadPanel() {
	HPBar = new Bar(HPBAR_POSX, HPBAR_POSY, BAR_WIDTH, BAR_HEIGHT, glm::vec3(1.0f, 0.0f, 0.0f));
	SPBar = new Bar(SPBAR_POSX, SPBAR_POSY, BAR_WIDTH, BAR_HEIGHT, glm::vec3(0.0f, 0.0f, 1.0f));
	BOSSHPBar = new Bar((1960 - 800) / 2.0 - 40, 20, 800, 40, glm::vec3(1.0f, 0.0f, 0.0f));
}

void Game::DrawRoles() {
	if (!isFuture) {
		for (Role* role : player->GetRoles()) {
			role->Draw(*modelShader);
			role->CallBack(deltaTime);
		}
	}
}

void Game::DrawEnemys() {
	if (isFuture) {
		for (Enemy* enemy : player->GetEnemys()) {
			enemy->SetTargetPosition(camera->GetPosition());
			enemy->Draw(*modelShader);
			enemy->CallBack(deltaTime);
		}
	}
}

void Game::DrawPanel() {
	if (timeElapsedTimer >= 1080.0f) {
		return;
	}
	glDepthFunc(GL_LEQUAL);
	HPBar->Draw(player->GetRateHP());
	SPBar->Draw(player->GetRateSP());
	if (boss->GetBattleState()) {
		BOSSHPBar->Draw(boss->GetRateHP());
	}
	glDepthFunc(GL_LESS);
}

void Game::DrawPlayer() {
	if (boss->GetInCastleState() && !player->GetFixedState()) {
		camera->SetPosition(glm::vec3(1.5f, 3.2f, -72.0f));
		player->SetFixedState(GL_TRUE);
	}
	if (player->GetFixedState()) {
		timeElapsedTimer += 0.5;
	}
	player->SetBattleState(boss->GetBattleState());
	player->Draw(*modelShader);
	if (timeElapsedTimer <= 1080.0f) {
		player->CallBack(deltaTime);
	}
}

void Game::DrawFloor() {
	Shader* shader = floor->m_shader;
	shader->Bind();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(100.0f));
	shader->SetUniformMatrix4fv("u_model", model);
	shader->SetUniformMatrix4fv("u_view", camera->GetView());
	shader->SetUniformMatrix4fv("u_projection", camera->GetProjection());
	camera->DrawLight(*shader);
	floor->DrawArrays(GL_TRIANGLES, 0, 6);
}

void Game::DrawSkyBox() {
	glDepthFunc(GL_LEQUAL);
	Shader* shader = skybox->m_shader;
	shader->Bind();
	shader->SetUniformMatrix4fv("u_view", glm::mat4(glm::mat3(camera->GetView())));
	shader->SetUniformMatrix4fv("u_projection", camera->GetProjection());
	skybox->DrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS);
}

void Game::DrawTrees() {
	for (int i = 0; i < (int)treePositions.size(); i++) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), treePositions[i]);
		modelShader->Bind();
		modelShader->SetUniformMatrix4fv("u_model", model);
		tree->Draw(*modelShader);
	}
}

void Game::DrawInitHint() {
	if (player->GetSleepState()) {
		initHint->Draw();
	}
}

void Game::DrawTimeHint() {
	if (timeTravelTimer > 0 && timeTravelTimer <= 260) {
		timeTravelTimer += 0.5f;
		timeHint->Draw();
	}
}

void Game::DrawStartScene() {
	glDepthFunc(GL_LEQUAL);
	startBackground->Draw();
	title->Draw();
	play->Draw();
	exit->Draw();
	glDepthFunc(GL_LESS);
}

void Game::DrawHotel() {
	glDisable(GL_CULL_FACE);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-60.0f, -2.0f, 20.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, .0f, .0f));
	model = glm::rotate(model, glm::radians(-50.0f), glm::vec3(.0f, .0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.7f));
	modelShader->Bind();
	modelShader->SetUniformMatrix4fv("u_model", model);
	hotel->Draw(*modelShader);
	glEnable(GL_CULL_FACE);
}

void Game::DrawCastle() {
	if (isFuture) {
		glDisable(GL_CULL_FACE);
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, -65.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, .0f, .0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(.0f, .0f, 1.0f));
		model = glm::scale(model, glm::vec3(6.f));
		modelShader->Bind();
		modelShader->SetUniformMatrix4fv("u_model", model);
		castle->Draw(*modelShader);
		glEnable(GL_CULL_FACE);
	}
}

void Game::DrawHouse() {
	glDisable(GL_CULL_FACE);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, -1.1f, 30.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, .0f, .0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(.0f, .0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.5f));
	modelShader->Bind();
	modelShader->SetUniformMatrix4fv("u_model", model);
	house->Draw(*modelShader);
	glEnable(GL_CULL_FACE);
}

void Game::DrawBed() {
	glDisable(GL_CULL_FACE);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, 0.0f, 30.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, .0f, .0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(.0f, .0f, 1.0f));
	model = glm::scale(model, glm::vec3(2.f));
	modelShader->Bind();
	modelShader->SetUniformMatrix4fv("u_model", model);
	bed->Draw(*modelShader);
	glEnable(GL_CULL_FACE);
}

void Game::DrawWell() {
	glDisable(GL_CULL_FACE);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 7.3f, -5.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, .0f, .0f));
	model = glm::scale(model, glm::vec3(.02f));
	modelShader->Bind();
	modelShader->SetUniformMatrix4fv("u_model", model);
	well->Draw(*modelShader);
	glEnable(GL_CULL_FACE);
}

void Game::Draw() {
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	currentFrame = (GLfloat)glfwGetTime();

	if (isStart) {
		DrawStartScene();
		StartCallBack();
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS ||
			glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		}
	}
	else if (startAnimationTimer <= 100) {
		startAnimationTimer += 0.01;
		StartAnimationPlay();
	}
	else {
		if (!isAudioLoad) {
			LoadAudio();
			isAudioLoad = GL_TRUE;
			isSoundPlay = GL_TRUE;
		}
		if (!isFuture) {
			glm::vec3 apos = camera->GetPosition();
			glm::vec3 pos = glm::vec3(1.0f, 7.2f, 0.0f);
			glm::vec3 direction = apos - pos;
			direction.y = 0;
			GLfloat distance = glm::l2Norm(direction);
			if (distance < 5.0f) {
				TimeTravelPlay();
			}
		}
		if (timeElapsedTimer >= 100.0f && timeElapsedTimer <= 1000.0f) {
			TimeElapsedPlay();
		}
		else if (timeTravelTimer == 0 || timeTravelTimer >= 200) {

			// 混合测试不能打乱绘制顺序
			DrawPlayer();

			DrawSkyBox();
			DrawFloor();
			DrawTrees();

			DrawHouse();
			DrawHotel();
			DrawCastle();
			DrawBed();
			DrawWell();

			DrawRoles();
			DrawEnemys();

			DrawPanel();
			DrawInitHint();
			DrawTimeHint();
			if (timeElapsedTimer >= 1000.0f && timeElapsedTimer <= 1080.0f) {
				textBox->SetTalker(futureText.first);
				textBox->SetContext(futureText.second);
				textBox->Draw();
			}
			else if (timeElapsedTimer >= 1080.0f && timeElapsedTimer <= 1200.0f) {
				end->Draw();
			}
			else if (timeElapsedTimer >= 1200.0f) {
				glfwSetWindowShouldClose(window, GL_TRUE);
			}
		}
	}
}

void Game::ProcessKeyInput() {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		TurnSoundState(isSoundPlay);
		isSoundPlay = 1 - isSoundPlay;
	}
}

void Game::TimeTravelPlay() {
	timeTravelTimer += 0.5;
	if (timeTravelTimer >= 200) {
		isFuture = GL_TRUE;
	}
	timeBack->SetColor(glm::vec4(200 - timeTravelTimer));
	timeBack->Draw();
}

void Game::TimeElapsedPlay() {
	glDepthFunc(GL_LEQUAL);
	timeElapsedTimer += 0.5f;
	partBack->Draw();
	text->Draw(*textShader, timeElapsedHint, 250, 800, 0.8f, glm::vec3(1.0f));
	glDepthFunc(GL_LESS);
}

void Game::StartAnimationPlay() {
	glDepthFunc(GL_LEQUAL);
	partBack->Draw();

	startAnimationShader->Bind();
	startAnimationShader->SetUniform1f("u_posy", startAnimationPosy);
	partText->Draw();

	partUp->Draw();
	partDown->Draw();
	glDepthFunc(GL_LESS);
	if (startAnimationTimer <= 88)
		startAnimationPosy += 0.0003;
}

void Game::StartCallBack() {
	GLdouble xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	ypos = WINDOW_HEIGHT - ypos;
	if (xpos >= PLAY_POSX && xpos <= PLAY_POSX + BUTTON_WIDTH &&
		ypos >= PLAY_POSY && ypos <= PLAY_POSY + BUTTON_HEIGHT) {
		play->SetTexture("./res/texture/play1.png");
		isPlayIn = GL_TRUE;
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			isStart = GL_FALSE;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		}
	}
	else {
		if (isPlayIn) {
			play->SetTexture("./res/texture/play0.png");
			isPlayIn = GL_FALSE;
		}
	}

	if (xpos >= EXIT_POSX && xpos <= EXIT_POSX + BUTTON_WIDTH &&
		ypos >= EXIT_POSY && ypos <= EXIT_POSY + BUTTON_HEIGHT) {
		isExitIn = GL_TRUE;
		exit->SetTexture("./res/texture/exit1.png");
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}
	else {
		if (isExitIn) {
			isExitIn = GL_FALSE;
			exit->SetTexture("./res/texture/exit0.png");
		}
	}

}

void Game::SetSoundVolume(GLfloat volume) {
	sound->setVolume(volume);
}

void Game::TurnSoundState(GLboolean state) {
	soundEngine->setAllSoundsPaused(state);
}


