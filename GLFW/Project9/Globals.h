#pragma once
#include <GL/glew.h>
#include "wglew.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <fstream>
#include <SOIL.h>
#include <iostream>
#include <time.h>
class Globals
{
public:
	int screenSize = 1024;
	GLFWwindow * window;
	float *orthographicProjection;


	static Globals& instance()
	{
		static Globals * m_instance = new Globals();
		return *m_instance;
	}

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

	Globals();
	~Globals();
	
};
struct Vertex
{
	float fPositions[4];
	float fColours[4];
	float fUVs[2];
};
