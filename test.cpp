#include "./include/shader.hpp"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

using namespace std;

/* A callbakc function used to reset the window coordinates when 
 * the user resizes the window. */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);  

/* This function is called in the main loop, and it threats
 * users' input (from keyboard, mouse, ...). */
void processInput(GLFWwindow *window);

int main(){

	// Initialize GLFW Library
	if(glfwInit() == GL_FALSE){
		fprintf(stderr, "Error initializing GLFW\n");
		exit(EXIT_FAILURE);
	}

	GLFWmonitor* primary = glfwGetPrimaryMonitor();

	// Tell to GLFW that we want to use OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell to GLFW that we want to use the OpenGL core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a window using GLFW
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "QIF Graphics", NULL, NULL);
	if (window == NULL){
		fprintf(stderr, "Failed to create GLFW window\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetWindowPos(window, 300, 100);

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

	Shader mainShader("vertexShader.glsl", "fragmentShader.glsl");

	float vertices[] = {
	    -0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,
	     0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	     0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
		// 2. copy our vertices array in a buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// 3. then set the vertex attributes pointers
		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// Color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

	// glBindVertexArray(0);

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while(!glfwWindowShouldClose(window)){
		// Input
		processInput(window);
		
		// Rendering commands here
		glClearColor(0.85f, 0.85f, 0.85f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw
		// 4. use our shader program when we want to render an object
		mainShader.use();
		glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		// glBindVertexArray(0); // Unbind the current VAO

		// Check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();    
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glfwSetWindowAspectRatio(window, 1, 1);
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window){

	// If the user presses the key ESC, close the window
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}