#include "Animation.h"



Animation::Animation()
{
	Globals& myGlobals = Globals::instance();

	//put vertex info first
	playerShip = new Vertex[4];
	playerShip[0].fPositions[0] = 0;
	//y position of the top corner
	playerShip[0].fPositions[1] = 0;
	//x position of the left corner 
	playerShip[1].fPositions[0] = 0;
	//y position of the left corner
	playerShip[1].fPositions[1] = 0;
	//x position of the right corner
	playerShip[2].fPositions[0] = 0;
	//y pos right corner
	playerShip[2].fPositions[1] = 0;
	playerShip[3].fPositions[0] = 0;
	playerShip[3].fPositions[1] = 0;
	for (int i = 0; i < 4; i++)
	{
		playerShip[i].fPositions[2] = 0.0f;
		playerShip[i].fPositions[3] = 1.0f;
		playerShip[i].fColours[0] = 1.0f;
		playerShip[i].fColours[1] = 1.0f;
		playerShip[i].fColours[2] = 1.0f;
		playerShip[i].fColours[3] = 1.0f;
	}
	//set up the UVs
	playerShip[0].fUVs[0] = 0.0f; //top of the triangle
	playerShip[0].fUVs[1] = 0.0f;

	playerShip[1].fUVs[0] = 1.0f; //bottom left
	playerShip[1].fUVs[1] = 0.0f;

	playerShip[2].fUVs[0] = 0.0f; //bottom right
	playerShip[2].fUVs[1] = 1.0f;

	playerShip[3].fUVs[0] = 1.0f;
	playerShip[3].fUVs[1] = 1.0f;

	//making buffers
	glGenBuffers(1, &uiVBOplayerShip);	// VBO
	glGenBuffers(1, &uiIBOplayerShip);	// IBO

	// make the shaders
	uiProgramTextured = myGlobals.CreateProgram("VertexShader.glsl", "TexturedFragmentShader.glsl");

	// load the texture
	int width = 50, height = 50, bpp = 4;
	uiTextureId = myGlobals.loadTexture("mario.jpg", width, height, bpp);
}
void Animation::Draw(float xpos1, float ypos1, float xpos2, float ypos2, float xpos3, float ypos3, float xpos4, float ypos4)
{
	Globals& myGlobals = Globals::instance();

	// specify the shader program to be used for rendering
	glUseProgram(uiProgramTextured);

	// get the location of the MVP variable
	GLuint MatrixIDFlat = glGetUniformLocation(uiProgramTextured, "MVP");

	glBindTexture(GL_TEXTURE_2D, uiTextureId);				// sets up the texture that we gon use
	glBindBuffer(GL_ARRAY_BUFFER, uiVBOplayerShip);			// sets up the VBO we gon use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBOplayerShip);	// sets up the IBO we gon use

	// send the ortho to the shader
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

	

	//glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, NULL);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, NULL);
	//glDrawElements(GL_POINTS, 3, GL_UNSIGNED_BYTE, NULL);
	//glDrawArrays(GL_POINTS, 3, GL_UNSIGNED_BYTE);
	glBindBuffer(GL_ARRAY_BUFFER, 0);				// clear the currently bound buffer for VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);		// clear the currently bound buffer for IBO
	//swap front and back buffers
	glBindTexture(GL_TEXTURE_2D, 0);				// clear the currently bound buffer for texture
	//x position of the top corner



	// send data to the IBO
	if (uiIBOplayerShip != 0)
	{
		//bind IBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBOplayerShip);
		//allocate space for index info on the graphics card
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(char), NULL, GL_STATIC_DRAW);
		//get pointer to newly allocated space on the graphics card
		GLvoid* iBuffer = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
		//specify the order we'd like to draw our vertices.
		//In this case they are in sequential order
		for (int i = 0; i < 4; i++)
		{
			((char*)iBuffer)[i] = i;
		}
		//unmap and unbind
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// send data to the VBO
	if (uiVBOplayerShip != 0)
	{
		//bind VBO
		glBindBuffer(GL_ARRAY_BUFFER, uiVBOplayerShip);
		//allocate space for vertices on the graphics card
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* 4, NULL, GL_STATIC_DRAW);
		//get pointer to allocated space on the graphics card
		GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		//copy data to graphics card
		memcpy(vBuffer, playerShip, sizeof(Vertex)* 4);
		//unmap and unbind buffer
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

/*void Animation::Move()

{
	Globals& myGlobals = Globals::instance();
	if (glfwGetKey(myGlobals.window, GLFW_KEY_W) == GLFW_PRESS)
	{
		//move forward
		for (int i = 0; i < 4; i++)
		{
			playerShip[i].fPositions[1] += myGlobals.screenSize*.0001f;
		}
		glBindBuffer(GL_ARRAY_BUFFER, uiVBOplayerShip);
		GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		//copy data to graphics card
		memcpy(vBuffer, playerShip, sizeof(Vertex)* 4);
		//unmap and unbind buffer
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	if (glfwGetKey(myGlobals.window, GLFW_KEY_S) == GLFW_PRESS)
	{
		//move forward
		for (int i = 0; i < 4; i++)
		{
			playerShip[i].fPositions[1] -= myGlobals.screenSize*.0001f;
		}
		glBindBuffer(GL_ARRAY_BUFFER, uiVBOplayerShip);
		GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		//copy data to graphics card
		memcpy(vBuffer, playerShip, sizeof(Vertex)* 4);
		//unmap and unbind buffer
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	if (glfwGetKey(myGlobals.window, GLFW_KEY_D) == GLFW_PRESS)
	{
		//move forward
		for (int i = 0; i < 4; i++)
		{
			playerShip[i].fPositions[0] += myGlobals.screenSize*.0001f;
		}
		glBindBuffer(GL_ARRAY_BUFFER, uiVBOplayerShip);
		GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		//copy data to graphics card
		memcpy(vBuffer, playerShip, sizeof(Vertex)* 4);
		//unmap and unbind buffer
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	if (glfwGetKey(myGlobals.window, GLFW_KEY_A) == GLFW_PRESS)
	{
		//move forward
		for (int i = 0; i < 4; i++)
		{
			playerShip[i].fPositions[0] -= myGlobals.screenSize*.0001f;
		}
		glBindBuffer(GL_ARRAY_BUFFER, uiVBOplayerShip);
		GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		//copy data to graphics card
		memcpy(vBuffer, playerShip, sizeof(Vertex)* 4);
		//unmap and unbind buffer
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}*/


Animation::~Animation()
{
	delete playerShip;
}