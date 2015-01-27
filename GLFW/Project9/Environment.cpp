#include "Environment.h"

Environment::Environment()
{
	Globals& myGlobals = Globals::instance();
	glGenBuffers(1, &uiIBOstars);
	glGenBuffers(1, &uiVBOstars);
	glGenBuffers(1, &uiVBOasteroids);
	glGenBuffers(1, &uiIBOasteroids);
	uiProgramFlat = myGlobals.CreateProgram("VertexShader.glsl", "FlatFragmentShader.glsl");
	srand(time(NULL));
	Vertex* stars = new Vertex[360];
	for (int i = 0; i < 360; i++)
	{
		stars[i].fPositions[0] = rand() % 1025;
		stars[i].fPositions[1] = rand() % 1025;
		stars[i].fPositions[2] = 0.0f;
		stars[i].fPositions[3] = 1.0f;
		stars[i].fColours[0] = 1.0f;
		stars[i].fColours[1] = 1.0f;
		stars[i].fColours[2] = 1.0f;
		stars[i].fColours[3] = 1.0f;

		if (uiVBOstars != 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, uiVBOstars);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* 360, NULL, GL_STATIC_DRAW);
			GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
			memcpy(vBuffer, stars, sizeof(Vertex)* 360);
			glUnmapBuffer(GL_ARRAY_BUFFER);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}



		if (uiIBOstars != 0)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBOstars);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 360 * sizeof(char), NULL, GL_STATIC_DRAW);
			GLvoid* iBuffer = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
			for (int i = 0; i < 360; i++)
			{
				((char*)iBuffer)[i] = i;
			}
			glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}


	}
	Vertex* asteroids0 = new Vertex[6];
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
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(char), NULL, GL_STATIC_DRAW);
		GLvoid* iBuffer = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
		for (int i = 0; i < 6; i++)
		{
			((char*)iBuffer)[i] = i;
		}
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
void Environment::DrawStars()
{
	Globals& myGlobals = Globals::instance();
	glUseProgram(uiProgramFlat);
	GLuint MatrixIDFlat = glGetUniformLocation(uiProgramFlat, "MVP");
	//create some vertices
	srand(time(NULL));
	// seed the random number jesus
	//starz
	glUseProgram(uiProgramFlat);
	glBindBuffer(GL_ARRAY_BUFFER, uiVBOstars);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBOstars);
	//send our orthographic projection info to the shader
	float * ortho = myGlobals.getOrtho(0, 1024, 0, 720, 0, 100);
	glUniformMatrix4fv(MatrixIDFlat, 1, GL_FALSE, myGlobals.orthographicProjection);
	//enable the vertex array state, since we're sending in an array of vertices
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//specify where our vertex array is, how many components each vertex has, 
	//the data type of each component and whether the data is normalised or not
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)* 4));
	//draw to the screen
	glDrawElements(GL_POINTS, 360, GL_UNSIGNED_BYTE, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//swap front and back buffers
	glBindTexture(GL_TEXTURE_2D, 0);		
}

void Environment::DrawAsteroids()
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
		float * ortho = myGlobals.getOrtho(0, 1024, 0, 720, 0, 100);
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


Environment::~Environment()
{

}