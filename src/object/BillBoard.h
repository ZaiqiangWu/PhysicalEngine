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
class BillBoard: public Object
{
public:
    Texture2D tex;
    GLfloat height, width;
    BillBoard(const char* path)
    {
        Translate(glm::vec3(0,1,0));
        SetShader("../shader/billboard/shader.vs", "../shader/billboard/shader.fs");
        shadowShader=new Shader("../shader/billboard/shadow.vs", "../shader/billboard/shadow.fs");
        tex=Texture2D(path);
        height = (float)(tex.height) / (float)(tex.width);
        width = 1.0f;
        GLfloat vertices[] = {
            // Positions         // Colors
             0.5f * width, 0.5f*height, -10.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, // Bottom Right
            0.5f * width, -0.5f * height, -10.0f,  1.0f, 1.0f, 0.0f,   1.0f, 1.0f, // Bottom Left
             -0.5f * width,  -0.5f * height, -10.0f,  0.0f, 1.0f, 1.0f,   0.0f, 1.0f,// Top 
             -0.5f * width,  0.5f * height, -10.0f,  0.0f, 0.0f, 0.5f,   0.0f, 0.0f
        };
        GLuint indices[] = {
            0, 1, 3,
            1, 2, 3
        };
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        // TexCoord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);


        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //glBindVertexArray(0); // Unbind VAO
    }
    ~BillBoard()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        if (myShader)
        {
            delete myShader;
        }
       
    }
    void Render(Camera* cam, Light* light, GLuint depthMap)
    {


        glUseProgram(myShader->ID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex.id);
       
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);

        //GLint modelLoc = glGetUniformLocation(myShader->ID, "model");
        //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(GetModelMatrix()));
        myShader->setM4("model",GetModelMatrix());
        cam->SendToGPU(myShader);
    }

    void GenDepthBuffer(glm::mat4 lightSpaceMatrix, Shader* shader)
    {
        shadowShader->use();
        shadowShader->setM4("model",GetModelMatrix());
        shadowShader->setM4("lightSpaceMatrix",lightSpaceMatrix);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex.id);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);

    }

    void SetShader(const GLchar* vertexPath, const GLchar* fragmentPath)
    {
        myShader = new Shader(vertexPath, fragmentPath);
    }
private:
    Shader* myShader;
    Shader* shadowShader;
    GLuint VBO, VAO;
    GLuint EBO;
};