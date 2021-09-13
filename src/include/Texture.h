#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include "SOIL.h"
#include<iostream>
#include<string>

//#define	STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_STATIC
//#include <stb_image.h>
using namespace std;

class Texture2D
{
public:
	int width, height,channel;
	//GLuint texture;
	GLuint id;
	string type;
	string path;
	Texture2D()
	{
		width = 0;
		height = 0;
		channel = 0;
	}
	Texture2D(const char* path)
	{
		LoadImage(path);
	}
	
	~Texture2D()
	{
		
		glBindTexture(GL_TEXTURE_2D, 0);
		
	}
private:
	
	
	void LoadImage(const char* path)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		// Set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		unsigned char* image;
		image = SOIL_load_image(path, &width, &height, &channel, SOIL_LOAD_AUTO);
		if (!image)
		{
			cout << "Load image failed!" << endl;
		}
		if (channel == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

		}
		else if (channel == 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			//cout << "hi" << endl;
		}
		SOIL_free_image_data(image);
		glGenerateMipmap(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0); //在绑定纹理之前先激活纹理单元
		glBindTexture(GL_TEXTURE_2D, id);
	}
};