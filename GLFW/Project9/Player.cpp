#include "Player.h"
Globals myGlobals = Globals::instance();
Player::Player()
{
	//x position of the top corner
	playerShip[0].fPositions[0] = myGlobals.screenSize / 2;
	//y position of the top corner
	playerShip[0].fPositions[1] = myGlobals.screenSize / 2 + 10.0f;
	//x position of the left corner 
	playerShip[1].fPositions[0] = myGlobals.screenSize / 2 - 10.0f;
	//y position of the left corner
	playerShip[1].fPositions[1] = myGlobals.screenSize / 2 - 10.0f;
	//x position of the right corner
	playerShip[2].fPositions[0] = myGlobals.screenSize / 2 + 10.0f;
	//y pos right corner
	playerShip[2].fPositions[1] = myGlobals.screenSize / 2 - 10.0f;
	for (int i = 0; i < 3; i++)
	{
		playerShip[i].fPositions[2] = 0.0f;
		playerShip[i].fPositions[3] = 1.0f;
		playerShip[i].fColours[0] = 1.0f;
		playerShip[i].fColours[1] = 0.0f;
		playerShip[i].fColours[2] = 1.0f;
		playerShip[i].fColours[3] = 1.0f;
	}
	//set up the UVs
	playerShip[0].fUVs[0] = 0.5f; //top of the triangle
	playerShip[0].fUVs[1] = 1.0f;
	playerShip[1].fUVs[0] = 0.0f; //bottom left
	playerShip[1].fUVs[1] = 0.0f;
	playerShip[2].fUVs[0] = 1.0f; //bottom right
	playerShip[2].fUVs[1] = 0.0f;
}
void Player::Move()
{
	
	if (glfwGetKey(myGlobals.window, GLFW_KEY_W) == GLFW_PRESS)
	{
		//move forward
		for (int i = 0; i < 3; i++)
		{
			playerShip[i].fPositions[1] += myGlobals.screenSize*.0001f;
		}
		glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
		GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		//copy data to graphics card
		memcpy(vBuffer, playerShip, sizeof(Vertex)* 3);
		//unmap and unbind buffer
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	if (glfwGetKey(myGlobals.window, GLFW_KEY_S) == GLFW_PRESS)
	{
		//move forward
		for (int i = 0; i < 3; i++)
		{
			playerShip[i].fPositions[1] -= myGlobals.screenSize*.0001f;
		}
		glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
		GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		//copy data to graphics card
		memcpy(vBuffer, playerShip, sizeof(Vertex)* 3);
		//unmap and unbind buffer
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	if (glfwGetKey(myGlobals.window, GLFW_KEY_D) == GLFW_PRESS)
	{
		//move forward
		for (int i = 0; i < 3; i++)
		{
			myShape[i].fPositions[0] += myGlobals.screenSize*.0001f;
		}
		glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
		GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		//copy data to graphics card
		memcpy(vBuffer, myShape, sizeof(Vertex)* 3);
		//unmap and unbind buffer
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	if (glfwGetKey(myGlobals.window, GLFW_KEY_A) == GLFW_PRESS)
	{
		//move forward
		for (int i = 0; i < 3; i++)
		{
			myShape[i].fPositions[0] -= myGlobals.screenSize*.0001f;
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
	delete playerShip;
}