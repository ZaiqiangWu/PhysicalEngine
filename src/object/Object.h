#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include"Camera.h"
#include"Light.h"
#include "shader.h"
class Object
{
public:
	virtual void Render(Camera* cam, Light* light, GLuint depthMap)
	{
		;
	}
	virtual void GenDepthBuffer(glm::mat4 lightSpaceMatrix, Shader* shader)
	{
		;
	}
	
	virtual ~Object()
	{
		;
	}

	//Transform
	void Scale(float s)
	{
		model_matrix = glm::scale(model_matrix, glm::vec3(s, s, s));
	}
	void Translate(glm::vec3 v)
	{
		model_matrix = glm::translate(model_matrix, v);
	}
	void Scale(glm::vec3 v)
	{
		model_matrix = glm::scale(model_matrix, v);
	}
	void Rotate(float theta,glm::vec3 axis)
	{
		model_matrix = glm::rotate(model_matrix, theta, axis);
	}
	glm::mat4 GetModelMatrix()
	{
		return model_matrix;
	}
	void SetModelMatrix(glm::mat4 m)
	{
		model_matrix = m;
	}
private:
	glm::mat4 model_matrix;
};