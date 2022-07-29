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
#include "Scene.h"
#include "Skybox.h"


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
    int height = 800;
    int width =800;
    int channel=3;
    float image[height*width*channel];

    for (int h=0;h<height;++h)
    {
        for(int w=0;w<width;++w)
        {
            image[h*width*channel+w*channel+0]=1.0;
            image[h*width*channel+w*channel+1]=0.0;
            image[h*width*channel+w*channel+2]=0.0;
        }
    }

    BillBoard* board = new BillBoard(image,height,width,channel);
    board->full_screen=true;


    cam = new Camera();
    cam->SetProjectionMatrix(WinW,WinH);
    Scene scene;
    scene.cam = cam;
    scene.light = new Light();

    scene.objects.push_back(board);
    //scene.skybox = new Skybox();
    //scene.skybox->SetShader(PROJECT_DIR"/shader/skybox/shader.vs", PROJECT_DIR"/shader/skybox/shader.fs");

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