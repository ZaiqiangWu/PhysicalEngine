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
#include "Geometry.h"
#include <iostream>
using namespace std;
// octahedron
vector<GLfloat> vtx =
{
   0.0f, -1.0f,  0.0f,
   1.0f,  0.0f,  0.0f,
   0.0f,  0.0f,  1.0f,
  -1.0f,  0.0f,  0.0f,
   0.0f,  0.0f, -1.0f,
   0.0f,  1.0f,  0.0f
};

vector<GLuint> idx =
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

class Sphere : public Object
{
public:
	Sphere():mesh(vtx,idx), shader("../shader/basic/shader.vs", "../shader/basic/shader.gs", "../shader/basic/shader.fs")
	{
		mesh.Subdivision(2);
		//cout << mesh.v_datas.size() << endl;
		radius = 1.0f;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER,VBO);
		glBufferData(GL_ARRAY_BUFFER,mesh.v_datas.size()*sizeof(GLfloat),mesh.v_datas.data(),GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(GLuint), mesh.indices.data(), GL_STATIC_DRAW);
		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0); 
		glBindVertexArray(0);

		Scale(2);
		Translate(glm::vec3(-2,2,0));

	}
	~Sphere()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
	void Render(Camera* cam, Light* light, GLuint depthMap)
	{
		shader.use();
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		

		shader.setM4("model", GetModelMatrix());
		cam->SendToGPU(&shader);
		shader.set3Float("objectColor", 0.9, 0.9, 0.1);
		shader.set3Float("lightPos", light->lightPos);


		glDrawElements(GL_TRIANGLES, (GLsizei)mesh.indices.size(), GL_UNSIGNED_INT, 0);
		/*
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//wireframe mode
		shader.set3Float("objectColor", 0, 0, 0);
		glLineWidth(2);
		glDrawElements(GL_TRIANGLES, (GLsizei)mesh.indices.size(), GL_UNSIGNED_INT, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//go back
		*/
		glBindVertexArray(0);
	}
	void GenDepthBuffer(glm::mat4 lightSpaceMatrix, Shader* shader)
	{
		;
	}
private:
	float radius;
	geometry::Mesh mesh;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	Shader shader;
};

