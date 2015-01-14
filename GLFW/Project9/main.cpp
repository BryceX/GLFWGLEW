#include <GL/glew.h>
#include "wglew.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <fstream>
#include <SOIL.h>
#include <iostream>
#include <time.h>
int screenSize = 1024;
int xPos = 450;
int astConstant = 40;
float* getOrtho(float left, float right, float bottom, float top, float a_fNear, float a_fFar)
{
	//to correspond with mat4 in the shader
	//ideally this function would be part of your matrix class
	//however I wasn't willing to write your matrix class for you just to show you this
	//so here we are in array format!
	//add this to your matrix class as a challenge if you like!
	float* toReturn = new float[12];
	toReturn[0] = 2.0 / (right - left);;
	toReturn[1] = toReturn[2] = toReturn[3] = toReturn[4] = 0;
	toReturn[5] = 2.0 / (top - bottom);
	toReturn[6] = toReturn[7] = toReturn[8] = toReturn[9] = 0;
	toReturn[10] = 2.0 / (a_fFar - a_fNear);
	toReturn[11] = 0;
	toReturn[12] = -1 * ((right + left) / (right - left));
	toReturn[13] = -1 * ((top + bottom) / (top - bottom));
	toReturn[14] = -1 * ((a_fFar + a_fNear) / (a_fFar - a_fNear));
	toReturn[15] = 1;
	return toReturn;
}
unsigned int loadTexture(const char* a_pFilename, int & a_iWidth, int & a_iHeight, int & a_iBPP)
{
	unsigned int uiTextureID = 0;
	//check file exists
	if (a_pFilename != nullptr)
	{
		//read in image data from file
		unsigned char* pImageData = SOIL_load_image(a_pFilename, &a_iWidth, &a_iHeight, &a_iBPP, SOIL_LOAD_AUTO);

		//check for successful read
		if (pImageData)
		{
			//create opengl texture handle
			uiTextureID = SOIL_create_OGL_texture(pImageData, a_iWidth, a_iHeight, a_iBPP,
				SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
			//clear what was read in from file now that it is stored in the handle
			SOIL_free_image_data(pImageData);
		}

		//check for errors
		if (uiTextureID == 0)
		{
			std::cout << "SOIL loading error: " << SOIL_last_result() << std::endl;
		}
		return uiTextureID;
	}
}
//declare our vertex structure
struct Vertex
{
	float fPositions[4];
	float fColours[4];
	float fUVs[2];
};

GLuint CreateShader(GLenum a_eShaderType, const char *a_strShaderFile)
{
	std::string strShaderCode;
	//open shader file
	std::ifstream shaderStream(a_strShaderFile);
	//if that worked ok, load file line by line
	if (shaderStream.is_open())
	{
		std::string Line = "";
		while (std::getline(shaderStream, Line))
		{
			strShaderCode += "\n" + Line;
		}
		shaderStream.close();
	}

	//convert to cstring
	char const *szShaderSourcePointer = strShaderCode.c_str();

	//create shader ID
	GLuint uiShader = glCreateShader(a_eShaderType);
	//load source code
	glShaderSource(uiShader, 1, &szShaderSourcePointer, NULL);

	//compile shader
	glCompileShader(uiShader);

	//check for compilation errors and output them
	GLint iStatus;
	glGetShaderiv(uiShader, GL_COMPILE_STATUS, &iStatus);
	if (iStatus == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(uiShader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(uiShader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch (a_eShaderType)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
	}

	return uiShader;
}

GLuint CreateProgram(const char *a_vertex, const char *a_frag)
{
	std::vector<GLuint> shaderList;

	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, a_vertex));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, a_frag));

	//create shader program ID
	GLuint uiProgram = glCreateProgram();

	//attach shaders
	for (auto shader = shaderList.begin(); shader != shaderList.end(); shader++)
		glAttachShader(uiProgram, *shader);

	//link program
	glLinkProgram(uiProgram);

	//check for link errors and output them
	GLint status;
	glGetProgramiv(uiProgram, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(uiProgram, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(uiProgram, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	for (auto shader = shaderList.begin(); shader != shaderList.end(); shader++)
	{
		glDetachShader(uiProgram, *shader);
		glDeleteShader(*shader);
	}

	return uiProgram;
}


int main()
{
	//Initialise GLFW
	if (!glfwInit())
	{
		return -1;
	}

	//create some vertices
	Vertex* playerShip = new Vertex[3];
	//x position of the top corner
	playerShip[0].fPositions[0] = screenSize / 2;
	//y position of the top corner
	playerShip[0].fPositions[1] = screenSize / 2 + 10.0f;
	//x position of the left corner 
	playerShip[1].fPositions[0] = screenSize / 2 - 10.0f;
	//y position of the left corner
	playerShip[1].fPositions[1] = screenSize / 2 - 10.0f;
	//x position of the right corner
	playerShip[2].fPositions[0] = screenSize / 2 + 10.0f;
	//y pos right corner
	playerShip[2].fPositions[1] = screenSize / 2 - 10.0f;
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

	Vertex* stars = new Vertex[360];
	stars[0].fPositions[0] = xPos;
	stars[0].fPositions[1] = screenSize/2;

	srand(time(NULL));	// seed the random number jesus
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
	}
	Vertex* asteroids0 = new Vertex[60];
	asteroids0[0].fPositions[0] = screenSize * .2f;
	asteroids0[0].fPositions[1] = screenSize * .2f;

	asteroids0[1].fPositions[0] = (screenSize * .2f) + astConstant;
	asteroids0[1].fPositions[1] = (screenSize * .2f) - astConstant;

	asteroids0[2].fPositions[0] = (screenSize * .2f) + 2*astConstant;
	asteroids0[2].fPositions[1] = (screenSize * .2f) - astConstant;

	asteroids0[3].fPositions[0] = (screenSize * .2f) + 3*astConstant;
	asteroids0[3].fPositions[1] = (screenSize * .2f);

	asteroids0[4].fPositions[0] = (screenSize * .2f) + 2*astConstant;
	asteroids0[4].fPositions[1] = (screenSize * .2f) + astConstant;

	asteroids0[5].fPositions[0] = (screenSize * .2f) + astConstant;
	asteroids0[5].fPositions[1] = (screenSize*.2f) + astConstant;
	for (int i = 0; i < 6; i++)
	{
		asteroids0[i].fPositions[2] = 0.0f;
		asteroids0[i].fPositions[3] = 1.0f;
		asteroids0[i].fColours[0] = 0.0f;
		asteroids0[i].fColours[1] = 1.0f;
		asteroids0[i].fColours[0] = 0.0f;
		asteroids0[i].fColours[1] = 1.0f;
	}

	//create a windowed mode window and it's OpenGL context
	GLFWwindow* window;
	window = glfwCreateWindow(1024, 1024, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	//make the window's context current
	glfwMakeContextCurrent(window);

	//………Insert this into the main.cpp example given in the setting up GLFW section – 
	//once the glfw window has been created and after 'glfwMakeContextCurrent(window);'
	// start GLEW
	if (glewInit() != GLEW_OK)
	{
		// OpenGL didn't start-up! shutdown GLFW and return an error code
		glfwTerminate();
		return -1;
	}

	//create ID for a vertex buffer object for the player
	GLuint uiVBOplayerShip;
	glGenBuffers(1, &uiVBOplayerShip);

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

	//create ID for a vertex buffer object for the star
	GLuint uiVBOstar;
	glGenBuffers(1, &uiVBOstar);
	if (uiVBOstar != 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, uiVBOstar);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* 360, NULL, GL_STATIC_DRAW);
		GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		memcpy(vBuffer, stars, sizeof(Vertex)* 360);
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	//VBO for asteroids
	GLuint uiVBOasteroid;
	glGenBuffers(1, &uiVBOasteroid);
	if (uiVBOasteroid != 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, uiVBOasteroid);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* 6, NULL, GL_STATIC_DRAW);
		GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		memcpy(vBuffer, asteroids0, sizeof(Vertex)* 6);
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//create ID for index buffer object for the player ship
	GLuint uiIBOplayerShip;
	glGenBuffers(1, &uiIBOplayerShip);

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
	GLuint uiIBOstar;
	glGenBuffers(1, &uiIBOstar);

	if (uiIBOstar != 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBOstar);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 360 * sizeof(char), NULL, GL_STATIC_DRAW);
		GLvoid* iBuffer = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
		for (int i = 0; i < 360; i++)
		{
			((char*)iBuffer)[i] = i;
		}
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	GLuint uiIBOasteroids;
	glGenBuffers(1, &uiIBOasteroids);

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
	//………
	//create shader program
	GLuint uiProgramFlat = CreateProgram("VertexShader.glsl", "FlatFragmentShader.glsl");
	GLuint uiProgramTextured = CreateProgram("VertexShader.glsl", "TexturedFragmentShader.glsl");
	//find the position of the matrix variable in the shader so we can send info there later
	GLuint MatrixIDFlat = glGetUniformLocation(uiProgramFlat, "MVP");

	//set up the mapping of the screen to pixel co-ordinates. Try changing these values to see what happens.
	float *orthographicProjection = getOrtho(0, 1024, 0, 720, 0, 100);
	//loop until the user closes the window
	int width = 50, height = 50, bpp = 4;
	GLuint uiTextureId = loadTexture("zeroprofile.jpg", width, height, bpp);



	while (!glfwWindowShouldClose(window))
	{
		//draw code goes here
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//enable shaders

		//starz
		{
			glUseProgram(uiProgramFlat);
			glBindBuffer(GL_ARRAY_BUFFER, uiVBOstar);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBOstar);
			//send our orthographic projection info to the shader
			glUniformMatrix4fv(MatrixIDFlat, 1, GL_FALSE, orthographicProjection);
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



		//this is the fucking ship
		{
		glUseProgram(uiProgramTextured);
		glBindTexture(GL_TEXTURE_2D, uiTextureId);
		glBindBuffer(GL_ARRAY_BUFFER, uiVBOplayerShip);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBOplayerShip);
		//send our orthographic projection info to the shader
		glUniformMatrix4fv(MatrixIDFlat, 1, GL_FALSE, orthographicProjection);
		//enable the vertex array state, since we're sending in an array of vertices
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		//specify where our vertex array is, how many components each vertex has, 
		//the data type of each component and whether the data is normalised or not
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)* 4));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)* 8));
		//draw to the screen
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		//swap front and back buffers
		glBindTexture(GL_TEXTURE_2D, 0);
		}

		//meant to be asteroids
		{
			glUseProgram(uiProgramFlat);
			//glBindTexture(GL_TEXTURE_2D, uiTextureId);
			glBindBuffer(GL_ARRAY_BUFFER, uiVBOasteroid);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBOasteroids);
			glUniformMatrix4fv(MatrixIDFlat, 1, GL_FALSE, orthographicProjection);
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

		

		glfwSwapBuffers(window);






		//MOVEMENT
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			//move forward
			for (int i = 0; i < 3; i++)
			{
				playerShip[i].fPositions[1] += screenSize*.0001f;
			}
			glBindBuffer(GL_ARRAY_BUFFER, uiVBOplayerShip);
			GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
			//copy data to graphics card
			memcpy(vBuffer, playerShip, sizeof(Vertex)* 3);
			//unmap and unbind buffer
			glUnmapBuffer(GL_ARRAY_BUFFER);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			//move forward
			for (int i = 0; i < 3; i++)
			{
				playerShip[i].fPositions[1] -= screenSize*.0001f;
			}
			glBindBuffer(GL_ARRAY_BUFFER, uiVBOplayerShip);
			GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
			//copy data to graphics card
			memcpy(vBuffer, playerShip, sizeof(Vertex)* 3);
			//unmap and unbind buffer
			glUnmapBuffer(GL_ARRAY_BUFFER);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			//move forward
			for (int i = 0; i < 3; i++)
			{
				playerShip[i].fPositions[0] += screenSize*.0001f;
			}
			glBindBuffer(GL_ARRAY_BUFFER, uiVBOplayerShip);
			GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
			//copy data to graphics card
			memcpy(vBuffer, playerShip, sizeof(Vertex)* 3);
			//unmap and unbind buffer
			glUnmapBuffer(GL_ARRAY_BUFFER);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			//move forward
			for (int i = 0; i < 3; i++)
			{
				playerShip[i].fPositions[0] -= screenSize*.0001f;
			}
			glBindBuffer(GL_ARRAY_BUFFER, uiVBOplayerShip);
			GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
			//copy data to graphics card
			memcpy(vBuffer, playerShip, sizeof(Vertex)* 3);
			//unmap and unbind buffer
			glUnmapBuffer(GL_ARRAY_BUFFER);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}


		//poll for and process events
		glfwPollEvents();
	}
	glDeleteBuffers(1, &uiIBOplayerShip);
	glfwTerminate();
	return 0;
}