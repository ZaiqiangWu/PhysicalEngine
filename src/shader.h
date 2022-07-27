#ifndef SHADER_H
#define SHADER_H
#define GLEW_STATIC
#include <GL/glew.h> // include glad to get all the required OpenGL headers
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	// the program ID
	unsigned int ID;

	// constructor reads and builds the shader
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	Shader(const GLchar* vertexPath, const GLchar* geometryPath, const GLchar* fragmentPath);
	// use/activate the shader
	void use();
	// utility uniform functions
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void set3Float(const std::string& name, float v1,float v2,float v3) const;
	void set3Float(const std::string& name, glm::vec3 v) const;
	void setM4(const std::string& name, glm::mat4 mat) const;
};

#endif