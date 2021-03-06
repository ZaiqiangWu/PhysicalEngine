#pragma once
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

// GL Includes
#define GLEW_STATIC
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>

#include "Object.h"
#include "Mesh.h"
#include "Light.h"

GLint TextureFromFile(const char* path, std::string directory);


class Model: public Object
{
public:
    /*  Functions   */
    // Constructor, expects a filepath to a 3D model.
    Model(GLchar* path)
    {
        this->loadModel(path);
        //model_matrix = glm::translate(model_matrix, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
        Translate(glm::vec3(0.0f, 1.75f, 0.0f));
        //model_matrix = glm::scale(model_matrix, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
        Scale(glm::vec3(0.2f, 0.2f, 0.2f));
    }
    ~Model()
    {
        
    }
    // Draws the model, and thus all its meshes
    void Render(Camera* cam, Light* light, GLuint depthMap)
    {
        //glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        for (GLuint i = 0; i < this->meshes.size(); i++)
        {
            
            this->meshes[i].Draw(cam,light,GetModelMatrix());
        }
        
    }
    void Render(Camera* cam, Light* light, GLuint depthMap, glm::mat4 m)
    {
        //glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        for (GLuint i = 0; i < this->meshes.size(); i++)
        {

            this->meshes[i].Draw(cam, light, m*GetModelMatrix());
        }

    }
    void GenDepthBuffer(glm::mat4 lightSpaceMatrix, Shader* shader)
    {
        for (GLuint i = 0; i < this->meshes.size(); i++)
        {

            this->meshes[i].GenDepthBuffer(lightSpaceMatrix,shader,GetModelMatrix());
        }
    }
    void GenDepthBuffer(glm::mat4 lightSpaceMatrix, Shader* shader, glm::mat4 m)
    {
        for (GLuint i = 0; i < this->meshes.size(); i++)
        {

            this->meshes[i].GenDepthBuffer(lightSpaceMatrix, shader, m*GetModelMatrix());
        }
    }

    std::vector<Mesh> GetMeshes()
    {
        return meshes;
    }


private:
    /*  Model Data  */
    
    
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    //vector<Material> materials;

    /*  Functions   */
    // Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(std::string path)
    {
        
        // Read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
        // Check for errors
        if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
            return;
        }
        // Retrieve the directory path of the filepath
        this->directory = path.substr(0, path.find_last_of('/'));

        // Process ASSIMP's root node recursively
        this->processNode(scene->mRootNode, scene);
    }

    // Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene)
    {
        // Process each mesh located at the current node
        for (GLuint i = 0; i < node->mNumMeshes; i++)
        {
            // The node object only contains indices to index the actual objects in the scene. 
            // The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            this->meshes.push_back(this->processMesh(mesh, scene));
        }
        // After we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for (GLuint i = 0; i < node->mNumChildren; i++)
        {
            this->processNode(node->mChildren[i], scene);
        }

    }

    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    {
        // Data to fill
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture> textures;
        bool has_normal = true;

        // Walk through each of the mesh's vertices
        for (GLuint i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // Positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // Normals
            if (mesh->mNormals)
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            else
            {
                has_normal = false;
            }
            
            // Texture Coordinates
            if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            vertices.push_back(vertex);
        }
        // Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for (GLuint i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // Retrieve all indices of the face and store them in the indices vector
            for (GLuint j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
            //Compute normal 
            if (!has_normal)
            {
                for (int i = 0; i < vertices.size(); ++i)
                {
                    vertices[i].Normal = glm::vec3(0,0,0);

                }
                for (int i = 0; i < indices.size() / 3; ++i)
                {
                    glm::vec3 x0 = vertices[indices[3*i+0]].Position;
                    glm::vec3 x1 = vertices[indices[3 * i + 1]].Position;
                    glm::vec3 x2 = vertices[indices[3 * i + 2]].Position;
                    glm::vec3 n = glm::cross(x1-x0,x2-x1);
                    //std::cout << vertices[3 * i + 0].Normal.x << std::endl;
                    vertices[indices[3 * i + 0]].Normal += n;
                    vertices[indices[3 * i + 1]].Normal += n;
                    vertices[indices[3 * i + 2]].Normal += n;
                }
                for (int i = 0; i < vertices.size(); ++i)
                {
                    vertices[i].Normal = glm::normalize(vertices[i].Normal);
                    
                }
            }
        }
        // Process materials
        Material mat;
        if (mesh->mMaterialIndex >= 0)
        {
            //cout << "hi" << endl;
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            
            aiColor3D color;

            //Read mtl file vertex data

            material->Get(AI_MATKEY_COLOR_AMBIENT, color);
            mat.Ka = glm::vec3(color.r, color.g, color.b);
            material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
            mat.Kd = glm::vec3(color.r, color.g, color.b);
            material->Get(AI_MATKEY_COLOR_SPECULAR, color);
            mat.Ks = glm::vec3(color.r, color.g, color.b);
            material->Get(AI_MATKEY_SHININESS, color);

            //material->
            // We assume a convention for sampler names in the shaders. Each diffuse texture should be named
            // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
            // Same applies to other texture as the following list summarizes:
            // Diffuse: texture_diffuseN
            // Specular: texture_specularN
            // Normal: texture_normalN

            // 1. Diffuse maps
            std::vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            // 2. Specular maps
            std::vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            // 3. normal maps
            std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
            // 4. height maps
            std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
            textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        }
        mat.textures = textures;
        // Return a mesh object created from the extracted mesh data
        return Mesh(vertices, indices, mat);//textures);
    }

    // Checks all material textures of a given type and loads the textures if they're not loaded yet.
    // The required info is returned as a Texture struct.
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
    {
        std::vector<Texture> textures;
        for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
        {
            //cout << "hi" << endl;
            aiString str;
            mat->GetTexture(type, i, &str);
            //std::cout << str.C_Str() << std::endl;
            // Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            GLboolean skip = false;
            for (GLuint j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if (!skip)
            {   // If texture hasn't been loaded already, load it
                
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), this->directory);
                texture.type = typeName;
                texture.path = str;
                textures.push_back(texture);
                this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }
        }
        return textures;
    }
};




GLint TextureFromFile(const char* path, std::string directory)
{
    //Generate texture ID and load texture data 
    std::string filename = std::string(path);
    filename = directory + '/' + filename;
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width, height, channel;
    unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, &channel, SOIL_LOAD_AUTO);
    if (!image)
    {
        std::cout << "load texture failed" << std::endl;
    }
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    if (channel == 3)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    }
    else if (channel == 4)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    }
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);
    return textureID;
}

