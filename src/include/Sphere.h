#pragma once
#include "shader.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Texture.h"
#include "Object.h"
#include "Light.h"

class Sphere : public Object
{
public:
	Sphere()
	{

	}
	~Sphere()
	{

	}
	void Render(Camera* cam, Light* light, GLuint depthMap)
	{

	}
	void GenDepthBuffer(glm::mat4 lightSpaceMatrix, Shader* shader)
	{

	}
private:
	float radius;
};