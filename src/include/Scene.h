#pragma once
#include "Camera.h"
#include "Object.h"
#include<vector>
#include<ctime>
#include<cmath>
#include "Skybox.h"
using namespace std;
class Scene
{
public:
	Camera* cam;
	vector<Object*> objects;
	Light* light;
	Skybox* skybox;
	void Render()
	{
		//glClear(GL_COLOR_BUFFER_BIT); 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox->Render(cam);
		for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->Render(cam,light);
			//cam->SenToGPU();
		}
	}

	void Update()
	{
		cam->UpdateMove();
		float t = (float)clock()/1000.0f;
		t = t * 5;
		glm::vec3 center = glm::vec3(0, 100, 0);
		float radius = 100.0f;
		light->lightPos=center+radius* glm::vec3(sin(t), 1, cos(t));
	}
};