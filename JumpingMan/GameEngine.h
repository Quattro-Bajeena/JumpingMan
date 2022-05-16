#pragma once
#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <filesystem>

#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "RenderModel.h"
#include "Camera.h"
#include "Object.h"


class GameEngine
{
private:
	GLFWwindow* window;
	glm::vec2 windowSize;
	float angle_x = 0;
	float angle_y = 0;
	float rotateObjectsX = 0;
	float rotateObjectsY = 0;

	glm::vec2 mouseDelta;
	glm::vec2 mousePos;

	int camMoveForward = 0;
	int camMoveSide = 0;

	std::map<std::string, GLuint> textures;
	std::map<std::string, RenderModel> models;
	std::map<std::string, ShaderProgram*> shaders;

	std::vector<Object> objects;

	Camera cam;

	
	float lastTime;
	float deltaTime;
	float aspectRatio;

	void Update(float dt);
	void Render();
	void KeyInputCallback(int key, int scancode, int action, int mod);
	void KeyInput();
	void MouseInput();
	GLuint ReadTexture(std::string filename);
	void InitOpenGLProgram();
	void FreeOpenGLProgram();
	void LoadTextures();
	void LoadModels();
	void LoadShaders();

	//callbacks
	static void ErrorCallback(int error, const char* description);
	static void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mod);
	static void WindowResizeCallback(GLFWwindow* window, int width, int height);

public:
	GameEngine();
	~GameEngine();
	void Run();
};

