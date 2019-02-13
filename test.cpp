// Tags: g++ test.cpp -lglfw -lGL -lX11 -lpthread -lXrandr -ldl /usr/local/src/glad.o /usr/lib/x86_64-linux-gnu/libdl.so

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

using namespace std;

/* A callbakc function used to reset the window coordinates when 
 * the user resizes the window. */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);  

/* This function is called in the main loop, and it threats
 * users' input (from keyboard, mouse, ...). */
void processInput(GLFWwindow *window);

/* Read a shader source from a file and compile the shader.
 * The second parameter must be "vertex" or "fragment". 
 * It returns the shader id. */
unsigned int compileShader(string file, string type);

/* Creat a program, attach and link the vertex and fragment shaders to it. */
unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader);

int main(){

	// Initialize GLFW Library
	if(glfwInit() == GL_FALSE){
		fprintf(stderr, "Error initializing GLFW\n");
		exit(EXIT_FAILURE);
	}

	// Tell to GLFW that we want to use OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell to GLFW that we want to use the OpenGL core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a window using GLFW
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL){
		fprintf(stderr, "Failed to create GLFW window\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Makes the context of the specified window current
	glfwMakeContextCurrent(window);
	
	/* Tell to GLFW we want to call the function "framebuffer_size_callback" 
	 * when the user resizes the window */
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		fprintf(stderr, "Failed to initialize GLAD\n");
		exit(EXIT_FAILURE);
	}

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	unsigned int vertexShader = compileShader("vertexShader.glsl", "vertex");
	unsigned int fragmentShader = compileShader("fragmentShader.glsl", "fragment");
	unsigned int fragmentShader2 = compileShader("fragmentShader2.glsl", "fragment");

	unsigned int shaderProgram = createProgram(vertexShader, fragmentShader);
	unsigned int shaderProgram2 = createProgram(vertexShader, fragmentShader2);
	float vertices1[] = {
	     -0.5f, -0.5f, 0.0f,
	     0.0f, 0.5f, 0.0f,
	     0.5f, -0.5f, 0.0f,
	     -0.5f, -0.5f, 0.0f
	};

	float vertices2[] = {
	     0.0f, 0.0f, 0.0f,
	     0.5f, 1.0f, 0.0f,
	     1.0f, 0.5f, 0.0f
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	unsigned int VAO[2], VBO[2], EBO;
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(1, &EBO);

	// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
	// 1. bind Vertex Array Object
	glBindVertexArray(VAO[0]);
		// 2. copy our vertices array in a buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

		// 2. copy our vertices' indexes in an element buffer for OpenGL to use
		// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// 3. then set the vertex attributes pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	glBindVertexArray(VAO[1]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while(!glfwWindowShouldClose(window)){
		// Input
		processInput(window);
		
		// Rendering commands here
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw
		// 4. use our shader program when we want to render an object
		glUseProgram(shaderProgram);
			glBindVertexArray(VAO[0]);
				glDrawArrays(GL_TRIANGLES, 0, 3);
				// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0); // Unbind the current VAO

		glUseProgram(shaderProgram2);
			glBindVertexArray(VAO[1]);
				glDrawArrays(GL_TRIANGLES, 0, 3);
			glBindVertexArray(0);

		// Check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();    
	}

	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window){

	// If the user presses the key ESC, close the window
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

unsigned int compileShader(string file, string type){
	
	char c;
	string src;
	unsigned int shader;

	// Reading
	FILE* f = fopen(file.c_str(), "r");
	while(fscanf(f, "%c", &c) != EOF)
		src += c;

	fclose(f);

	const char *str = src.c_str();

	// Compiling
	if(type == "vertex"){
		shader = glCreateShader(GL_VERTEX_SHADER);
	}else if(type == "fragment"){
		shader = glCreateShader(GL_FRAGMENT_SHADER);
	}else{
		fprintf(stderr, "%s: Invalid type of shader!\n", type.c_str());
		exit(EXIT_FAILURE);
	}

	glShaderSource(shader, 1, &str, NULL);
	glCompileShader(shader);

	// Checking if an error ocurred when compiling the shader
	int status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if(status != GL_TRUE){
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		fprintf(stderr, "Error compiling %s shader\n%s\n", type.c_str(), infoLog);
	}

	return shader;
}

unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader){

	// Create a program object
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check if the linking was successfully
	int status;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
	if(status != GL_TRUE){
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		fprintf(stderr, "Error linking shaders to a program: %s\n", infoLog);
	}

	// glDeleteShader(vertexShader);
	// glDeleteShader(fragmentShader);

	return shaderProgram;
}