#include "pch.h"
#include "GameEngine.h"




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

		if (key == GLFW_KEY_SPACE) { 
			//std::cout << "spacja" << std::endl; 
			player.Jump(); }
		}
	if (key == GLFW_KEY_R) player.GoToStartingPos();

	if (action == GLFW_RELEASE) {

		if (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) rotateObjectsY = 0;
		if (key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) rotateObjectsX = 0;
	}
}

void GameEngine::KeyInput()
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) moveForward = 1;
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) moveForward = -1;
	else moveForward = 0;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) moveSide = 1;
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) moveSide = -1;
	else moveSide = 0;
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
	for (const auto& entry : fs::directory_iterator(fs::path("textures/color"))) {
		if (entry.is_regular_file()) {
			textures[entry.path().filename().stem().string()] = ReadTexture(entry.path().string());
			//std::cout << "Loaded texture: " << entry.path().string() << '\n';
		}
	}

	for (const auto& entry : fs::directory_iterator(fs::path("textures/normal"))) {
		if (entry.is_regular_file()) {
			normalMaps[entry.path().filename().stem().string()] = ReadTexture(entry.path().string());
			//std::cout << "Loaded texture: " << entry.path().string() << '\n';
		}
	}

	for (const auto& entry : fs::directory_iterator(fs::path("textures/metallic"))) {
		if (entry.is_regular_file()) {
			metallicMaps[entry.path().filename().stem().string()] = ReadTexture(entry.path().string());
			//std::cout << "Loaded texture: " << entry.path().string() << '\n';
		}
	}

	for (const auto& entry : fs::directory_iterator(fs::path("textures/roughness"))) {
		if (entry.is_regular_file()) {
			roughnessMaps[entry.path().filename().stem().string()] = ReadTexture(entry.path().string());
			//std::cout << "Loaded texture: " << entry.path().string() << '\n';
		}
	}

	for (const auto& entry : fs::directory_iterator(fs::path("textures/height"))) {
		if (entry.is_regular_file()) {
			heightMaps[entry.path().filename().stem().string()] = ReadTexture(entry.path().string());
			//std::cout << "Loaded texture: " << entry.path().string() << '\n';
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

			std::string color_texture = std::filesystem::path(mesh.MeshMaterial.map_Kd).stem().string();
			std::string normal_map = std::filesystem::path(mesh.MeshMaterial.map_bump).stem().string();
			std::string metallic_map = std::filesystem::path(mesh.MeshMaterial.refl).stem().string();
			std::string rougness_map = std::filesystem::path(mesh.MeshMaterial.map_Ns).stem().string();
			std::string height_map = std::filesystem::path(mesh.MeshMaterial.map_Ks).stem().string();

			RenderModel newModel(mesh);
			
			if (textures.count(color_texture) == 1) {
				newModel.diffuseMap = textures.at(color_texture);
			}
			if (normalMaps.count(normal_map) == 1) {
				newModel.normalMap = normalMaps.at(normal_map);
			}
			if (metallicMaps.count(metallic_map) == 1) {
				newModel.metallicMap = metallicMaps.at(metallic_map);
			}
			if (roughnessMaps.count(rougness_map) == 1) {
				newModel.roughnessMap = roughnessMaps.at(rougness_map);
			}
			if (heightMaps.count(height_map) == 1) {
				newModel.heightMap = heightMaps.at(height_map);
			}
			
			if (mesh.MeshMaterial.name != "textured") {
				newModel.shader = shaders.at("custom");
			}
			else {
				newModel.shader = shaders.at("textured");
			}
			
			models[newModel.name] = newModel;

			Collider collider(mesh);

			Object new_obj = Object(&models.at(newModel.name), collider);
			//new_obj.position = positions.at(newModel.name);
			objects.emplace_back(new_obj);

			std::cout << "Loaded model: " << newModel.name << "\n";

		}
	}
}

void GameEngine::LoadShaders()
{
	shaders["custom"] = new ShaderProgram("shaders/v_custom.glsl", nullptr, "shaders/f_custom.glsl");
	shaders["textured"] = new ShaderProgram("shaders/v_textured.glsl", nullptr, "shaders/f_textured.glsl");
}

void GameEngine::Update(float dt) {
	MouseInput();
	KeyInput();

	angle_x += rotateObjectsX * dt;
	angle_y += rotateObjectsY * dt;

	for (auto& object : objects) {
		object.SetRotation(angle_x, angle_y, 0);
	}

	player.Move(moveForward, moveSide, dt);
	player.Rotate(-mouseDelta.y, mouseDelta.x, deltaTime);
	camera.position = player.position + glm::vec3(0, 1, 0);
	camera.yaw = player.yaw;
	camera.pitch = player.pitch;

	// COllision detection
	for (auto& object : objects) {
		if (object.collider.PointInside(player.position + player.direction)) {
			player.position = player.prevPosition;
			player.speed = 0;

			break;
		}
	}

	float maxFloorLevel = -1000;
	for (auto& object : objects) {
		if (object.collider.PointInSquare(player.position)) {

			float currentObjectFloorLevel = object.collider.GetFloorLevel();

			if (currentObjectFloorLevel > maxFloorLevel)
			{
				maxFloorLevel = currentObjectFloorLevel;
			}

		}

	}
	player.floorLevel = maxFloorLevel;
	//std::cout << "FL: " << player.floorLevel << " POSY: " << player.position.y <<std::endl;

}



void GameEngine::Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glm::mat4 V = camera.GetViewMatrix();
	glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, aspectRatio, 0.01f, 1000.0f);


	for (auto& object : objects) {
		object.Render(V, P, camera.position);
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

	//player = new Player();
	//player.position = glm::vec3(0, 0, 0);

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
