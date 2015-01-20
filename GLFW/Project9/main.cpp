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
	
	//create some vertices
	Vertex* stars = new Vertex[360];
	stars[0].fPositions[0] = xPos;
	stars[0].fPositions[1] = myGlobal.screenSize/2;

	srand(time(NULL));	// seed the random number jesus
	for (int i = 0; i < 360; i++)
	{
		stars[i].fPositions[0] = rand() % 1025;
		stars[i].fPositions[1] = rand() % 1025;
		stars[i].fPositions[2] = 0.0f;
		stars[i].fPositions[3] = 1.0f;
		stars[i].fColours[0] = 1.0f;
		stars[i].fColours[1] = 1.0f;
		stars[i].fColours[2] = 1.0f;
		stars[i].fColours[3] = 1.0f;
	}
	Vertex* asteroids0 = new Vertex[60];
	
	for (int i = 0; i < 60; i+=6)
	{
		// 1st vert
		asteroids0[0+i].fPositions[0] = rand() % 900;
		asteroids0[0+i].fPositions[1] = rand() % 900;
		// 2nd vert
		asteroids0[1+i].fPositions[0] = asteroids0[0+i].fPositions[0] + astConstant;
		asteroids0[1+i].fPositions[1] = asteroids0[0+i].fPositions[1] - astConstant;

		asteroids0[2+i].fPositions[0] = asteroids0[0+i].fPositions[0] + 2 * astConstant;
		asteroids0[2+i].fPositions[1] = asteroids0[0+i].fPositions[1] - astConstant;

		asteroids0[3+i].fPositions[0] = asteroids0[0+i].fPositions[0] + 3 * astConstant;
		asteroids0[3+i].fPositions[1] = asteroids0[0+i].fPositions[1];

		asteroids0[4+i].fPositions[0] = asteroids0[0+i].fPositions[0] + 2 * astConstant;
		asteroids0[4+i].fPositions[1] = asteroids0[0+i].fPositions[1] + astConstant;

		asteroids0[5 + i].fPositions[0] = asteroids0[0 + i].fPositions[0] + astConstant;
		asteroids0[5 + i].fPositions[1] = asteroids0[0 + i].fPositions[1] + astConstant;
	}

	for (int j = 0; j < 60; j++)
	{
		asteroids0[j].fPositions[2] = 0.0f;
		asteroids0[j].fPositions[3] = 1.0f;
		asteroids0[j].fColours[0] = 0.0f;
		asteroids0[j].fColours[1] = 1.0f;
		asteroids0[j].fColours[2] = 0.0f;
		asteroids0[j].fColours[3] = 0.0f;
	}

	//create a windowed mode window and it's OpenGL context
	

	//create ID for a vertex buffer object for the star
	GLuint uiVBOstar;
	glGenBuffers(1, &uiVBOstar);
	if (uiVBOstar != 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, uiVBOstar);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* 360, NULL, GL_STATIC_DRAW);
		GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		memcpy(vBuffer, stars, sizeof(Vertex)* 360);
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	//VBO for asteroids
	GLuint uiVBOasteroid;
	glGenBuffers(1, &uiVBOasteroid);
	if (uiVBOasteroid != 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, uiVBOasteroid);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* 60, NULL, GL_STATIC_DRAW);
		GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		memcpy(vBuffer, asteroids0, sizeof(Vertex)* 60);
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//create ID for index buffer object for the player ship
	GLuint uiIBOplayerShip;
	glGenBuffers(1, &uiIBOplayerShip);

	//check it succeeded
	if (uiIBOplayerShip != 0)
	{
		//bind IBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBOplayerShip);
		//allocate space for index info on the graphics card
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(char), NULL, GL_STATIC_DRAW);
		//get pointer to newly allocated space on the graphics card
		GLvoid* iBuffer = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
		//specify the order we'd like to draw our vertices.
		//In this case they are in sequential order
		for (int i = 0; i < 3; i++)
		{
			((char*)iBuffer)[i] = i;
		}
		//unmap and unbind
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	GLuint uiIBOstar;
	glGenBuffers(1, &uiIBOstar);

	if (uiIBOstar != 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBOstar);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 360 * sizeof(char), NULL, GL_STATIC_DRAW);
		GLvoid* iBuffer = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
		for (int i = 0; i < 360; i++)
		{
			((char*)iBuffer)[i] = i;
		}
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	GLuint uiIBOasteroids;
	glGenBuffers(1, &uiIBOasteroids);

	if (uiIBOasteroids != 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBOasteroids);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 60 * sizeof(char), NULL, GL_STATIC_DRAW);
		GLvoid* iBuffer = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
		for (int i = 0; i < 60; i++)
		{
			((char*)iBuffer)[i] = i;
		}
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
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
		//enable shaders

		//starz
		{
			glUseProgram(uiProgramFlat);
			glBindBuffer(GL_ARRAY_BUFFER, uiVBOstar);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBOstar);
			//send our orthographic projection info to the shader
			glUniformMatrix4fv(MatrixIDFlat, 1, GL_FALSE, orthographicProjection);
			//enable the vertex array state, since we're sending in an array of vertices
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			//specify where our vertex array is, how many components each vertex has, 
			//the data type of each component and whether the data is normalised or not
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)* 4));
			//draw to the screen
			glDrawElements(GL_POINTS, 360, GL_UNSIGNED_BYTE, NULL);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			//swap front and back buffers
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		

		

		//meant to be asteroids
		{
			glUseProgram(uiProgramFlat);
			//glBindTexture(GL_TEXTURE_2D, uiTextureId);
			glBindBuffer(GL_ARRAY_BUFFER, uiVBOasteroid);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBOasteroids);
			glUniformMatrix4fv(MatrixIDFlat, 1, GL_FALSE, orthographicProjection);
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)* 4));
			//draw to the screen
			glDrawElements(GL_LINE_LOOP, 60, GL_UNSIGNED_BYTE, NULL);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			//swap front and back buffers
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		

		glfwSwapBuffers(myGlobal.window);



		//poll for and process events
		glfwPollEvents();
	}
	glDeleteBuffers(1, &uiIBOplayerShip);
	glfwTerminate();
	return 0;
}