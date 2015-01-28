#pragma once
#include <GL/glew.h>
#include "wglew.h"
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include "Globals.h"
class PlayerShip
{
public:
	Vertex vertices[3];
	GLuint VBO;
	GLuint IBO;
	GLuint shader;





	PlayerShip();
	~PlayerShip();
};
