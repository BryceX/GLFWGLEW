#include "Player.h"
Player::Player()
{
}
/*void Player::Move()
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		//move forward
		for (int i = 0; i < 3; i++)
		{
			myShape[i].fPositions[1] += screenSize*.0001f;
		}
		glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
		GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		//copy data to graphics card
		memcpy(vBuffer, myShape, sizeof(Vertex)* 3);
		//unmap and unbind buffer
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		//move forward
		for (int i = 0; i < 3; i++)
		{
			myShape[i].fPositions[1] -= screenSize*.0001f;
		}
		glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
		GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		//copy data to graphics card
		memcpy(vBuffer, myShape, sizeof(Vertex)* 3);
		//unmap and unbind buffer
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		//move forward
		for (int i = 0; i < 3; i++)
		{
			myShape[i].fPositions[0] += screenSize*.0001f;
		}
		glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
		GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		//copy data to graphics card
		memcpy(vBuffer, myShape, sizeof(Vertex)* 3);
		//unmap and unbind buffer
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		//move forward
		for (int i = 0; i < 3; i++)
		{
			myShape[i].fPositions[0] -= screenSize*.0001f;
		}
		glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
		GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		//copy data to graphics card
		memcpy(vBuffer, myShape, sizeof(Vertex)* 3);
		//unmap and unbind buffer
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

Player::~Player()
{
}*/