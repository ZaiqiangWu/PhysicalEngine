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

/*
*  octahedron
static GLfloat vtx[] =
{
   0.0f, -1.0f,  0.0f,
   1.0f,  0.0f,  0.0f,
   0.0f,  0.0f,  1.0f,
  -1.0f,  0.0f,  0.0f,
   0.0f,  0.0f, -1.0f,
   0.0f,  1.0f,  0.0f
};

static GLuint idx[] =
{
  0,    1,    2,
  0,    2,    3,
  0,    3,    4,
  0,    4,    1,
  1,    5,    2,
  2,    5,    3,
  3,    5,    4,
  4,    5,    1
};
*/