#include "Asteroids.h"

Asteroids::Asteroids()
{
	Globals& myGlobals = Globals::instance();
	glGenBuffers(1, &uiVBOasteroids);
	glGenBuffers(1, &uiIBOasteroids);
	uiProgramFlat = myGlobals.CreateProgram("VertexShader.glsl", "FlatFragmentShader.glsl");
	

	asteroids0 = new Vertex[6];
	asteroids0[0].fPositions[0] = rand() % 900;
	asteroids0[0].fPositions[1] = rand() % 900;

	asteroids0[1].fPositions[0] = asteroids0[0].fPositions[0] + astConstant;
	asteroids0[1].fPositions[1] = asteroids0[0].fPositions[1] - astConstant;

	asteroids0[2].fPositions[0] = asteroids0[0].fPositions[0] + 2 * astConstant;
	asteroids0[2].fPositions[1] = asteroids0[0].fPositions[1] - astConstant;

	asteroids0[3].fPositions[0] = asteroids0[0].fPositions[0] + 3 * astConstant;
	asteroids0[3].fPositions[1] = asteroids0[0].fPositions[1];

	asteroids0[4].fPositions[0] = asteroids0[0].fPositions[0] + 2 * astConstant;
	asteroids0[4].fPositions[1] = asteroids0[0].fPositions[1] + astConstant;

	asteroids0[5].fPositions[0] = asteroids0[0].fPositions[0] + astConstant;
	asteroids0[5].fPositions[1] = asteroids0[0].fPositions[1] + astConstant;



	for (int j = 0; j < 6; j++)
	{
		asteroids0[j].fPositions[2] = 0.0f;
		asteroids0[j].fPositions[3] = 1.0f;
		asteroids0[j].fColours[0] = 0.0f;
		asteroids0[j].fColours[1] = 1.0f;
		asteroids0[j].fColours[2] = 0.0f;
		asteroids0[j].fColours[3] = 0.0f;
	}
	if (uiVBOasteroids != 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, uiVBOasteroids);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* 6, NULL, GL_STATIC_DRAW);
		GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		memcpy(vBuffer, asteroids0, sizeof(Vertex)* 6);
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	if (uiIBOasteroids != 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBOasteroids);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6* sizeof(char), NULL, GL_STATIC_DRAW);
		GLvoid* iBuffer = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
		for (int i = 0; i < 6; i++)
		{
			((char*)iBuffer)[i] = i;
		}
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}


void Asteroids::DrawAsteroids()
{
	Globals& myGlobals = Globals::instance();
	glUseProgram(uiProgramFlat);
	GLuint MatrixIDFlat = glGetUniformLocation(uiProgramFlat, "MVP");
	//srand(time(NULL));
	//meant to be asteroids
	
		glUseProgram(uiProgramFlat);
		//glBindTexture(GL_TEXTURE_2D, uiTextureId);
		glBindBuffer(GL_ARRAY_BUFFER, uiVBOasteroids);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBOasteroids);
		glUniformMatrix4fv(MatrixIDFlat, 1, GL_FALSE, myGlobals.orthographicProjection);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)* 4));
		//draw to the screen
		glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_BYTE, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		//swap front and back buffers
		glBindTexture(GL_TEXTURE_2D, 0);
}


Asteroids::~Asteroids()
{
}