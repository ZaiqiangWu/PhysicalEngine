#define GLEW_STATIC
#include<gl/glew.h>
//#include<gl/freeglut.h>
#include <GLFW/glfw3.h>
#include<GLFW/glfw3native.h>
#include<iostream>
#include"shader.h"
#include "BillBoard.h"
#include "Checkerboard.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "SOIL.h"
#include"Texture.h"
#include "Mesh.h"
#include "Model.h"
#include "Text.h"
#include "Cloth.h"
#include "Scene.h"

using namespace std;

class Object;
using namespace std;


extern GLint WinW;
extern GLint WinH;
//Camera* cam;





void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    WinW = width;
    WinH = height;
    cam->SetProjectionMatrix(WinW, WinH);
}




int main(int argc, char** argv)
{
    //initialization
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);// Core Profile mode
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);// for Mac OS
    

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
    glViewport(0, 0, WinW, WinH);

    //glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);go back
    // test glew
    
    
    BillBoard* board = new BillBoard("../texture1.png");


    CheckerBoard* checkerboard = new CheckerBoard();
    checkerboard->SetShader("../shader/checkerboard/shader.vs", "../shader/checkerboard/shader.fs");

    //Shader* model_shader = new Shader("../shader/model/shader.vs", "../shader/model/shader.fs");
    
    
    //Model* myModel = new Model("../obj/grass/Palm_01.obj");
    //Model* myModel = new Model("../obj/girl/untitled.obj");
    Model* myModel = new Model("../obj/PEACE_LILLY_obj/PEACE_LILLY_5K.obj");
    //Model* myModel = new Model("../obj/test_cube.obj");
    myModel->Scale(30);

    Cloth* cloth=new Cloth();

    cam = new Camera();
    cam->SetProjectionMatrix(WinW,WinH);
    Scene scene;
    scene.cam = cam;
    scene.light = new Light();
    scene.objects.push_back(myModel);
    scene.objects.push_back(board);
    scene.objects.push_back(checkerboard);
    scene.objects.push_back(cloth);
    RigidBody* box=new RigidBody();
    scene.rigidBodys.push_back(box);
    scene.skybox = new Skybox();
    scene.skybox->SetShader("../shader/skybox/shader.vs", "../shader/skybox/shader.fs");
    
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