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
        renderModel = new Model("../obj/PEACE_LILLY_obj/PEACE_LILLY_5K.obj");//("../obj/cube/cube.obj");
        isStatic = false;
        
        renderModel->Translate(glm::vec3(18,6,0));
        renderModel->Scale(15);
        renderModel->Rotate(45,glm::vec3(1,0,0));
        InitializeData();
    }
    ~RigidBody()
    {

    }
    virtual void Step()
    {
        
        for (int i = 0; i < vertices.size(); ++i)
        {
            velocity[i] += dt * glm::vec3(0, -1.1f, 0);
            vertices[i] += dt * velocity[i];
        }
        Collision_Impulse(glm::vec3(0, 0.01f, 0), glm::vec3(0, 1, 0));
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
        
        //PolarDecomposition(A);
        for (int i = 0; i < vertices.size(); ++i)
        {
            glm::vec3 x = rotation * Q[i] + translate;
            velocity[i] += (x - vertices[i]) / dt;
            vertices[i] = x;
        }
        
    }
    void Collision_Impulse(glm::vec3 P, glm::vec3 N)
    {
        float restitution = 15.9f;                 // for collision
        float mu_T = 0.9f;
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
                    glm::vec3 v_N_new= -restitution * v_N;
                    glm::vec3  v_T_new = a * v_T;
                    glm::vec3 v_new = v_N_new + v_T_new;
                    velocity[i] = v_new;
                }
            }
        }
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
    glm::vec3 t0;
    vector<glm::vec3> Q;
    glm::mat3 QQt;
    glm::mat3 inv_QQt;
    vector<glm::vec3> velocity;
    glm::mat3 rotation=glm::mat3(1);
    glm::vec3 translate = glm::vec3(0);
    float dt = 0.015f;
    void InitializeData()
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
    // Polar Decomposition that returns the rotation from F.
    glm::mat3 PolarDecomposition(glm::mat3 F)
    {
        
        cout <<"a" <<glm::determinant(F) << endl;
        glm::mat3 C = glm::mat3(0);
        for (int ii = 0; ii < 3; ii++)
            for (int jj = 0; jj < 3; jj++)
                for (int kk = 0; kk < 3; kk++)
                    C[jj][ii] += F[ii][kk] * F[jj][kk];

        glm::mat3 C2 = glm::mat3(0);
        for (int ii = 0; ii < 3; ii++)
            for (int jj = 0; jj < 3; jj++)
                for (int kk = 0; kk < 3; kk++)
                    C2[jj][ii] += C[kk][ii] * C[kk][jj];

        float det =glm::determinant(F);
            /*F[0, 0] * F[1, 1] * F[2, 2] +
            F[0, 1] * F[1, 2] * F[2, 0] +
            F[1, 0] * F[2, 1] * F[0, 2] -
            F[0, 2] * F[1, 1] * F[2, 0] -
            F[0, 1] * F[1, 0] * F[2, 2] -
            F[0, 0] * F[1, 2] * F[2, 1];*/
        

        float I_c = C[0][0] + C[1][1] + C[2][2];
        float I_c2 = I_c * I_c;
        float II_c = 0.5f * (I_c2 - C2[0][0] - C2[1][1] - C2[2][2]);
        float III_c = det * det;
        float k = I_c2 - 3 * II_c;

        glm::mat3 inv_U = glm::mat3(0);
        if (k < 1e-10f)
        {
            float inv_lambda = 1 / sqrt(I_c / 3);
            inv_U[0][0] = inv_lambda;
            inv_U[1][1] = inv_lambda;
            inv_U[2][2] = inv_lambda;
        }
        else
        {
            float l = I_c * (I_c * I_c - 4.5f * II_c) + 13.5f * III_c;
            float k_root = sqrt(k);
            float value = l / (k * k_root);
            if (value < -1.0f) value = -1.0f;
            if (value > 1.0f) value = 1.0f;
            float phi = acos(value);
            float lambda2 = (I_c + 2 * k_root * cos(phi / 3)) / 3.0f;
            float lambda = sqrt(lambda2);

            float III_u = sqrt(III_c);
            if (det < 0)   III_u = -III_u;
            float I_u = lambda + sqrt(-lambda2 + I_c + 2 * III_u / lambda);
            float II_u = (I_u * I_u - I_c) * 0.5f;


            float inv_rate, factor;
            inv_rate = 1 / (I_u * II_u - III_u);
            factor = I_u * III_u * inv_rate;

            glm::mat3 U = glm::mat3(0);
            U[0][0] = factor;
            U[1][1] = factor;
            U[2][2] = factor;

            factor = (I_u * I_u - II_u) * inv_rate;
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    U[i, j] += factor * C[i, j] - inv_rate * C2[i, j];

            inv_rate = 1 / III_u;
            factor = II_u * inv_rate;
            inv_U[0][0] = factor;
            inv_U[1][1] = factor;
            inv_U[2][2] = factor;

            factor = -I_u * inv_rate;
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    inv_U[i, j] += factor * U[i, j] + inv_rate * C[i, j];
        }

        glm::mat3 R = glm::mat3(0);
        
        for (int ii = 0; ii < 3; ii++)
            for (int jj = 0; jj < 3; jj++)
                for (int kk = 0; kk < 3; kk++)
                    R[jj][ii] += F[kk][ii] * inv_U[jj][kk];
        cout << "b" << glm::determinant(R) << endl;
        
        return R;
    }
    glm::mat4 GetMat4()
    {
        glm::mat4 result;
        
        result[0] = glm::vec4(rotation[0], 0);
        result[1] = glm::vec4(rotation[1], 0);
        result[2] = glm::vec4(rotation[2], 0);
        result[3] = glm::vec4(translate-rotation*t0, 1);
        //result = glm::translate(result, translate);
        
        
        return result;
    }

};

#endif //PHYSICALENGINE_RIGIDBODY_H
