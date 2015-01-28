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
#include "Asteroids.h"
#include "Stars.h"
#include "Animation.h"


int main()
{
	Globals& myGlobals = Globals::instance();
	srand(time(NULL));
	// init GLFW
	if (!glfwInit())
	{
		return -1;
	}
	myGlobals.window = glfwCreateWindow(1024, 1024, "Hello World", NULL, NULL);
	myGlobals.orthographicProjection = myGlobals.getOrtho(0, 1024, 0, 720, 0, 100);
	if (!myGlobals.window)
	{
		glfwTerminate();
		return -1;
	}

	//make the window's context current
	glfwMakeContextCurrent(myGlobals.window);

	//���Insert this into the main.cpp example given in the setting up GLFW section � 
	//once the glfw window has been created and after 'glfwMakeContextCurrent(window);'
	// start GLEW
	if (glewInit() != GLEW_OK)
	{
		// OpenGL didn't start-up! shutdown GLFW and return an error code
		glfwTerminate();
		return -1;
	}
	
	//GLuint uiProgramFlat = myGlobals.CreateProgram("VertexShader.glsl", "FlatFragmentShader.glsl");
	//GLuint uiProgramTextured = myGlobals.CreateProgram("VertexShader.glsl", "TexturedFragmentShader.glsl");	
	//Initialise GLFW
	Player myShip;
	Animation myOtherShip;
	//Environment backgroundObjects;
	Stars myStars[100];
	Asteroids myAsteroids[10];


	
	//set up the mapping of the screen to pixel co-ordinates. Try changing these values to see what happens.
	myGlobals.orthographicProjection = myGlobals.getOrtho(0, 1024, 0, 720, 0, 100);
	//loop until the user closes the window

	
	while (!glfwWindowShouldClose(myGlobals.window))
	{
		//draw code goes here
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		myShip.Move();
		myShip.Draw();
		myOtherShip.Draw(0,0,0,0,0,0,0,0);
		for (int i = 0; i < 100; i++)
		{
		myStars[i].Draw();
		}
		for (int i = 0; i < 10; i++)
		{
			myAsteroids[i].DrawAsteroids();
		}
		//backgroundObjects.DrawStars();
		//multiples of 6 make more ateroids
		//backgroundObjects.DrawAsteroids();

		
		glfwSwapBuffers(myGlobals.window);

		if (glfwGetKey(myGlobals.window, GLFW_KEY_ESCAPE))
		{
			break;
		}

		//poll for and process events
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}