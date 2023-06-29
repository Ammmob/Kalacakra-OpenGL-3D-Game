#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Game.h"

const GLint WINDOW_WIDTH = 1960;
const GLint WINDOW_HEIGHT = 1080;

void frameBufferSizeCallBack(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

GLFWwindow* WindowInitialization(GLboolean isFullScreen = GL_TRUE) {
	// GLFW init
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	glfwWindowHint(GL_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__  // Mac 向前版本兼容
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWmonitor* pMonitor = isFullScreen ? glfwGetPrimaryMonitor() : NULL;
	GLFWwindow* window;
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "This is title!", pMonitor, NULL);

	if (!window) {
		std::cout << "ERROR::failed to create window" << std::endl;
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallBack);
	std::cout << glGetString(GL_VERSION) << std::endl;

	glewExperimental = GL_TRUE;
	// GLEW init
	if (glewInit() != GLEW_OK) {
		std::cout << "ERROR::failed to initializa GLEW" << std::endl;
		return NULL;
	}
	return window;
}

void WindowConfig() {
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main() {
	GLFWwindow* window = WindowInitialization(GL_TRUE);
	if (!window) {
		return -1;
	}

	WindowConfig();


	Game game = Game(window, WINDOW_WIDTH, WINDOW_HEIGHT);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game.ProcessKeyInput();
		game.Draw();

		// call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}