#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/ext.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>

typedef struct{
	unsigned int width;
	unsigned int height;
	const char* title;
	GLFWwindow* ID;
}RenderWindow;

typedef struct RenderModel{
	uint32_t VAO;
	uint32_t VBO;
	uint32_t EBO;
	uint32_t program;
	uint32_t elements;
	glm::mat4 modelMatrix;

	RenderModel()
	{
		modelMatrix = glm::mat4(1.0);
	}

}RenderModel;

typedef struct FlyCamera{
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::vec3 position;

	FlyCamera()
	{
		position = glm::vec3(0.0, 2.0, -10.0 );
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 Center = glm::vec3(0.0f, 0.0f, 0.0f);
		viewMatrix = glm::lookAt(position, Center, Up);

		float fov = glm::radians(45.0f);
		float aspect = 800/600;
		float near = 0.01f;
		float far = 10000.0f;
		projectionMatrix = glm::perspective(fov,aspect,near,far);
	}

}FlyCamera;

RenderWindow* createWindow(unsigned int width, unsigned int height, const char* title);
void destroyWindow(RenderWindow* window);
//
RenderModel* getScene(uint32_t program);
void destroyScene(RenderModel* scene);

uint32_t getShaderProgram(const char* vertex, const char* fragment);

void loadFromFile(std::string* file, const char* filePath);
bool updateEvents(RenderWindow* window);
void render(FlyCamera* camera, RenderModel* model);
bool isKeyPressed(RenderWindow* window, int keyCode);
void rotateModel(RenderModel* model, float angle, glm::vec3 axis);
void scaleModel(RenderModel* model, glm::vec3 scale);
void translateCamera(FlyCamera* camera, glm::vec3 position);
void translateModel(RenderModel* model, glm::vec3 translation);
int initLibraries();

int main(void)
{
	initLibraries();
	RenderWindow* window = createWindow(800,600,"GLRender");

	std::string vertexShader, fragmentShader;
	loadFromFile(&vertexShader, "./Vertex.glsl");
	loadFromFile(&fragmentShader, "./Fragment.glsl");

	uint32_t program = getShaderProgram(vertexShader.c_str(),fragmentShader.c_str());
	if(!program)
		return 0;

	RenderModel* cube = getScene(program);
	RenderModel* grownd = getScene(program);
	RenderModel* lProb = getScene(program);

	FlyCamera* camera = new FlyCamera;

	bool running = true;

	scaleModel(grownd, glm::vec3(7.0, 0.05, 7.0));
	scaleModel(lProb, glm::vec3(0.5, 0.5, 0.5));
	translateModel(grownd, glm::vec3(0.0, -2.0, 0.0));
	translateModel(lProb, glm::vec3(0.0,  2.0, 7.0));
	translateModel(cube, glm::vec3(0.0, 2.0, 0.0));

	while( running && updateEvents(window))
	{

		rotateModel(cube, 0.005f, glm::vec3(1.0, 0.0, 1.0));
		translateModel(lProb, glm::vec3(0.02, 0.0, 0.0f));
		rotateModel(lProb, 0.003f, glm::vec3(0.0f,1.0f,0.0f));

		if(isKeyPressed(window, GLFW_KEY_R))
			rotateModel(cube, 0.005f, glm::vec3(1.0, 0.0, 0.0));

		if(isKeyPressed(window, GLFW_KEY_ESCAPE))
			running = false;
		
		if(isKeyPressed(window, GLFW_KEY_LEFT))
			translateCamera(camera, glm::vec3(-0.01f, 0.0f, 0.0f));
		else if(isKeyPressed(window, GLFW_KEY_RIGHT))
			translateCamera(camera, glm::vec3( 0.01f, 0.0f, 0.0f));
		if(isKeyPressed(window,GLFW_KEY_UP))
			translateCamera(camera, glm::vec3(0.0f, 0.0f, -0.01f));
		else if(isKeyPressed(window, GLFW_KEY_DOWN))
			translateCamera(camera, glm::vec3(0.0f, 0.0f, 0.01f));
		if(isKeyPressed(window, GLFW_KEY_LEFT_CONTROL))
			translateCamera(camera, glm::vec3(0.0f,  0.01f, 0.0f));
		else if(isKeyPressed(window, GLFW_KEY_LEFT_SHIFT))
			translateCamera(camera, glm::vec3(0.0f, -0.01f, 0.0f));

		render(camera, cube);
		render(camera, grownd);
		render(camera, lProb);
	}

	destroyScene(cube);
	destroyScene(grownd);
	destroyScene(lProb);

	destroyWindow(window);
	return 0;
}

