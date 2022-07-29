#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include "SOIL.h"
#include<iostream>
#include<string>

//#define	STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_STATIC
//#include <stb_image.h>


class Texture2D
{
public:
	int width, height,channel;
	//GLuint texture;
	GLuint id;
	std::string type;
	std::string path;
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

    Texture2D(unsigned char* image, int height, int width, int channel)
    {
        LoadTexture(image,height,width,channel);
    }
	
	~Texture2D()
	{
		
		glBindTexture(GL_TEXTURE_2D, 0);
		
	}
private:

    void LoadTexture(unsigned char* image, int height, int width, int channel)
    {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        // Set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // Set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        if (!image)
        {
            std::cout << "Image array is null!" << std::endl;
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
        glGenerateMipmap(GL_TEXTURE_2D);

        //glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
	
	void LoadImage(const char* path)
	{
		glGenTextures(1, &id);

        //cout<<"id:"<<id<<endl;
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
			std::cout << "Load image failed!" << std::endl;
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
        glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);

		//glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};