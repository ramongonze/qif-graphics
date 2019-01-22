// Tags to compile -lglfw -lrt -lm -ldl -lGLEW -lGL

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "./include/graphics.hpp"
#include <cmath>
#include <exception>

#define GLEW_STATIC
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define CIRCLES_SIDES 300
#define PRIOR_RADIUS 0.1f

// Shaders
#define MAIN_V_SHADER "./shaders/main_v_shader.glsl"
#define MAIN_F_SHADER "./shaders/main_f_shader.glsl"

/* Read a shader source from a file and compile the shader.
 * The second parameter must be "vertex" or "fragment". 
 * It returns the shader id. */
GLuint compileShader(string file, string type);

double* createCircle(GLdouble x, GLdouble y, GLdouble radius, double r, double g, double b);

Point* mouseCoord2ScreenCoord(double x, double y);

int main(){
	
	GLFWwindow *window;
	GLint posAttrib;
	GLint colAttrib;

	// Initialize the library
	if(!glfwInit()){
		fprintf(stderr, "Error initializing GLFW!\n");
		exit(EXIT_FAILURE);
	}

	// Specify that the required OpenGL version is 3.2
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	/* GLFW_OPENGL_CORE_PROFILE specifies that we want a context that 
	 * supports the new core functionality. */
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create a windowed mode window and its OpenGl context
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "QIF Graphics", NULL, NULL);
	if(!window){
		fprintf(stderr, "Error creating a window!\n");
		glfwTerminate();
		exit(EXIT_FAILURE);	
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK){
		fprintf(stderr, "Error initializing GLEW!\n");
		exit(EXIT_FAILURE);
	}

	GLuint vaoTriangle;
	glGenVertexArrays(1, &vaoTriangle);
	glBindVertexArray(vaoTriangle);
	
	// Creating shaders and the program
	GLuint vertexShader = compileShader(MAIN_V_SHADER, "vertex");
	GLuint fragmentShader = compileShader(MAIN_F_SHADER, "fragment");

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	/* Since a fragment shader is allowed to write to multiple buffers, you need to
	 * explicitly specify which output is written to which buffer. This needs to happen
	 * before linking the program. */
	glBindFragDataLocation(shaderProgram, 0, "outColor");

	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram); // Only one program can be active at a time.

	double main_triangle[] = {
		-0.5, -0.5, 0.0f, 0.0f, 0.0f,
		0.0, 0.5, 0.0f, 0.0f, 0.0f,
		0.0, 0.5, 0.0f, 0.0f, 0.0f,
		0.5, -0.5, 0.0f, 0.0f, 0.0f,
		0.5, -0.5, 0.0f, 0.0f, 0.0f,
		-0.5, -0.5, 0.0f, 0.0f, 0.0f
	};

	GLuint vboTriangle;
	glGenBuffers(1, &vboTriangle); // Generate 1 buffer
	glBindBuffer(GL_ARRAY_BUFFER, vboTriangle); // Select a buffer to put some data in it
	glBufferData(GL_ARRAY_BUFFER, sizeof(main_triangle), main_triangle, GL_STATIC_DRAW);
	
	posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 2, GL_DOUBLE, GL_FALSE, 5 * sizeof(double), 0);
	glEnableVertexAttribArray(posAttrib);
	
	colAttrib = glGetAttribLocation(shaderProgram, "Color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_DOUBLE, GL_FALSE, 5 * sizeof(double), (void*)(2*sizeof(double)));

	// QIF ----------------
	Hyper hyper("prior", "channel");
	Point *p = dist2BaryCoord(*(hyper.prior));
	// --------------------

	double **circlesVertices = (double**) malloc((hyper.channel->num_out+1) * sizeof(double*));

	GLuint *vaoCircles = (GLuint*) malloc((hyper.channel->num_out+1) * sizeof(GLuint));
	glGenVertexArrays(hyper.channel->num_out+1, vaoCircles);
	glBindVertexArray(vaoCircles[0]);

	// Prior vertices
	circlesVertices[0] = createCircle(p->x + ORIGIN_X, p->y + ORIGIN_Y, PRIOR_RADIUS, 0.274f, 0.509f, 0.705);

	GLuint *vboCircles = (GLuint*) malloc((hyper.channel->num_out+1) * sizeof(GLuint));
	glGenBuffers(hyper.channel->num_out+1, vboCircles); // Generate 1 buffer

	// Prior distribution buffer
	glBindBuffer(GL_ARRAY_BUFFER, vboCircles[0]); // Select a buffer to put some data in it
	glBufferData(GL_ARRAY_BUFFER, 3 * 2 * (CIRCLES_SIDES+2) * sizeof(double), circlesVertices[0], GL_STATIC_DRAW);
	
	posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 2, GL_DOUBLE, GL_FALSE, 5 * sizeof(double), 0);
	glEnableVertexAttribArray(posAttrib);

	colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_DOUBLE, GL_FALSE, 5 * sizeof(double), (void*)(2*sizeof(double)));

	free(p);

	// Inners distributions buffers
	for(int i = 0; i < hyper.channel->num_out; i++){
		glBindVertexArray(vaoCircles[i+1]);
		
		long double x1 = hyper.inners[0][i];
		long double x2 = hyper.inners[1][i];
		long double x3 = hyper.inners[2][i];
		long double radius = sqrt(hyper.outer.prob[i] * PRIOR_RADIUS * PRIOR_RADIUS);

		p = dist2BaryCoord(x1,x2,x3);

		circlesVertices[i+1] = createCircle(p->x + ORIGIN_X, p->y + ORIGIN_Y, radius, 0.196f, 0.803f, 0.196f);

		glBindBuffer(GL_ARRAY_BUFFER, vboCircles[i+1]); // Select a buffer to put some data in it
		glBufferData(GL_ARRAY_BUFFER, 3 * 2 * (CIRCLES_SIDES+2) * sizeof(double), circlesVertices[i+1], GL_STATIC_DRAW);
		
		posAttrib = glGetAttribLocation(shaderProgram, "position");
		glVertexAttribPointer(posAttrib, 2, GL_DOUBLE, GL_FALSE, 5 * sizeof(double), 0);
		glEnableVertexAttribArray(posAttrib);

		colAttrib = glGetAttribLocation(shaderProgram, "color");
		glEnableVertexAttribArray(colAttrib);
		glVertexAttribPointer(colAttrib, 3, GL_DOUBLE, GL_FALSE, 5 * sizeof(double), (void*)(2*sizeof(double)));

		free(p);
	}

	/* Use glDrawBuffers when rendering to multiple buffers, because only
	 * the first output will be enabled by default. */
	bool movePrior = false;
	double old_x, old_y, moveX, moveY;
	// Loop until the user closese the window
	while(!glfwWindowShouldClose(window)){
		// Render here
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(vaoTriangle);
		glDrawArrays(GL_LINES, 0, 6);

		// Check if the user is trying to move the prior
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		Point *mousePos = mouseCoord2ScreenCoord(xpos, ypos);
			
		Point priorPos;
		priorPos.x = circlesVertices[0][0];
		priorPos.y = circlesVertices[0][1];

		// Check if the mouse left button is pressed
		int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		if(state == GLFW_PRESS && movePrior == false && euclidianDistance(*mousePos, priorPos) <= PRIOR_RADIUS){
			movePrior = true;
			old_x = mousePos->x;
			old_y = mousePos->y;
		}else if(state == GLFW_PRESS){
			moveX = mousePos->x - old_x;
			moveY = mousePos->y - old_y;
			old_x = mousePos->x;
			old_y = mousePos->y;
		}else{
			movePrior = false;
		}

		if(movePrior){
			// Move the prior
			for(int i = 0; i < CIRCLES_SIDES+2; i++){
				circlesVertices[0][(i * 5)] += moveX; // X
				circlesVertices[0][(i * 5)+1] += moveY; // Y
			}

			if(circlesVertices[0][0] < -1.0f)
				circlesVertices[0][0] = -1.0f;
			if(circlesVertices[0][1] < -1.0f)
				circlesVertices[0][1] = -1.0f;
			if(circlesVertices[0][0] > 1.0f)
				circlesVertices[0][0] = 1.0f;
			if(circlesVertices[0][1] > 1.0f)
				circlesVertices[0][1] = 1.0f;

			Point new_prior;
			new_prior.x = circlesVertices[0][0];
			new_prior.y = circlesVertices[0][1];
			*(hyper.prior) = baryCoord2Dist(new_prior);
			hyper.channel = new Channel(*(hyper.prior), hyper.channel->matrix);
			hyper.buildHyper(*(hyper.prior), *(hyper.channel));

			// Update inners distributions buffers
			for(int i = 0; i < hyper.channel->num_out; i++){
				glBindVertexArray(vaoCircles[i+1]);
				
				long double x1 = hyper.inners[0][i];
				long double x2 = hyper.inners[1][i];
				long double x3 = hyper.inners[2][i];
				long double radius = sqrt(hyper.outer.prob[i] * PRIOR_RADIUS * PRIOR_RADIUS);

				p = dist2BaryCoord(x1,x2,x3);

				free(circlesVertices[i+1]);
				circlesVertices[i+1] = createCircle(p->x + ORIGIN_X, p->y + ORIGIN_Y, radius, 0.196f, 0.803f, 0.196f);

				glBindBuffer(GL_ARRAY_BUFFER, vboCircles[i+1]); // Select a buffer to put some data in it
				glBufferData(GL_ARRAY_BUFFER, 3 * 2 * (CIRCLES_SIDES+2) * sizeof(double), circlesVertices[i+1], GL_STATIC_DRAW);
				
				posAttrib = glGetAttribLocation(shaderProgram, "position");
				glVertexAttribPointer(posAttrib, 2, GL_DOUBLE, GL_FALSE, 5 * sizeof(double), 0);
				glEnableVertexAttribArray(posAttrib);

				colAttrib = glGetAttribLocation(shaderProgram, "color");
				glEnableVertexAttribArray(colAttrib);
				glVertexAttribPointer(colAttrib, 3, GL_DOUBLE, GL_FALSE, 5 * sizeof(double), (void*)(2*sizeof(double)));

				free(p);
			}

			glBindVertexArray(vaoCircles[0]);

			glBindBuffer(GL_ARRAY_BUFFER, vboCircles[0]); // Select a buffer to put some data in it
			glBufferData(GL_ARRAY_BUFFER, 3 * 2 * (CIRCLES_SIDES+2) * sizeof(double), circlesVertices[0], GL_STATIC_DRAW);
			
			posAttrib = glGetAttribLocation(shaderProgram, "position");
			glVertexAttribPointer(posAttrib, 2, GL_DOUBLE, GL_FALSE, 5 * sizeof(double), 0);
			glEnableVertexAttribArray(posAttrib);
		}

		free(mousePos);

		for(int i = 0; i < hyper.channel->num_out+1; i++){
			glBindVertexArray(vaoCircles[i]);
			glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLES_SIDES+2);
		}

		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}

