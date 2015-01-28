#pragma once
#include <GL/glew.h>
#include "wglew.h"
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include "Globals.h"
class Animation
{
public:
	void Move();
	void Draw();
	Vertex* playerShip;
	GLuint uiVBOplayerShip;
	GLuint uiIBOplayerShip;
	GLuint  uiProgramTextured;
	GLuint  uiTextureId;

	Animation();
	~Animation();
};