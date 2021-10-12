#pragma once
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include"shader.h"
#include <SOIL.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Texture.h"

// GL Includes
#define GLEW_STATIC
#include <GL/glew.h> // Contains all the necessery OpenGL includes

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


struct Vertex {
    // Position
    glm::vec3 Position;
    // Normal
    glm::vec3 Normal;
    // TexCoords
    glm::vec2 TexCoords;
};


struct Texture {
    GLuint id;
    std::string type;
    aiString path;
};




//ka��kd��ks����Ӧ�Ļ����⣬�����估���淴��Ĳ�����mTextureName�����Ÿò��ʵ����ƣ���mtl�ļ����룬mTextureId��������ͼ��
//https://www.programmersought.com/article/5130383977/
class Material
{
public:
    //float Ns;     //shinness
    glm::vec3 Ka;  //������
    glm::vec3 Kd; //������
    glm::vec3 Ks; //���淴��
    std::vector<Texture> textures;
    Shader* shader;
    std::string name;
    Material()
    {
        shader = new Shader("../shader/model/shader.vs", "../shader/model/shader.fs");
        textures.clear();
    }
    ~Material()
    {
        if (shader)
        {
            //delete shader;
        }
    }
    void Apply()
    {
        shader->use();     
    }
};

class Mesh {
public:
    /*  Mesh Data  */
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    //vector<Texture> textures;
    Material material;

    /*  Functions  */
    // Constructor
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, Material material)//vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->material = material;// .textures = textures;

        // Now that we have all the required data, set the vertex buffers and its attribute pointers.
        this->setupMesh();
    }

    // Render the mesh
    void Draw(Camera* cam, Light* light, glm::mat4 model)
    {
        // Bind appropriate textures
        GLuint diffuseNr = 1;
        GLuint specularNr = 1;
        GLuint normalNr = 1;
        GLuint heightNr = 1;
        material.Apply();
        
        for (GLuint i = 0; i < this->material.textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
            // Retrieve texture number (the N in diffuse_textureN)
            std::stringstream ss;
            std::string number;
            std::string name = this->material.textures[i].type;
            if (name == "texture_diffuse")
            {
                ss << diffuseNr++; // Transfer GLuint to stream
            }

            else if (name == "texture_specular")
            {
                ss << specularNr++; // Transfer GLuint to stream
            }

            else if (name == "texture_normal")
                ss << normalNr++; // transfer unsigned int to stream
            else if (name == "texture_height")
                heightNr++; // transfer unsigned int to stream

            number = ss.str();
            // Now set the sampler to the correct texture unit
            glUniform1i(glGetUniformLocation(material.shader->ID, (name + number).c_str()), i);
            // And finally bind the texture
            glBindTexture(GL_TEXTURE_2D, this->material.textures[i].id);
        }

        // Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
        //glUniform1f(glGetUniformLocation(material.shader->ID, "material.shininess"), 16.0f);

        // Draw mesh
        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Always good practice to set everything back to defaults once configured.
        for (GLuint i = 0; i < this->material.textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        //glUniformMatrix4fv(glGetUniformLocation(material.shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        material.shader->setM4("model",model);
        //cam->SendToGPU(material.shader);
        material.shader->setM4("view", cam->GetViewMatrix());
        material.shader->setM4("projection", cam->GetProjectionMatrix());
        
        GLint lightColorLoc = glGetUniformLocation(material.shader->ID, "lightColor");
        GLint lightPosLoc = glGetUniformLocation(material.shader->ID, "lightPos");
        GLint viewPosLoc = glGetUniformLocation(material.shader->ID, "viewPos");
        glUniform3f(lightColorLoc, light->lightColor.r, light->lightColor.g, light->lightColor.b);
        glUniform3f(lightPosLoc, light->lightPos.x, light->lightPos.y, light->lightPos.z);
        glUniform3f(viewPosLoc, cam->Position.x, cam->Position.y, cam->Position.z);
       //material.SetFlag();
        //glUniform1i(glGetUniformLocation(material.shader->ID, "TextureValid"), (int)(material.textures.size()>0));
        material.shader->setInt("TextureValid", (int)(material.textures.size() > 0));
        material.shader->set3Float("Ambient",material.Ka.r, material.Ka.g, material.Ka.b);
        material.shader->set3Float("Diffuse", material.Kd.r, material.Kd.g, material.Kd.b);
        material.shader->set3Float("Specular", material.Ks.r, material.Ks.g, material.Ks.b);
        //GLboolean 
    }

    void GenDepthBuffer(glm::mat4 lightSpaceMatrix, Shader* shader,glm::mat4 model)
    {
        shader->use();
        shader->setM4("model",model);
        shader->setM4("lightSpaceMatrix",lightSpaceMatrix);
        // Draw mesh
        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    /*  Render data  */
    GLuint VAO, VBO, EBO;

    /*  Functions    */
    // Initializes all the buffer objects/arrays
    void setupMesh()
    {
        // Create buffers/arrays
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glGenBuffers(1, &this->EBO);

        glBindVertexArray(this->VAO);
        // Load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

        // Set the vertex attribute pointers
        // Vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
        // Vertex Normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
        // Vertex Texture Coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

        glBindVertexArray(0);
    }
};


