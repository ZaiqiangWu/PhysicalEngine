#pragma once
#include "Camera.h"
#include "Object.h"
#include<vector>
#include<chrono>
#include<cmath>
#include "Skybox.h"
#include<queue>
#include<string>
#include "RigidBody.h"
using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
class Scene
{
public:
	Camera* cam;
	vector<Object*> objects;
	vector<RigidBody*> rigidBodys;
	Light* light;
	Skybox* skybox;
	Text text;
	Scene()
    {
	    fps=0.0f;
	    t_begin=high_resolution_clock::now();
        InitDepthBuffer();
        depthShader=new Shader("../shader/shadowmap/shader.vs", "../shader/shadowmap/shader.fs");
    }
	void Render()
	{
	    t_queue.push(GetTime());
	    if(t_queue.size()>100)
	        t_queue.pop();

	    GenDepthBuffer();

	    //glClear(GL_COLOR_BUFFER_BIT);
        glViewport(0, 0, WinW, WinH);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox->Render(cam);
		for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->Render(cam,light);

		}
		for (int i = 0; i < rigidBodys.size(); i++)
		{
			rigidBodys[i]->Render(cam,light);

		}
		//text.RenderText("This is sample text"+to_string(fps), 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f),cam);
        text.RenderText("fps:"+to_string(fps).substr(0,4), 25.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f),cam);
	}
	void GenDepthBuffer()
    {
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        //ConfigureShaderAndMatrices;
        glm::mat4 lightProjection = glm::perspective(45.0f, 1.0f, 0.1f, 500.0f);
        glm::mat4 lightView = glm::lookAt(light->lightPos, cam->Position, glm::vec3(0.0, 1.0, 0.0));
        glm::mat4 lightSpacematrix=lightProjection*lightView;
        //RenderScene
        for (int i = 0; i < objects.size(); i++)
        {
            objects[i]->GenDepthBuffer(lightProjection,depthShader);

        }
        for (int i = 0; i < rigidBodys.size(); i++)
        {
            rigidBodys[i]->GenDepthBuffer(lightProjection,depthShader);

        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

	void Update()
	{
		ComputeFPS();
	    cam->UpdateMove();
	    cam->MovementSpeed=5.0f/fps;
		float t = (float)clock()/1000.0f;
		t = t * 0.05;
		glm::vec3 center = glm::vec3(0, 100, 0);
		float radius = 100.0f;
		light->lightPos=center+radius* glm::vec3(sin(t), 1, cos(t));
		for (int i = 0; i < rigidBodys.size(); i++)
		{
			rigidBodys[i]->Step();

		}
	}
private:
    float fps;
	queue<float> t_queue;
	high_resolution_clock::time_point t_begin;
    GLuint depthMapFBO;
    Shader* depthShader;

    void InitDepthBuffer()
    {

        glGenFramebuffers(1, &depthMapFBO);
        glBindTexture(GL_TEXTURE_2D, depthMapFBO);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                     SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapFBO, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

	void ComputeFPS()
    {
        if(!t_queue.empty())
        {
            fps=t_queue.size()/(t_queue.back()-t_queue.front());
        }
    }
    float GetTime()
	{
		return (float)std::chrono::duration_cast<milliseconds>(high_resolution_clock::now()-t_begin).count()/1000;
	}
};