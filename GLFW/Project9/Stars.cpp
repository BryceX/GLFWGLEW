#include "Stars.h"

Stars::Stars()
{

	Globals& myGlobals = Globals::instance();
	glGenBuffers(1, &uiIBOstars);
	glGenBuffers(1, &uiVBOstars);

	//put vertex info first
	stars = new Vertex[1];
	
	//x position of the left corner 

		stars[0].fPositions[0] = rand() % 1025;
		stars[0].fPositions[1] = rand() % 1025;
		stars[0].fPositions[2] = 0.0f;
		stars[0].fPositions[3] = 1.0f;
		stars[0].fColours[0] = 1.0f;
		stars[0].fColours[1] = 1.0f;
		stars[0].fColours[2] = 1.0f;
		stars[0].fColours[3] = 1.0f;
	


	// make the shaders
	uiProgramFlat = myGlobals.CreateProgram("VertexShader.glsl", "FlatFragmentShader.glsl");

	// load the texture



	if (uiVBOstars != 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, uiVBOstars);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex), NULL, GL_STATIC_DRAW);
		GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		memcpy(vBuffer, stars, sizeof(Vertex));
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}



	if (uiIBOstars != 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBOstars);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(char), NULL, GL_STATIC_DRAW);
		GLvoid* iBuffer = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
			((char*)iBuffer)[0] = 0;
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}
void Stars::Draw()
{
	Globals& myGlobals = Globals::instance();
	glUseProgram(uiProgramFlat);
	GLuint MatrixIDFlat = glGetUniformLocation(uiProgramFlat, "MVP");
	//create some vertices

	// seed the random number jesus
	//starz
	glUseProgram(uiProgramFlat);
	glBindBuffer(GL_ARRAY_BUFFER, uiVBOstars);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBOstars);
	//send our orthographic projection info to the shader

	glUniformMatrix4fv(MatrixIDFlat, 1, GL_FALSE, myGlobals.orthographicProjection);
	//enable the vertex array state, since we're sending in an array of vertices
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//specify where our vertex array is, how many components each vertex has, 
	//the data type of each component and whether the data is normalised or not
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)* 4));
	//draw to the screen
	glDrawElements(GL_POINTS, 1, GL_UNSIGNED_BYTE, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//swap front and back buffers
	glBindTexture(GL_TEXTURE_2D, 0);
}

Stars::~Stars()
{

}