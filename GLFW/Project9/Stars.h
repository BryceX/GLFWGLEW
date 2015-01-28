#pragma once
#include <GL/glew.h>
#include "wglew.h"
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include "Globals.h"

class Stars
{
public:
	Vertex* stars;
	GLuint uiVBOstars;
	GLuint uiIBOstars;
	GLuint uiProgramFlat;
	void Draw();

	Stars();
	~Stars();
};