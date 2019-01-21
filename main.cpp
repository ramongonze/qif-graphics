// Tags to compile -lglfw -lrt -lm -ldl -lGLEW -lGL

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "graphics.hpp"
#include <cmath>
#include <exception>

#define GLEW_STATIC
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define CIRCLES_SIDES 50

/* Read a shader source from a file and compile the shader.
 * The second parameter must be "vertex" or "fragment". 
 * It returns the shader id. */
GLuint compileShader(string file, string type);

double* createCircle(GLdouble x, GLdouble y, GLdouble radius);

int main(){
	
	GLFWwindow *window;
	GLint posAttrib;

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
	GLuint vertexShader = compileShader("vertexShader.glsl", "vertex");
	GLuint fragmentShader = compileShader("fragmentShader.glsl", "fragment");

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
		-0.5, -0.5,
		0.0, 0.5,
		0.0, 0.5,
		0.5, -0.5,
		0.5, -0.5,
		-0.5, -0.5,
	};

	GLuint vboTriangle;
	glGenBuffers(1, &vboTriangle); // Generate 1 buffer
	glBindBuffer(GL_ARRAY_BUFFER, vboTriangle); // Select a buffer to put some data in it
	glBufferData(GL_ARRAY_BUFFER, sizeof(main_triangle), main_triangle, GL_STATIC_DRAW);
	posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 2, GL_DOUBLE, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);

	// QIF ----------------
	Hyper hyper("prior", "channel");
	Point *p = dist2BaryCoord(*(hyper.prior));
	// --------------------

	double **circlesVertices = (double**) malloc((hyper.channel->num_out+1) * sizeof(double*));

	GLuint *vaoCircles = (GLuint*) malloc((hyper.channel->num_out+1) * sizeof(GLuint));
	glGenVertexArrays(hyper.channel->num_out+1, vaoCircles);
	glBindVertexArray(vaoCircles[0]);

	// Prior vertices
	circlesVertices[0] = createCircle(p->x + ORIGIN_X, p->y + ORIGIN_Y, 0.1);

	GLuint *vboCircles = (GLuint*) malloc((hyper.channel->num_out+1) * sizeof(GLuint));
	glGenBuffers(hyper.channel->num_out+1, vboCircles); // Generate 1 buffer

	// Prior distribution buffer
	glBindBuffer(GL_ARRAY_BUFFER, vboCircles[0]); // Select a buffer to put some data in it
	glBufferData(GL_ARRAY_BUFFER, 2 * (CIRCLES_SIDES+2) * sizeof(double), circlesVertices[0], GL_STATIC_DRAW);
	posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 2, GL_DOUBLE, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);

	free(p);

	for(int i = 0; i < hyper.channel->num_out; i++){
		glBindVertexArray(vaoCircles[i+1]);
		
		long double x1 = hyper.inners[0][i];
		long double x2 = hyper.inners[1][i];
		long double x3 = hyper.inners[2][i];
		long double radius = 0.1 * hyper.outer.prob[i];

		p = dist2BaryCoord(x1,x2,x3);

		circlesVertices[i+1] = createCircle(p->x + ORIGIN_X, p->y + ORIGIN_Y, radius);

		glBindBuffer(GL_ARRAY_BUFFER, vboCircles[i+1]); // Select a buffer to put some data in it
		glBufferData(GL_ARRAY_BUFFER, 2 * (CIRCLES_SIDES+2) * sizeof(double), circlesVertices[i+1], GL_STATIC_DRAW);
		posAttrib = glGetAttribLocation(shaderProgram, "position");
		glVertexAttribPointer(posAttrib, 2, GL_DOUBLE, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(posAttrib);

		free(p);
	}

	/* Use glDrawBuffers when rendering to multiple buffers, because only
	 * the first output will be enabled by default. */
	// Loop until the user closese the window
	while(!glfwWindowShouldClose(window)){
		// Render here
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(vaoTriangle);
		glDrawArrays(GL_LINES, 0, 6);

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

double* createCircle(GLdouble x, GLdouble y, GLdouble radius){
	int numberOfVertices = CIRCLES_SIDES + 2;
    
	GLdouble twicePi = 2.0f * M_PI;
    
	GLdouble *circleVerticesX = (GLdouble*) malloc(numberOfVertices * sizeof(double));
	GLdouble *circleVerticesY = (GLdouble*) malloc(numberOfVertices * sizeof(double));
	
	circleVerticesX[0] = x;
	circleVerticesY[0] = y;
	
	for(int i = 1; i < numberOfVertices; i++){
		circleVerticesX[i] = x + (radius * cos(i * twicePi / CIRCLES_SIDES));
		circleVerticesY[i] = y + (radius * sin(i * twicePi / CIRCLES_SIDES));
	}
	
	double *allCircleVertices = (double*) malloc(numberOfVertices * 2 * sizeof(double));
	
	for(int i = 0; i < numberOfVertices; i++){
		allCircleVertices[i * 2] = circleVerticesX[i];
		allCircleVertices[(i * 2) + 1] = circleVerticesY[i];
	}
	
	free(circleVerticesX);
	free(circleVerticesY);

	return allCircleVertices;
}