int initLibraries()
{
	if(!glfwInit())
	{
		fprintf(stderr,"Unable to initialize GLFW\n");
		return false;
	}
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	return true;
}

RenderWindow* createWindow(unsigned int width, unsigned int height, const char* title)
{
	RenderWindow* _window = new RenderWindow;

	GLFWwindow* ID = glfwCreateWindow(width,height,title,0,0);
	if(!ID)
	{
		fprintf(stderr, "Unable to create window\n");
		return nullptr;
	}

	glfwMakeContextCurrent(ID);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
    	std::cout << "Failed to initialize GLAD" << std::endl;
    	return nullptr;
	}

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	glfwShowWindow(ID);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	_window->width = width;
	_window->height = height;
	_window->title = title;
	_window->ID = ID;
	
	return _window;
}

void destroyWindow(RenderWindow* window)
{
	glfwDestroyWindow(window->ID);
	glfwTerminate();
	delete window;
}

bool updateEvents(RenderWindow* window)
{
	GLFWwindow* ID = window->ID;

	glfwSwapBuffers(ID);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glfwPollEvents();


	return !glfwWindowShouldClose(ID);
}

void loadFromFile(std::string* file, const char* filePath)
{
	std::ifstream ifs(filePath, std::ios::in);

	if(!ifs.is_open())
	{
		fprintf(stderr, "Unable to load file\n");
		return;
	}

	std::string line = "";
	while(!ifs.eof())
	{
		std::getline(ifs, line);
		file->append(line+"\n");
	}

	ifs.close();
}

int compileShader(uint32_t shader)
{
	glCompileShader(shader);
	int sucess;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &sucess);
	if(!sucess)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		fprintf(stderr, "Unable to compile shader: %s\n",infoLog);
		return false;
	}

	return true;
}


int linkProgram(uint32_t program)
{
	glLinkProgram(program);
	int sucess;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &sucess);
	if(!sucess)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		fprintf(stderr, "Unable to link program: %s\n",infoLog);
		return false;
	}

	return true;
}


