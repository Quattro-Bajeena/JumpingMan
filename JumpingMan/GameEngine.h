#pragma once
#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "RenderModel.h"


class GameEngine
{
private:
	GLFWwindow* window;
	float angle_x = 0;
	float angle_y = 0;
	float speed_x = 0;
	float speed_y = 0;

	GLuint tex;
	RenderModel model;
	ShaderProgram* sp;

	
	float lastTime;
	float deltaTime;
	float aspectRatio;

	void Update(float dt);
	void Render();
	void KeyInput(int key, int scancode, int action, int mod);
	GLuint ReadTexture(const char* filename);
	void InitOpenGLProgram();
	void FreeOpenGLProgram();

	//callbacks
	static void ErrorCallback(int error, const char* description);
	static void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mod);
	static void WindowResizeCallback(GLFWwindow* window, int width, int height);

public:
	GameEngine();
	~GameEngine();
	void Run();
};

