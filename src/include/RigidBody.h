//
// Created by wuzaiqiang on 21-9-16.
//

#ifndef PHYSICALENGINE_RIGIDBODY_H
#define PHYSICALENGINE_RIGIDBODY_H
#include "shader.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Texture.h"
#include "Object.h"
#include "Light.h"
#include "Model.h"

class RigidBody: public Object
{
public:

    RigidBody()
    {
        renderModel=new Model("../obj/cube/cube.obj");

        renderModel->Translate(glm::vec3(18,6,0));
        renderModel->Scale(5);
    }
    ~RigidBody()
    {

    }
    virtual void Step()
    {
        renderModel->Translate(glm::vec3(0,0,0));
    }
    void Render(Camera* cam, Light* light, GLuint depthMap)
    {
        renderModel->Render(cam,light,depthMap);
    }
    void GenDepthBuffer(glm::mat4 lightSpaceMatrix, Shader* shader)
    {
        renderModel->GenDepthBuffer(lightSpaceMatrix,shader);
    }
private:
    bool isStatic;
    Model* renderModel;

};

#endif //PHYSICALENGINE_RIGIDBODY_H
