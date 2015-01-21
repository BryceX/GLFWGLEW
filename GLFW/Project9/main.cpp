#include <GL/glew.h>
#include "wglew.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <fstream>
#include <SOIL.h>
#include <iostream>
#include <time.h>
#include "Globals.h"
#include "Player.h"
int xPos = 450;
int astConstant = 40;

int main()
{
	Globals myGlobal = Globals::instance();

	// init GLFW
	if (!glfwInit())
	{
		return -1;
	}
	myGlobal.window = glfwCreateWindow(1024, 1024, "Hello World", NULL, NULL);
	if (!myGlobal.window)
	{
		glfwTerminate();
		return -1;
	}

	//make the window's context current
	glfwMakeContextCurrent(myGlobal.window);

	//………Insert this into the main.cpp example given in the setting up GLFW section – 
	//once the glfw window has been created and after 'glfwMakeContextCurrent(window);'
	// start GLEW
	if (glewInit() != GLEW_OK)
	{
		// OpenGL didn't start-up! shutdown GLFW and return an error code
		glfwTerminate();
		return -1;
	}
	
	GLuint uiProgramFlat = myGlobal.CreateProgram("VertexShader.glsl", "FlatFragmentShader.glsl");
	GLuint uiProgramTextured = myGlobal.CreateProgram("VertexShader.glsl", "TexturedFragmentShader.glsl");
	int width = 50, height = 50, bpp = 4;
	GLuint uiTextureId = myGlobal.loadTexture("zeroprofile.jpg", width, height, bpp);
	//Initialise GLFW
	
	
	
	//………
	//create shader program
	//find the position of the matrix variable in the shader so we can send info there later
	GLuint MatrixIDFlat = glGetUniformLocation(uiProgramFlat, "MVP");

	//set up the mapping of the screen to pixel co-ordinates. Try changing these values to see what happens.
	float *orthographicProjection = myGlobal.getOrtho(0, 1024, 0, 720, 0, 100);
	//loop until the user closes the window


	while (!glfwWindowShouldClose(myGlobal.window))
	{
		//draw code goes here
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(myGlobal.window);



		//poll for and process events
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}