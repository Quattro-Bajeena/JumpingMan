#pragma once
#define GLM_FORCE_RADIANS

#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "RenderModel.h"
#include "Camera.h"
#include "Object.h"
#include "Player.h"


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

	int moveForward = 0;
	int moveSide = 0;

	std::map<std::string, GLuint> textures;
	std::map<std::string, GLuint> normalMaps;
	std::map<std::string, GLuint> metallicMaps;
	std::map<std::string, GLuint> roughnessMaps;
	std::map<std::string, GLuint> heightMaps;

	std::map<std::string, RenderModel> models;
	std::map<std::string, ShaderProgram*> shaders;

	Object sun;
	glm::vec3 lightPos;
	float sunRotationTime;
	float sunTimer;
	

	std::vector<Object> objects;

	Player player;
	Camera camera;

	
	float lastTime;
	float deltaTime;
	float aspectRatio;

	void Update(float dt);
	void UpdateSunPosition(float dt);
	void Render();
	void KeyInput();
	void MouseInput();
	GLuint ReadTexture(std::string filename);
	void InitOpenGLProgram();
	void LoadTextures();
	void LoadModels();
	void LoadShaders();

	//callbacks
	static void ErrorCallback(int error, const char* description);
	static void WindowResizeCallback(GLFWwindow* window, int width, int height);

public:
	GameEngine();
	~GameEngine();
	void Run();
};

