#include "./include/shader.hpp"
#include "./include/graphics.hpp"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

#define MAIN_V_SHADER "./shaders/main_v_shader.glsl"
#define MAIN_F_SHADER "./shaders/main_f_shader.glsl"

using namespace std;

/* A callbakc function used to reset the window coordinates when 
 * the user resizes the window. */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);  

/* This function is called in the main loop, and it threats
 * users' input (from keyboard, mouse, ...). */
void processInput(GLFWwindow *window, unsigned int *VAO, unsigned int *VBO, Hyper &hyper, bool *moveCircles, Point &oldMousePos);

/* Using the given hyper distribution, create the circles for prior and posterior distributions */
void buildCircles(unsigned int *VAO, unsigned int *VBO, Hyper &hyper);

int main(){

	// Auxiliar variables
	Point p;

	unsigned int *VAO, *VBO;

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
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "QIF Graphics", NULL, NULL);
	if (window == NULL){
		fprintf(stderr, "Failed to create GLFW window\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Set the window position to the center of the monitor
	const GLFWvidmode *video = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(window, (video->width - WINDOW_WIDTH)/2, (video->height - WINDOW_HEIGHT)/2);

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

	Shader mainShader(MAIN_V_SHADER, MAIN_F_SHADER);

	float mainTriangle[] = {
	    0.0f+ORIGIN_X, 0.0f+ORIGIN_Y, 0.0f, 0.0f, 0.0f, 0.0f,
	    0.5f+ORIGIN_X, 1.0f+ORIGIN_Y, 0.0f, 0.0f, 0.0f, 0.0f,
	    1.0f+ORIGIN_X, 0.0f+ORIGIN_Y, 0.0f, 0.0f, 0.0f, 0.0f
	};

	// QIF ----------------
	Hyper hyper("prior", "channel");
	// --------------------
	
	VAO = (unsigned int *) malloc((hyper.channel->num_out + 2) * sizeof(unsigned int));
	VBO = (unsigned int *) malloc((hyper.channel->num_out + 2) * sizeof(unsigned int));

	/* VAO[0] -> Main triangle
	 * VAO[1] -> Prior
	 * VAO[2]...VAO[n] -> Posteriors */
	glGenVertexArrays(hyper.channel->num_out + 2, VAO);
	glGenBuffers(hyper.channel->num_out + 2, VBO);
	
	// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
	// 1. bind Vertex Array Object
	glBindVertexArray(VAO[0]);
		// 2. copy our vertices array in a buffer for OpenGL to use
		// Main Triangle
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(mainTriangle), mainTriangle, GL_STATIC_DRAW);
		// 3. then set the vertex attributes pointers
		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// Color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	bool moveCircles = false;
	Point oldMousePos;
	buildCircles(VAO, VBO, hyper);
	while(!glfwWindowShouldClose(window)){
		// Input
		processInput(window, VAO, VBO, hyper, &moveCircles, oldMousePos);
		
		// Rendering commands here
		glClearColor(0.85f, 0.85f, 0.85f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw ---------------------------------
			mainShader.use();
			
			// Main Triangle
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glBindVertexArray(VAO[0]);			
				glDrawArrays(GL_TRIANGLES, 0, 3);
			glBindVertexArray(0);

			// Circles
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			for(int i = 1; i <= hyper.channel->num_out+1; i++){
				glBindVertexArray(VAO[i]);
					glDrawArrays(GL_TRIANGLE_FAN, 0, (CIRCLES_SIDES+2));
				glBindVertexArray(0);
			}
		// --------------------------------------

		// Check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();    
	}

	glDeleteVertexArrays(hyper.channel->num_out+2, VAO);
	glDeleteBuffers(hyper.channel->num_out+2, VBO);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	// glfwSetWindowAspectRatio(window, 1, 1);
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, unsigned int *VAO, unsigned int *VBO, Hyper &hyper, bool *moveCircles, Point &oldMousePos){

    Point mousePos, priorPos, newPriorPos;

	// If the user presses the key ESC, close the window
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // If the user presses the mouse left button
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
    	// Get mouse position
    	double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		pixelCoord2ScreenCoord(xpos, ypos, mousePos);

		// Get prior position
		dist2BaryCoord(*(hyper.prior), priorPos);
		priorPos.x += ORIGIN_X;
		priorPos.y += ORIGIN_Y;

		if(*moveCircles == false && euclidianDistance(mousePos, priorPos) <= PRIOR_RADIUS){
			*moveCircles = true;
			oldMousePos.x = mousePos.x;
			oldMousePos.y = mousePos.y;
		}else{
			newPriorPos.x = mousePos.x - oldMousePos.x;
			newPriorPos.y = mousePos.y - oldMousePos.y;
			oldMousePos.x = mousePos.x;
			oldMousePos.y = mousePos.y;
		}
    }else{
    	*moveCircles = false;
    }

    if(*moveCircles == true){
    	vector<long double> prob(3);
    	newPriorPos.x += priorPos.x;
    	newPriorPos.y += priorPos.y;

    	// Check if the new vector of 3 elements make up a probability distribution
    	if(baryCoord2Dist(newPriorPos, prob)){
    		baryCoord2Dist(newPriorPos, hyper.prior->prob);
			hyper.channel = new Channel(*(hyper.prior), hyper.channel->matrix);
			hyper.buildHyper(*(hyper.prior), *(hyper.channel));

			buildCircles(VAO, VBO, hyper);
    	}
    }
}

void buildCircles(unsigned int *VAO, unsigned int *VBO, Hyper &hyper){

	/* Used to draw circles
	 * 6 = 3 position coordinates + 3 rgb color 
	 * CIRCLES_SIDES + 2 = number of vertices in a circle */
	double circleVertices[6 * (CIRCLES_SIDES+2)];
	Point p;

	// Prior distribution buffer
	glBindVertexArray(VAO[1]);
		dist2BaryCoord(*(hyper.prior), p);
		createCircle(p.x + ORIGIN_X, p.y + ORIGIN_Y, PRIOR_RADIUS, 0.274f, 0.509f, 0.705, circleVertices);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices), circleVertices, GL_STATIC_DRAW);
		// Position attribute
		glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), (void*)0);
		glEnableVertexAttribArray(0);
		// Color attribute
		glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), (void*)(3 * sizeof(double)));
		glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	// Inners distributions buffers
	for(int i = 0; i < hyper.channel->num_out; i++){
		long double x1 = hyper.inners[0][i];
		long double x2 = hyper.inners[1][i];
		long double x3 = hyper.inners[2][i];
		long double radius = sqrt(hyper.outer.prob[i] * PRIOR_RADIUS * PRIOR_RADIUS);

		dist2BaryCoord(x1,x2,x3,p);

		createCircle(p.x + ORIGIN_X, p.y + ORIGIN_Y, radius, 0.196f, 0.803f, 0.196f, circleVertices);

		glBindVertexArray(VAO[i+2]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[i+2]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices), circleVertices, GL_STATIC_DRAW);
			// Position attribute
			glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), (void*)0);
			glEnableVertexAttribArray(0);
			// Color attribute
			glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), (void*)(3 * sizeof(double)));
			glEnableVertexAttribArray(1);
		glBindVertexArray(0);
	}
}
