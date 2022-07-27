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
#include <cmath>
#include "MatrixDecomposition.h"

class RigidBody: public Object
{
public:

    RigidBody()
    {
        renderModel = new Model(PROJECT_DIR"/obj/bunny/bunny.obj");//("../obj/cube/cube.obj");
        isStatic = false;
        
        renderModel->Translate(glm::vec3(18,6,50));
        renderModel->Scale(0.1);
        renderModel->Rotate(45,glm::vec3(1,0,0));
        //InitializeShapeMatchingData();
        InitializeImpulseData();
    }
    ~RigidBody()
    {

    }
    virtual void Step()
    {
        ImpulseMethod();
        //ShapeMatching();
    }
    
    void Render(Camera* cam, Light* light, GLuint depthMap)
    {
        renderModel->Render(cam,light,depthMap,GetMat4());
        
        
    }
    void GenDepthBuffer(glm::mat4 lightSpaceMatrix, Shader* shader)
    {
        renderModel->GenDepthBuffer(lightSpaceMatrix,shader,GetMat4());
    }
private:
    bool isStatic;
    Model* renderModel;
    vector<glm::vec3> vertices;
    float dt = 0.015f;
    glm::mat3 rotation = glm::mat3(1);
    glm::vec3 translate = glm::vec3(0);
    glm::vec3 t0;
    vector<glm::vec3> Q;
    //data for shape matching
    glm::mat3 QQt;
    glm::mat3 inv_QQt;
    vector<glm::vec3> velocity;
    //data for impulse method
    glm::quat q = glm::quat_cast(glm::mat3(1));
    glm::vec3 w = glm::vec3(0, 0, 0);
    glm::vec3 v = glm::vec3(0, 0, 0);
    glm::mat3 I_ref = glm::mat3(0);
    float linear_decay = 0.9995f;				// for velocity decay
    float angular_decay = 0.9995f;
    float restitution = 0.8f;                 // for collision
    float mu_T = 0.8f;
    float mass;
    
    
    void InitializeShapeMatchingData()
    {
        vector<Mesh> meshes = renderModel->GetMeshes();
        for (int i = 0; i < meshes.size(); ++i)
        {
            for (int j = 0; j < meshes[i].vertices.size(); ++j)
            {
                glm::vec3 position = meshes[i].vertices[j].Position;
                glm::vec4 p4=glm::vec4(position.x, position.y, position.z, 1.0f);
                p4 = renderModel->GetModelMatrix() * p4;
                position = glm::vec3(p4.x,p4.y,p4.z)/p4.w;
                vertices.push_back(position);
                velocity.push_back(glm::vec3(0));
            }
        }
        Q = vertices;
        //Centerizing Q
        glm::vec3 c = glm::vec3(0);
        for (int i = 0; i < Q.size(); ++i)
        {
            c += Q[i];
        }
        c /= Q.size();
        t0 = c;
        for (int i = 0; i < Q.size(); ++i)
        {
            Q[i] -= c;
        }
        QQt = glm::mat3(0);
        for (int i = 0; i < Q.size(); ++i)
        {
            QQt[0][0] += Q[i][0] * Q[i][0];
            QQt[1][0] += Q[i][0] * Q[i][1];
            QQt[2][0] += Q[i][0] * Q[i][2];
            QQt[0][1] += Q[i][1] * Q[i][0];
            QQt[1][1] += Q[i][1] * Q[i][1];
            QQt[2][1] += Q[i][1] * Q[i][2];
            QQt[0][2] += Q[i][2] * Q[i][0];
            QQt[1][2] += Q[i][2] * Q[i][1];
            QQt[2][2] += Q[i][2] * Q[i][2];
        }
        inv_QQt = glm::inverse(QQt);
    }
    void InitializeImpulseData()
    {
        vector<Mesh> meshes = renderModel->GetMeshes();
        for (int i = 0; i < meshes.size(); ++i)
        {
            for (int j = 0; j < meshes[i].vertices.size(); ++j)
            {
                glm::vec3 position = meshes[i].vertices[j].Position;
                glm::vec4 p4 = glm::vec4(position.x, position.y, position.z, 1.0f);
                p4 = renderModel->GetModelMatrix() * p4;
                position = glm::vec3(p4.x, p4.y, p4.z) / p4.w;
                vertices.push_back(position);
                velocity.push_back(glm::vec3(0));
            }
        }
        Q = vertices;
        //Centerizing Q
        glm::vec3 c = glm::vec3(0);
        for (int i = 0; i < Q.size(); ++i)
        {
            c += Q[i];
        }
        c /= Q.size();
        t0 = c;
        for (int i = 0; i < Q.size(); ++i)
        {
            Q[i] -= c;
        }
        float m = 1.0f;
        mass = 0;
        for (int i = 0; i < Q.size(); i++)
        {
            mass += m;
            float diag = m * Q[i].length()*Q[i].length();
            I_ref[0][0] += diag;
            I_ref[1][1] += diag;
            I_ref[2][2] += diag;
            I_ref[0][0] -= m * Q[i][0] * Q[i][0];
            I_ref[0][1] -= m * Q[i][0] * Q[i][1];
            I_ref[0][2] -= m * Q[i][0] * Q[i][2];
            I_ref[1][0] -= m * Q[i][1] * Q[i][0];
            I_ref[1][1] -= m * Q[i][1] * Q[i][1];
            I_ref[1][2] -= m * Q[i][1] * Q[i][2];
            I_ref[2][0] -= m * Q[i][2] * Q[i][0];
            I_ref[2][1] -= m * Q[i][2] * Q[i][1];
            I_ref[2][2] -= m * Q[i][2] * Q[i][2];
        }
    }
    glm::mat3 Get_Cross_Matrix(glm::vec3 a)
    {
        //Get the cross product matrix of vector a
        glm::mat3 A = glm::mat3(0);
        A[0][0] = 0;
        A[0][1] = -a[2];
        A[0][2] = a[1];
        A[1][0] = a[2];
        A[1][1] = 0;
        A[1][2] = -a[0];
        A[2][0] = -a[1];
        A[2][1] = a[0];
        A[2][2] = 0;
        return glm::transpose(A);
    }
    glm::mat3 ComputeR()
    {
        return glm::mat3_cast(q);
    }
    glm::mat3 ComputeInertia()
    {
        return ComputeR() * I_ref * glm::transpose(ComputeR());
    }
    glm::mat3 ComputeK(glm::vec3 r)
    {
        //Matrix3x3 K = I_ref * I_ref;

        glm::mat3 K = (1.0f / mass) * glm::mat3(1) - Get_Cross_Matrix(r) * glm::inverse(ComputeInertia()) * Get_Cross_Matrix(r);
        return K;
    }
    glm::vec3 GetVerticePosition(glm::vec3 p)
    {
        
        return ComputeR()*(p)+translate+t0;
    }
    void Collision_Impulse(glm::vec3 P, glm::vec3 N)
    {
        int num_in_collision = 0;
        glm::vec3 average_velocity = glm::vec3(0, 0, 0);
        glm::vec3 average_r = glm::vec3(0, 0, 0);

        for (int i = 0; i < Q.size(); i++)
        {
            if (glm::dot(GetVerticePosition(Q[i]) - P, N) < 0)
            {
                if (glm::dot(glm::cross(w, ComputeR() * Q[i]) + v, N) < 0)
                {
                    num_in_collision++;
                    ComputeR();
                    average_velocity += Get_Cross_Matrix(w) * ComputeR() * Q[i] + v;
                    average_r += ComputeR() * Q[i];
                }

            }
        }

        if (num_in_collision > 0)
        {
            average_velocity = average_velocity / (float)num_in_collision;
            average_r = average_r / (float)num_in_collision;
            glm::vec3 v_N = glm::dot(average_velocity, N) * N;
            glm::vec3 v_T = average_velocity - v_N;
            float a = 1 - mu_T * (1 + restitution) * v_N.length() / v_T.length();
            if (a < 0)
            {
                a = 0;
            }
            glm::vec3 v_N_new = -restitution * v_N;
            glm::vec3 v_T_new = a * v_T;
            glm::vec3 v_new = v_N_new + v_T_new;

            glm::vec3 impulse = glm::inverse(ComputeK(average_r)) * (v_new - average_velocity);

            glm::vec3 dv = (1.0f / mass) * impulse;
            v = v + dv;
            w = w + glm::inverse(ComputeInertia()) * Get_Cross_Matrix(average_r) * impulse;
        }
    }
    void ImpulseMethod()
    {
        v *= linear_decay;
        v += dt * glm::vec3(0, -10.1, 0);
        Collision_Impulse(glm::vec3(0, 0.01f, 0), glm::vec3(0, 1, 0));
        translate += dt * v;

        q = glm::quat(0, w * dt*0.5f)+q;
        q = glm::normalize(q);
        w *= angular_decay;
    }
    void ShapeMatching()
    {
        for (int i = 0; i < vertices.size(); ++i)
        {
            velocity[i] += dt * glm::vec3(0, -1.1f, 0);
            vertices[i] += dt * velocity[i];
        }
        Collision(glm::vec3(0, 0.01f, 0), glm::vec3(0, 1, 0));
        translate = glm::vec3(0);
        for (int i = 0; i < vertices.size(); ++i)
        {
            translate += vertices[i];
        }
        translate /= vertices.size();
        glm::mat3 A = glm::mat3(0);
        for (int i = 0; i < Q.size(); i++)
        {
            A[0][0] += (vertices[i] - translate)[0] * Q[i][0];
            A[1][0] += (vertices[i] - translate)[0] * Q[i][1];
            A[2][0] += (vertices[i] - translate)[0] * Q[i][2];
            A[0][1] += (vertices[i] - translate)[1] * Q[i][0];
            A[1][1] += (vertices[i] - translate)[1] * Q[i][1];
            A[2][1] += (vertices[i] - translate)[1] * Q[i][2];
            A[0][2] += (vertices[i] - translate)[2] * Q[i][0];
            A[1][2] += (vertices[i] - translate)[2] * Q[i][1];
            A[2][2] += (vertices[i] - translate)[2] * Q[i][2];
        }
        A = A * inv_QQt;

        rotation = polar_decomp(A).U;//PolarDecomposition(A);

        
        for (int i = 0; i < vertices.size(); ++i)
        {
            glm::vec3 x = rotation * Q[i] + translate;
            velocity[i] += (x - vertices[i]) / dt;
            vertices[i] = x;
        }

    }
    void Collision(glm::vec3 P, glm::vec3 N)
    {
        for (int i = 0; i < vertices.size(); ++i)
        {
            if (glm::dot(vertices[i] - P, N) < 0.0f)
            {
                if (glm::dot(velocity[i], N) < 0.0f)
                {
                    vertices[i] -= glm::dot(vertices[i] - P, N) * N;
                    glm::vec3 v_N = glm::dot(velocity[i], N) * N;
                    glm::vec3 v_T = velocity[i] - v_N;
                    float a = 1.0f - mu_T * (1.0f + restitution) * v_N.length() / v_T.length();
                    if (a < 0) a = 0;
                    glm::vec3 v_N_new = -restitution * v_N;
                    glm::vec3  v_T_new = a * v_T;
                    glm::vec3 v_new = v_N_new + v_T_new;
                    velocity[i] = v_new;
                }
            }
        }
    }
    glm::mat4 GetMat4()
    {
        glm::mat4 result;
        rotation = ComputeR();
        //result[0] = glm::vec4(rotation[0], 0);
        //result[1] = glm::vec4(rotation[1], 0);
        //result[2] = glm::vec4(rotation[2], 0);
        result[0] = glm::vec4(rotation[0], 0);
        result[1] = glm::vec4(rotation[1], 0);
        result[2] = glm::vec4(rotation[2], 0);
        //result[3] = glm::vec4(translate-rotation*t0, 1);
        result[3] = glm::vec4(translate - ComputeR() * t0+t0, 1);
        //result = glm::translate(result, translate);
        
        
        return result;
    }

};

#endif //PHYSICALENGINE_RIGIDBODY_H