uint32_t getShaderProgram(const char* vertex, const char* fragment)
{
	uint32_t vertexShader,fragmentShader;
	
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	glShaderSource(vertexShader, 1, &vertex, NULL);
	glShaderSource(fragmentShader, 1, &fragment, NULL);

	if(!compileShader(vertexShader) || !compileShader(fragmentShader))
	{
		return false;
	}

	uint32_t program;
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	if(!linkProgram(program))
	{
		return false;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

RenderModel* getScene(uint32_t program)
{
	uint32_t VAO, VBO, EBO;
	uint32_t VAC; // Vertex Attrib Colors ;-; 
	float vertices[] = 
	{
		// Front
		 0.5, -0.5,  0.5,
		-0.5, -0.5,  0.5,
		-0.5,  0.5,  0.5,
		 0.5,  0.5,  0.5,
		// Back
		 0.5, -0.5, -0.5,
		-0.5, -0.5, -0.5,
		-0.5,  0.5, -0.5,
		 0.5,  0.5, -0.5,
		// Left
		-0.5, -0.5,  0.5,
		-0.5, -0.5, -0.5,
		-0.5,  0.5, -0.5,
		-0.5,  0.5,  0.5,
		// Right
		 0.5, -0.5,  0.5,
		 0.5, -0.5, -0.5,
		 0.5,  0.5, -0.5,
		 0.5,  0.5,  0.5,
		// Top
		-0.5, -0.5,  0.5,
		-0.5, -0.5, -0.5,
		 0.5, -0.5, -0.5,
		 0.5, -0.5,  0.5,
		 // Down
		-0.5,  0.5,  0.5,
		-0.5,  0.5, -0.5,
		 0.5,  0.5, -0.5,
		 0.5,  0.5,  0.5
	};

	float colors[] = 
	{
		// Front
		 0.5, -0.5,  0.5,
		-0.5, -0.5,  0.5,
		-0.5,  0.5,  0.5,
		 0.5,  0.5,  0.5,
		// Back
		 0.5, -0.5, -0.5,
		-0.5, -0.5, -0.5,
		-0.5,  0.5, -0.5,
		 0.5,  0.5, -0.5,
		// Left
		-0.5, -0.5,  0.5,
		-0.5, -0.5, -0.5,
		-0.5,  0.5, -0.5,
		-0.5,  0.5,  0.5,
		// Right
		 0.5, -0.5,  0.5,
		 0.5, -0.5, -0.5,
		 0.5,  0.5, -0.5,
		 0.5,  0.5,  0.5,
		// Top
		-0.5, -0.5,  0.5,
		-0.5, -0.5, -0.5,
		 0.5, -0.5, -0.5,
		 0.5, -0.5,  0.5,
		 // Down
		-0.5,  0.0,  0.5,
		-0.5,  0.0, -0.5,
		 0.5,  0.0, -0.5,
		 0.5,  0.0,  0.5
	};

	uint32_t indices[] = 
	{
		// Front
		 0,  1,  2, 0,  2,  3,
		// Back
		 4,  5,  6, 4,  6,  7,
		// Left
		 8,  9, 10, 8, 10, 11,
		// Right
		12, 13, 14, 12, 14, 15,
		// Top
		16, 17, 18, 16, 18, 19,
		// Down
		20, 21, 22, 20, 22, 23
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VAC);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VAC);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	RenderModel* model = new RenderModel;
	model->VAO = VAO;
	model->VBO = VBO;
	model->EBO = EBO;
	model->program = program;
	model->elements = 36;

	return model;
}

void render(FlyCamera* camera, RenderModel* model)
{
	uint32_t modelMatrixLoc = glGetUniformLocation(model->program, "modelMatrix");
	uint32_t ViewMatrixLoc = glGetUniformLocation(model->program, "viewMatrix");
	uint32_t ProjectionMatrixLoc = glGetUniformLocation(model->program, "projectionMatrix");

	glUseProgram(model->program);
	glBindVertexArray(model->VAO);
	
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(model->modelMatrix));
	glUniformMatrix4fv(ViewMatrixLoc, 1, GL_FALSE, glm::value_ptr(camera->viewMatrix));
	glUniformMatrix4fv(ProjectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(camera->projectionMatrix));

	glDrawElements(GL_TRIANGLES, model->elements, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void rotateModel(RenderModel* model, float angle, glm::vec3 axis)
{
	model->modelMatrix = glm::rotate(model->modelMatrix, angle,axis);
}

bool isKeyPressed(RenderWindow* window, int keyCode)
{
	return glfwGetKey(window->ID, keyCode);
}

void translateCamera(FlyCamera* camera, glm::vec3 position)
{
	camera->viewMatrix = glm::translate(camera->viewMatrix, position);
}

void scaleModel(RenderModel* model, glm::vec3 scale)
{
	model->modelMatrix = glm::scale(model->modelMatrix, scale);
}

void translateModel(RenderModel* model, glm::vec3 translate)
{
	model->modelMatrix = glm::translate(model->modelMatrix, translate);
}

void destroyScene(RenderModel* scene)
{
	glDeleteBuffers(1, &scene->VAO);
	glDeleteBuffers(1, &scene->VBO);
	glDeleteBuffers(1, &scene->EBO);
	glDeleteProgram(scene->program);
	delete scene;
}
