#define GLEW_STATIC
#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include<GLFW/glfw3native.h>
#include<iostream>
#include"shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "BillBoard.h"
#include "Checkerboard.hpp"
#include "SOIL.h"
#include"Texture.h"
#include "Mesh.h"
#include "Model.h"
#include "Text.h"
#include "Cloth.h"
#include "Sphere.h"
#include "Scene.h"
#include "RigidBody.h"
#include "mass_spring.h"


class Object;


extern GLint WinW;
extern GLint WinH;
//extern GLint framebufferWidth;
//extern GLint framebufferHeight;
//Camera* cam;





void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);//TODO:

    //glViewport(0, 0, framebufferWidth, framebufferHeight);
    //glViewport(0, 0, width, height);
    WinW = width;
    WinH = height;
    cam->SetProjectionMatrix(WinW, WinH);
}




int main(int argc, char** argv)
{
    //initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);// Core Profile mode
#if defined(__APPLE__)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);// for Mac OS
#endif

    GLFWwindow* window = glfwCreateWindow(WinW, WinH, "LearnOpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, curse_poscallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLE);
    glewExperimental = GL_TRUE;
    GLenum error = glewInit();
    if (error != GLEW_OK)
    {
        printf("GLEW CREATE ERROR");
        return -1;
    }
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);
    //glViewport(0, 0, WinW, WinH);
    
    //glFrontFace(GL_CCW);
    //glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);go back
    // test glew
    
    
    //BillBoard* board = new BillBoard("../texture1.png");


    CheckerBoard* checkerboard = new CheckerBoard();
    checkerboard->SetShader(PROJECT_DIR"/shader/checkerboard/shader.vs", PROJECT_DIR"/shader/checkerboard/shader.fs");

    //Shader* model_shader = new Shader("../shader/model/shader.vs", "../shader/model/shader.fs");

   // myModel->Scale(0.1);
    //Model* myModel = new Model(PROJECT_DIR"/obj/bunny/bunny.obj");
    
    Cloth* cloth=new Cloth();
    Sphere sphere;

    cam = new Camera();
    cam->SetProjectionMatrix(WinW,WinH);
    Scene scene;
    scene.cam = cam;
    scene.light = new Light();
    //scene.objects.push_back(myModel);
    //scene.objects.push_back(board);
    scene.objects.push_back(checkerboard);
    scene.objects.push_back(cloth);
    scene.objects.push_back(&sphere);
    RigidBody* box=new RigidBody();
    scene.rigidBodys.push_back(box);
    scene.skybox = new Skybox();
    scene.skybox->SetShader(PROJECT_DIR"/shader/skybox/shader.vs", PROJECT_DIR"/shader/skybox/shader.fs");
    
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        scene.Update();
        scene.Render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
   
}