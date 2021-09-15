#pragma once
#include "Camera.h"
#include "Object.h"
#include<vector>
#include<chrono>
#include<cmath>
#include "Skybox.h"
#include<queue>
#include<string>
using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
class Scene
{
public:
	Camera* cam;
	vector<Object*> objects;
	Light* light;
	Skybox* skybox;
	Text text;
	Scene()
    {
	    fps=0.0f;
	    t_begin=high_resolution_clock::now();
    }
	void Render()
	{
	    t_queue.push(GetTime());
	    if(t_queue.size()>100)
	        t_queue.pop();

	    //glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox->Render(cam);
		for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->Render(cam,light);

		}
		//text.RenderText("This is sample text"+to_string(fps), 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f),cam);
        text.RenderText("fps:"+to_string(fps).substr(0,4), 25.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f),cam);
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
	}
private:
    float fps;
	queue<float> t_queue;
	high_resolution_clock::time_point t_begin;
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