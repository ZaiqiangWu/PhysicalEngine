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

class Cloth : public Object
{
public:
bool showWireframe;
	Cloth():shader(PROJECT_DIR"/shader/basic/shader.vs", PROJECT_DIR"/shader/basic/shader.gs", PROJECT_DIR"/shader/basic/shader.fs"),
    n(21),
	showWireframe(false)
	{

		GenerateMesh();
        glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER,VBO);
		glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(GLfloat),vertices.data(),GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()* sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0); 
		glBindVertexArray(0);

		Scale(0.5);
		Translate(glm::vec3(-30,12,0));

	}
	~Cloth()
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
		shader.set3Float("objectColor", 0.9, 0.1, 0.9);
		shader.set3Float("lightPos", light->lightPos);


		glDrawElements(GL_TRIANGLES, num_triangle*3, GL_UNSIGNED_INT, 0);
		
		if(showWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//wireframe mode
			shader.set3Float("objectColor", 0, 0, 0);
			glLineWidth(2);
			glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//go back
		}
		
		
		glBindVertexArray(0);
	}
	void GenDepthBuffer(glm::mat4 lightSpaceMatrix, Shader* shader)
	{
		;
	}
private:
	
	vector<GLfloat> vertices;
    
	vector<GLuint> indices;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	Shader shader;
    int n;
    int num_vtx;
    int num_triangle;
    void GenerateMesh()
    {
        GLfloat *vtx=new GLfloat[n*n*3];
        GLuint *idx=new GLuint[(n-1)*(n-1)*6];
        num_vtx = n*n*3;
        num_triangle = (n-1)*(n-1)*2;
        for(int j=0;j<n;++j)
        {
            for(int i=0;i<n;++i)
            {
                vtx[j*n*3+i*3+0] = 5-10.0f*i/(n-1);
                vtx[j*n*3+i*3+1] = 0;
                vtx[j*n*3+i*3+2] = 5-10.0f*j/(n-1);
            }
        }
        int t=0;
        for(int j=0;j<n-1;++j)
        {
            for(int i=0;i<n-1;++i)
            {
                idx[t*6+0]=j*n+i;
			    idx[t*6+1]=j*n+i+1;
			    idx[t*6+2]=(j+1)*n+i+1;
			    idx[t*6+3]=j*n+i;
			    idx[t*6+4]=(j+1)*n+i+1;
			    idx[t*6+5]=(j+1)*n+i;
			    t++;
            }
        }
		vertices = vector<GLfloat>(&vtx[0], &vtx[n*n*3]);
		indices = vector<GLuint>(&idx[0], &idx[(n-1)*(n-1)*6]);
		delete [] vtx;
		delete [] idx;

    }
};

