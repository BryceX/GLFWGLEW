#pragma once
#include <GL/glew.h>
#include "wglew.h"
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include "Globals.h"

class Asteroids
{
public:

	void DrawStars();
	void DrawAsteroids();
	Vertex* stars;
	GLuint uiVBOstars;
	GLuint uiIBOstars;
	Vertex* asteroids0;
	GLuint uiVBOasteroids;
	GLuint uiIBOasteroids;
	GLuint uiProgramFlat;
	int astConstant = 40;



	Asteroids();
	~Asteroids();
	
};