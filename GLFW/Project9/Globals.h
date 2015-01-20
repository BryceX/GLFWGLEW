#pragma once
#include <GL/glew.h>
#include "wglew.h"
#include <GLFW/glfw3.h>
#include <SOIL.h>
class Globals
{
public:
	int screenSize = 1024;
	GLFWwindow * window;



	static Globals& instance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new Globals();
		}

		return *m_instance;
	}




	Globals();
	~Globals();
private:
	static Globals * m_instance;
};
struct Vertex
{
	float fPositions[4];
	float fColours[4];
	float fUVs[2];
};
