#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
class Light
{
public:
	glm::vec3 lightColor;
	glm::vec3 lightPos;
	Light()
	{
		lightColor = glm::vec3(0.5, 0.5, 0.5);
		lightPos = glm::vec3(20,20,20);
	}

};