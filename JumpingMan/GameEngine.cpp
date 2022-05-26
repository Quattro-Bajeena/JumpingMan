#include "pch.h"
#include "GameEngine.h"
#include "OBJ_Loader.h"



void GameEngine::ErrorCallback(int error, const char* description) {
	fputs(description, stderr);
}

void GameEngine::WindowResizeCallback(GLFWwindow* window, int width, int height) {
	GameEngine* engine = (GameEngine*)glfwGetWindowUserPointer(window);
	if (height == 0) return;
	engine->aspectRatio = (float)width / (float)height;
	engine->windowSize = glm::vec2(width, height);
	glViewport(0, 0, width, height);
}

void GameEngine::KeyCallBack(
	GLFWwindow* window,
	int key,
	int scancode,
	int action,
	int mod
) {
	GameEngine* engine = (GameEngine*)glfwGetWindowUserPointer(window);
	engine->KeyInputCallback(key, scancode, action, mod);
}

void GameEngine::KeyInputCallback(int key, int scancode, int action, int mod)
{
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) rotateObjectsY = PI;
		if (key == GLFW_KEY_RIGHT) rotateObjectsY = -PI;
		if (key == GLFW_KEY_UP) rotateObjectsX = -PI;
		if (key == GLFW_KEY_DOWN) rotateObjectsX = PI;
	}
	if (action == GLFW_RELEASE) {

		if (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) rotateObjectsY = 0;
		if (key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) rotateObjectsX = 0;
	}
}

void GameEngine::KeyInput()
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camMoveForward = 1;
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camMoveForward = -1;
	else camMoveForward = 0;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camMoveSide = 1;
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camMoveSide = -1;
	else camMoveSide = 0;
}

void GameEngine::MouseInput()
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	glm::vec2 newMousePos = glm::vec2(xpos, ypos);
	mouseDelta = (mousePos - newMousePos) / windowSize;
	mousePos = newMousePos;

	
}



GLuint GameEngine::ReadTexture(std::string filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Wczytanie do pamięci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import do pamięci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	return tex;
}

//Procedura inicjująca




void GameEngine::InitOpenGLProgram() {

	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST); 
	glfwSetKeyCallback(window, KeyCallBack);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

}


//Zwolnienie zasobów zajętych przez program
void GameEngine::FreeOpenGLProgram() {
	//glDeleteTextures(1, &tex);
}

void GameEngine::LoadTextures()
{
	namespace fs = std::filesystem;
	const fs::path pathToShow("textures");
	for (const auto& entry : fs::directory_iterator(pathToShow)) {
		if (entry.is_regular_file()) {
			textures[entry.path().filename().stem().string()] = ReadTexture(entry.path().string());
			std::cout << "Loaded texture: " << entry.path().string() << '\n';
		}
	}
}

void GameEngine::LoadModels()
{
	objl::Loader loader;

	std::map<std::string, glm::vec3> positions;
	std::ifstream scene_data("models/scene_data.txt", std::ios::in);
	std::string object_name;
	float x, y, z;
	while (scene_data >> object_name >> x >> y >> z) {
		positions[object_name] = glm::vec3(x, y, z);
		//std::cout << object_name << " " << x << " " << y << " " << z << "\n";
	}


	bool loadout = loader.LoadFile("models/scene.obj");
	if (loadout) {
		for (auto mesh : loader.LoadedMeshes) {
			std::string tex_path = mesh.MeshMaterial.map_Kd;
			std::string filename = tex_path.substr(tex_path.find_last_of("/\\") + 1);
			std::string texture_name = filename.substr(0, filename.find_last_of("."));

			RenderModel newModel(mesh);
			newModel.SetTexture(textures.at(texture_name));
			newModel.SetShader(shaders.at("textured"));
			models[newModel.name] = newModel;

			Object new_obj = Object(&models.at(newModel.name));
			//new_obj.position = positions.at(newModel.name);
			objects.emplace_back(new_obj);

			std::cout << "Loaded model: " << newModel.name << "\n";
			
		}
	}
}

void GameEngine::LoadShaders()
{
	shaders["textured"] = new ShaderProgram("shaders/v_textured.glsl", nullptr, "shaders/f_textured.glsl");
}

void GameEngine::Update(float dt) {
	MouseInput();
	KeyInput();

	angle_x += rotateObjectsX * dt;
	angle_y += rotateObjectsY * dt;

	for (auto &object : objects) {
		object.SetRotation(angle_x, angle_y, 0);
	}

	cam.Move(camMoveForward, camMoveSide, dt);
	cam.RotateMouse(-mouseDelta.y, mouseDelta.x, deltaTime);

}



void GameEngine::Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glm::mat4 V = cam.GetViewMatrix();
	glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, aspectRatio, 0.01f, 1000.0f); 


	for (auto& object : objects) {
		object.Render(V, P);
	}

	glfwSwapBuffers(window); 
}

GameEngine::GameEngine() {

	glfwSetErrorCallback(ErrorCallback);

	if (!glfwInit()) { 
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	int width = 1000;
	int height = 1000;
	windowSize = glm::vec2(width, height);
	window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);  
	aspectRatio = (float)width / height;
	mousePos = glm::vec2(0, 0);
	mouseDelta = glm::vec2(0, 0);

	if (!window) 
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetWindowUserPointer(window, this);
	glfwMakeContextCurrent(window); 
	glfwSwapInterval(1); 

	if (glewInit() != GLEW_OK) { 
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}


	LoadTextures();
	LoadShaders();
	LoadModels();
	InitOpenGLProgram(); 

	
	glfwSetTime(0); 
	lastTime = 0;

	cam.SetPosition(glm::vec3(0, 0, 0));
	
}

GameEngine::~GameEngine()
{
	FreeOpenGLProgram();

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}

void GameEngine::Run()
{
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		float nowTime = glfwGetTime();
		deltaTime = nowTime - lastTime;
		lastTime = nowTime;
		Update(deltaTime);
		Render();
		glfwPollEvents(); 

	}
}
