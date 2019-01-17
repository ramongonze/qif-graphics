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
// #include <glm/gtx/transform.hpp>

// QIF Library
#include "qif/qif.hpp"

using namespace std;
using namespace glm;

typedef struct Point{
	long double x;
	long double y;
}Point;

Point* dist2BaryCoord(Distribution &prior){
	Point *baryc = (Point*) malloc(sizeof(Point));

	baryc->x = prior.prob[2] + prior.prob[0]/2.0f;
	baryc->y = prior.prob[0];

	return baryc;
}

Point* dist2BaryCoord(long double x, long double y, long double z){
	Point *baryc = (Point*) malloc(sizeof(Point));

	baryc->x = z + x/2.0f;
	baryc->y = x;

	return baryc;
}

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
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(positions), positions, GL_STATIC_DRAW); //  Specify the data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);

	Hyper hyper("prior", "channel");

	Point *p = dist2BaryCoord(*(hyper.prior));
	Point **inners = (Point**) malloc(hyper.channel->num_out * sizeof(Point*));

	for(int i = 0; i < hyper.channel->num_out; i++){
		long double x, y, z;
		x = hyper.inners[0][i];
		y = hyper.inners[1][i];
		z = hyper.inners[2][i];

		inners[i] = dist2BaryCoord(x,y,z);
		printf("(%Lf,%Lf))\n", inners[i]->x, inners[i]->y);
	}

	cout << hyper.toString("inners", 3) << endl;

	// Loop until the user closese the window
	while(!glfwWindowShouldClose(window)){

		// Render here
		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_POLYGON);
			glVertex2f(p->x -0.05f,p->y - 0.05f);
			glVertex2f(p->x + 0.05f,p->y - 0.05f);
			glVertex2f(p->x + 0.05f,p->y + 0.05f);
			glVertex2f(p->x - 0.05f,p->y + 0.05f);
		glEnd();

		

		for(int i = 0; i < hyper.channel->num_out; i++){
			// printf("(%Lf, %Lf)\n", inners[i]->x, inners[i]->y);
			glBegin(GL_POLYGON);
				glVertex2f(inners[i]->x -0.05f,inners[i]->y - 0.05f);
				glVertex2f(inners[i]->x + 0.05f,inners[i]->y - 0.05f);
				glVertex2f(inners[i]->x + 0.05f,inners[i]->y + 0.05f);
				glVertex2f(inners[i]->x - 0.05f,inners[i]->y + 0.05f);
			glEnd();
		}

		glBegin(GL_LINES);
			glVertex2f(-1,0);
			glVertex2f(1,0);
		glEnd();

		glBegin(GL_LINES);
			glVertex2f(0,-1);
			glVertex2f(0,1);
		glEnd();

		// glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap front and black buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();

	}
	 
	return 0;
}

