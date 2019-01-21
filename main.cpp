// Tags to compile -lglfw -lrt -lm -ldl -lGLEW -lGL

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "graphics.hpp"

#define GLEW_STATIC
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define CIRCLES_SIDES 300

/* Read a shader source from a file and compile the shader.
 * The second parameter must be "vertex" or "fragment". 
 * It returns the shader id. */
GLuint compileShader(string file, string type);

float* createCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius);

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
	 * supports the new core functionality.*/
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

	glViewport(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
    glMatrixMode(GL_PROJECTION); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
    glLoadIdentity(); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1); // essentially set coordinate system
    glMatrixMode(GL_MODELVIEW); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
    glLoadIdentity(); // same as above comment

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
	glBindFragDataLocation(shaderProgram, 0, "outColor1");

	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram); // Only one program can be active at a time.

	float main_triangle[] = {
		SCREEN_WIDTH/4.0f, SCREEN_HEIGHT/4.0f, // Vertex 1 (X, Y)
		SCREEN_WIDTH/2.0f, SCREEN_HEIGHT*3/4.0f // Vertex 2 (X, Y)
		// SCREEN_WIDTH/2.0f, SCREEN_HEIGHT*3/4.0f,
		// SCREEN_WIDTH*3/4.0f, SCREEN_HEIGHT/4.0f, // Vertex 3 (X, Y)
		// SCREEN_WIDTH*3/4.0f, SCREEN_HEIGHT/4.0f,
		// SCREEN_WIDTH/4.0f, SCREEN_HEIGHT/4.0f
	};


	GLuint vboTriangle;
	glGenBuffers(1, &vboTriangle); // Generate 1 buffer
	glBindBuffer(GL_ARRAY_BUFFER, vboTriangle); // Select a buffer to put some data in it
	glBufferData(GL_ARRAY_BUFFER, sizeof(main_triangle), main_triangle, GL_STATIC_DRAW);
	posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);

	GLuint vaoCircles;
	glGenVertexArrays(1, &vaoCircles);
	glBindVertexArray(vaoCircles);
	
	float *circleVertices = createCircle(0, 0, 0, 0.10f * SCREEN_WIDTH);
	
	GLuint vboCircles;
	glGenBuffers(1, &vboCircles); // Generate 1 buffer
	glBindBuffer(GL_ARRAY_BUFFER, vboCircles); // Select a buffer to put some data in it
	glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices), circleVertices, GL_STATIC_DRAW);
	posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);

	/* Use glDrawBuffers when rendering to multiple buffers, because only
	 * the first output will be enabled by default. */
	// Loop until the user closese the window
	while(!glfwWindowShouldClose(window)){
		// Render here
		glClear(GL_COLOR_BUFFER_BIT);

		// glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(vaoTriangle);
		glDrawArrays(GL_LINES, 0, 2);

		// glBindVertexArray(vaoCircles);
		// glDrawArrays(GL_TRIANGLES, 0, 6);

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

float* createCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius){
	int numberOfVertices = CIRCLES_SIDES + 2;
    
	GLfloat twicePi = 2.0f * M_PI;
    
	GLfloat *circleVerticesX = (GLfloat*) malloc(numberOfVertices * sizeof(float));
	GLfloat *circleVerticesY = (GLfloat*) malloc(numberOfVertices * sizeof(float));
	GLfloat *circleVerticesZ = (GLfloat*) malloc(numberOfVertices * sizeof(float));
	
	circleVerticesX[0] = x;
	circleVerticesY[0] = y;
	circleVerticesZ[0] = z;
	
	for(int i = 1; i < numberOfVertices; i++){
		circleVerticesX[i] = x + (radius * cos(i *  twicePi / CIRCLES_SIDES));
		circleVerticesY[i] = y + (radius * sin(i * twicePi / CIRCLES_SIDES));
		circleVerticesZ[i] = z;
	}
	
	GLfloat *allCircleVertices = (GLfloat*) malloc(numberOfVertices * 3 * sizeof(float));
	
	for(int i = 0; i < numberOfVertices; i++){
		allCircleVertices[i * 3] = circleVerticesX[i];
		allCircleVertices[(i * 3) + 1] = circleVerticesY[i];
		allCircleVertices[(i * 3) + 2] = circleVerticesZ[i];
	}
	
	free(circleVerticesX);
	free(circleVerticesY);
	free(circleVerticesZ);

	return allCircleVertices;
}
