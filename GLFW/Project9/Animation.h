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
	void Draw(float xpos1, float ypos1, float xpos2, float ypos2, float xpos3, float ypos3, float xpos4, float ypos4);
	Vertex* playerShip;
	GLuint uiVBOplayerShip;
	GLuint uiIBOplayerShip;
	GLuint  uiProgramTextured;
	GLuint  uiTextureId;

	Animation();
	float xpos1;
	~Animation();
};