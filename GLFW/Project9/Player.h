#pragma once
#include <GL/glew.h>
#include "wglew.h"
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include "Globals.h"
class Player
{
public:
	unsigned int spriteID;
	void Move();
	Vertex* playerShip;












	Player();
	~Player();
};