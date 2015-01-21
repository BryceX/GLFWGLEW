#include "Player.h"



Player::Player()
{
	Globals& myGlobals = Globals::instance();
	glGenBuffers(1, &uiVBOplayerShip);
	glGenBuffers(1, &uiIBOplayerShip);
	playerShip = new Vertex[3];
	uiProgramTextured = myGlobals.CreateProgram("VertexShader.glsl", "TexturedFragmentShader.glsl");
	int width = 50, height = 50, bpp = 4; 
	uiTextureId = myGlobals.loadTexture("zeroprofile.jpg", width, height, bpp);

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
void Player::Draw()
//this is the fucking ship

{
	Globals& myGlobals = Globals::instance();
	glUseProgram(uiProgramTextured);
	GLuint MatrixIDFlat = glGetUniformLocation(uiProgramTextured, "MVP");
	
	glBindTexture(GL_TEXTURE_2D, uiTextureId);
	glBindBuffer(GL_ARRAY_BUFFER, uiVBOplayerShip);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBOplayerShip);
	//send our orthographic projection info to the shader
	float * ortho = myGlobals.getOrtho(0, 1024, 0, 720, 0, 100);

	glUniformMatrix4fv(MatrixIDFlat, 1, GL_FALSE, myGlobals.orthographicProjection);
	//enable the vertex array state, since we're sending in an array of vertices
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	//specify where our vertex array is, how many components each vertex has, 
	//the data type of each component and whether the data is normalised or not
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)* 4));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)* 8));
	//draw to the screen

	//glPointSize(100.f);

	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, NULL);
	//glDrawElements(GL_POINTS, 3, GL_UNSIGNED_BYTE, NULL);
	//glDrawArrays(GL_POINTS, 3, GL_UNSIGNED_BYTE);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//swap front and back buffers
	glBindTexture(GL_TEXTURE_2D, 0);
	//x position of the top corner
	


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
	//check it succeeded
	if (uiVBOplayerShip != 0)
	{
		//bind VBO
		glBindBuffer(GL_ARRAY_BUFFER, uiVBOplayerShip);
		//allocate space for vertices on the graphics card
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* 3, NULL, GL_STATIC_DRAW);
		//get pointer to allocated space on the graphics card
		GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		//copy data to graphics card
		memcpy(vBuffer, playerShip, sizeof(Vertex)* 3);
		//unmap and unbind buffer
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void Player::Move()
{
	Globals& myGlobals = Globals::instance();
	if (glfwGetKey(myGlobals.window, GLFW_KEY_W) == GLFW_PRESS)
	{
		//move forward
		for (int i = 0; i < 3; i++)
		{
			playerShip[i].fPositions[1] += myGlobals.screenSize*.0001f;
		}
		glBindBuffer(GL_ARRAY_BUFFER, uiVBOplayerShip);
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
		glBindBuffer(GL_ARRAY_BUFFER, uiVBOplayerShip);
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
			playerShip[i].fPositions[0] += myGlobals.screenSize*.0001f;
		}
		glBindBuffer(GL_ARRAY_BUFFER, uiVBOplayerShip);
		GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		//copy data to graphics card
		memcpy(vBuffer, playerShip, sizeof(Vertex)* 3);
		//unmap and unbind buffer
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	if (glfwGetKey(myGlobals.window, GLFW_KEY_A) == GLFW_PRESS)
	{
		//move forward
		for (int i = 0; i < 3; i++)
		{
			playerShip[i].fPositions[0] -= myGlobals.screenSize*.0001f;
		}
		glBindBuffer(GL_ARRAY_BUFFER, uiVBOplayerShip);
		GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		//copy data to graphics card
		memcpy(vBuffer, playerShip, sizeof(Vertex)* 3);
		//unmap and unbind buffer
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}


Player::~Player()
{
	delete playerShip;
}