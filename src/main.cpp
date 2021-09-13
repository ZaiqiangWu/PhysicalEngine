#define GLEW_STATIC
#include<gl/glew.h>
//#include<gl/freeglut.h>
#include <GLFW/glfw3.h>
#include<GLFW/glfw3native.h>
#include<iostream>
#include"shader.h"
#include "BillBoard.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "SOIL.h"
#include"Texture.h"
#include "Mesh.h"
#include "Model.h"
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
    
    glfwInit(); //初始化
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//主版本号3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//次版本号3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//使用核心模式
    
    

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
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);wireframe mode
    // test glew
    
    
    BillBoard* board = new BillBoard("../texture1.png");
    board->SetShader("../shader/billboard/shader.vs", "../shader/billboard/shader.fs");

    Shader* model_shader = new Shader("../shader/model/shader.vs", "../shader/model/shader.fs");
    
    
    //Model* myModel = new Model("../obj/grass/Palm_01.obj");
    //Model* myModel = new Model("../obj/girl/untitled.obj");
    Model* myModel = new Model("../obj/PEACE_LILLY_obj/PEACE_LILLY_5K.obj");
    myModel->Scale(30);

    cam = new Camera();
    cam->SetProjectionMatrix(WinW,WinH);
    Scene scene;
    scene.cam = cam;
    scene.light = new Light();
    scene.objects.push_back(myModel);
    scene.objects.push_back(board);
    scene.skybox = new Skybox();
    scene.skybox->SetShader("../shader/skybox/shader.vs", "../shader/skybox/shader.fs");
    
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        scene.Update();
        scene.Render();
        glfwSwapBuffers(window);
        glfwPollEvents();//检查有没有触发事件(键盘输入、鼠标移动)，新窗口状态，并调用对应的回调函数
    }
    glfwTerminate();//正确释放/删除之前分配的所有资源
    return 0;
   
}