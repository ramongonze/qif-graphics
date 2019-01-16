// Tags to compile -lglfw -lrt -lm -ldl -lGLEW -lGL

// Include standard headers
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

int main(){
	
	GLFWwindow *window;

	// Initialize the library
	if(!glfwInit()) exit(EXIT_FAILURE);


	// Create a windowed mode window and its OpenGl context
	window = glfwCreateWindow(640, 480, "Hello Bastards!", NULL, NULL);
	if(!window){
		glfwTerminate();
		exit(EXIT_FAILURE);	
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);

	if(glewInit() != GLEW_OK){
		fprintf(stderr, "Error initializing GLEW1\n");
		exit(EXIT_FAILURE);
	}
	
	cout << glGetString(GL_VERSION) << endl;

	float positions[6] = {
		-0.5f, -0.5f,
		0.0f, 0.5f,
		0.5f, -0.5f
	};

	unsigned int buffer;
	glGenBuffers(1, &buffer); // Generate abuffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer); // Select a buffer
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(positions). positions, GL_STATIC_DRAW); //  Specify the data

	// Loop until the user closese the window
	while(!glfwWindowShouldClose(window)){
		// Render here
		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_LINES);
			glVertex2f(-1.0f, -0.5f);
			glVertex2f(1.0f, -0.5f);
			glVertex2f(-0.5f, -1.0f);
			glVertex2f(-0.5f, 1.0f);
		glEnd();

		// Swap front and black buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();

	}
	 
	return 0;
}