GLuint compileShader(string file, string type){
	
	char c;
	string src;
	GLuint shader;

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
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if(status != GL_TRUE){
		char buffer[512];
		glGetShaderInfoLog(shader, 512, NULL, buffer);
		fprintf(stderr, "Error compiling %s shader\n%s\n", type.c_str(), buffer);
		exit(EXIT_FAILURE);
	}

	return shader;
}

double* createCircle(GLdouble x, GLdouble y, GLdouble radius, double r, double g, double b){
	// Parameters r,g,b are the circle's color

	int numberOfVertices = CIRCLES_SIDES + 2;
    
	GLdouble twicePi = 2.0f * M_PI;
    
	GLdouble *circleVerticesX = (GLdouble*) malloc(numberOfVertices * sizeof(double));
	GLdouble *circleVerticesY = (GLdouble*) malloc(numberOfVertices * sizeof(double));
	
	circleVerticesX[0] = x;
	circleVerticesY[0] = y;
	
	for(int i = 1; i < numberOfVertices; i++){
		GLdouble ang = twicePi/CIRCLES_SIDES * i;
		circleVerticesX[i] = x + (radius * cos(ang));
		circleVerticesY[i] = y + (radius * sin(ang));
	}
	
	double *allCircleVertices = (double*) malloc(numberOfVertices * 2 * 3 * sizeof(double));
	
	for(int i = 0; i < numberOfVertices; i++){
		allCircleVertices[i * 5] = circleVerticesX[i];
		allCircleVertices[(i * 5) + 1] = circleVerticesY[i];
		
		if(i == 0){
			allCircleVertices[(i * 5) + 2] = r + 0.3;
			allCircleVertices[(i * 5) + 3] = g + 0.3;
			allCircleVertices[(i * 5) + 4] = b + 0.3;
		}else{
			allCircleVertices[(i * 5) + 2] = r;
			allCircleVertices[(i * 5) + 3] = g;
			allCircleVertices[(i * 5) + 4] = b;
		}
	}
	
	free(circleVerticesX);
	free(circleVerticesY);

	return allCircleVertices;
}

Point* mouseCoord2ScreenCoord(double x, double y){
	Point *p = (Point*) malloc(sizeof(Point));

	if(x < SCREEN_WIDTH/2.0f){
		if(x < 0) p->x = -1;
		else p->x = -1+(x/(SCREEN_WIDTH/2.0f));
	}else{
		x -= (SCREEN_WIDTH/2.0f);
		if(x > SCREEN_WIDTH/2.0f) p->x = 1;
		else p->x = x/(SCREEN_WIDTH/2.0f);
	}

	if(y < SCREEN_HEIGHT/2.0f){
		if(y < 0) p->y = 1;
		else p->y = 1- (y/(SCREEN_HEIGHT/2.0f));
	}else{
		y -= (SCREEN_HEIGHT/2.0f);
		if(y > SCREEN_HEIGHT/2.0f) p->y = -1;
		else p->y = -(y/(SCREEN_HEIGHT/2.0f));
	}

	return p;
}
