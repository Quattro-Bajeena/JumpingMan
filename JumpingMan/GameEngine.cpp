#include "GameEngine.h"
#include "myCube.h"


void GameEngine::ErrorCallback(int error, const char* description) {
	fputs(description, stderr);
}

void GameEngine::WindowResizeCallback(GLFWwindow* window, int width, int height) {
	GameEngine* engine = (GameEngine*)glfwGetWindowUserPointer(window);
	if (height == 0) return;
	engine->aspectRatio = (float)width / (float)height;
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
	engine->KeyInput(key, scancode, action, mod);
}

void GameEngine::KeyInput(int key, int scancode, int action, int mod)
{
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) {
			speed_y = -PI;
		}
		if (key == GLFW_KEY_RIGHT) {
			speed_y = PI;
		}
		if (key == GLFW_KEY_UP) {
			speed_x = -PI;
		}
		if (key == GLFW_KEY_DOWN) {
			speed_x = PI;
		}
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) {
			speed_y = 0;
		}
		if (key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) {
			speed_x = 0;
		}
	}
}



GLuint GameEngine::ReadTexture(const char* filename) {
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
	//initShaders();

	glClearColor(0, 0, 0, 1); //Ustaw kolor czyszczenia bufora kolorów
	glEnable(GL_DEPTH_TEST); //Włącz test głębokości na pikselach
	glfwSetKeyCallback(window, KeyCallBack);
	tex = ReadTexture("textures/bricks.png");

	sp = new ShaderProgram("shaders/v_simplest.glsl", NULL, "shaders/f_simplest.glsl");
	//model = RenderModel(myCubeVertices, myCubetexCoords, myCubeVertexCount, tex, sp);
	model = RenderModel();
	model.LoadFromFile("models/teapot.obj");
	model.SetTexture(tex);
	model.SetShader(sp);
}


//Zwolnienie zasobów zajętych przez program
void GameEngine::FreeOpenGLProgram() {
	freeShaders();
	glDeleteTextures(1, &tex);
	//************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
}

void GameEngine::Update(float dt) {
	angle_x += speed_x * dt;
	angle_y += speed_y * dt;
}

void GameEngine::Render() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczyść bufor koloru i bufor głębokości

	glm::mat4 V = glm::lookAt(glm::vec3(0, 0, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 P = glm::perspective(glm::radians(50.0f), aspectRatio, 0.1f, 50.0f);


	model.rotation = glm::quat(glm::vec3(angle_x, angle_y, 0));
	model.Render(V, P);


	glfwSwapBuffers(window);
}

GameEngine::GameEngine() {

	glfwSetErrorCallback(ErrorCallback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	int width = 1000;
	int height = 1000;
	window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.
	aspectRatio = (float)width / height;

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetWindowUserPointer(window, this);
	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	InitOpenGLProgram(); //Operacje inicjujące

	
	glfwSetTime(0); //Wyzeruj licznik czasu
	lastTime = 0;
	
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
