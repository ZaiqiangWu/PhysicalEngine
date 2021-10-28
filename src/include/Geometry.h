#pragma once
#include<vector>
#define GLEW_STATIC
#include <GL/glew.h>
#include"vector3.h"
using Vector3 = vector3<float>;
using namespace std;
namespace geometry 
{
	class Vertex
	{
	public:
		float x, y, z;
	};

	class Edge
	{
	public:
		GLuint id[2] = {0,0};
		bool is_splited=false;
		GLuint split_id=0;
		bool is_same(Edge& e)
		{
			if (id[0] == e.id[0] && id[1] == e.id[1])
			{
				return true;
			}
			else if (id[0] == e.id[1] && id[1] == e.id[0])
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		bool is_in_vector(vector<Edge>& edges, GLuint& id)
		{
			bool res = false;
			for (int i = 0; i < edges.size(); ++i)
			{
				if (is_same(edges[i]))
				{
					res = true;
					id = i;
					break;
				}
			}
			return res;
		}
		int common_id(Edge& e)
		{
			int res = -1;
			if(id[0]==e.id[0]|| id[0] == e.id[1])
			{
				res=id[0];
			}
			else if (id[1] == e.id[0] || id[1] == e.id[1])
			{
				res = id[1];
			}
			return res;
		}
		void split(vector<Vertex>& vtx)
		{
			if (is_splited)
			{
				return;
			}
			else
			{
				Vertex temp;
				temp.x = (vtx[id[0]].x + vtx[id[1]].x) / 2.0f;
				temp.y = (vtx[id[0]].y + vtx[id[1]].y) / 2.0f;
				temp.z = (vtx[id[0]].z + vtx[id[1]].z) / 2.0f;
				vtx.push_back(temp);
				split_id = vtx.size()-1;
				is_splited = true;
			}
		}
	};

	class Triangle
	{
	public:
		GLuint id[3];
	};

	class Mesh
	{
	public:
		vector<Vertex> vertices;
		vector<Edge> edges;
		vector<Triangle> triangles;
		vector<GLfloat> v_datas;
		vector<GLuint> indices;
		Mesh(GLfloat vtx[], GLuint idx[])
		{
			unsigned int num_vertices = (unsigned int)(sizeof(vtx)/sizeof(GLfloat)/3);
			Vertex temp;
			for (int i = 0; i < num_vertices; ++i)
			{
				temp.x = vtx[3 * i + 0];
				temp.y = vtx[3 * i + 1];
				temp.z = vtx[3 * i + 2];
				this->vertices.push_back(temp);
			}
			unsigned int num_faces = (unsigned int)(sizeof(idx) / sizeof(GLuint) / 3);
			Edge temp_e;
			GLuint temp_id;
			Triangle temp_tri;
			for (int i = 0; i < num_faces; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					temp_e.id[0] = idx[3 * i +  j%3];
					temp_e.id[1] = idx[3 * i +  (j+1)%3];
					if (!temp_e.is_in_vector(edges, temp_id))
					{
						edges.push_back(temp_e);
						temp_id = edges.size()-1;
					}
					temp_tri.id[j] = temp_id;
				}
				triangles.push_back(temp_tri);
			}
			UpdateData();
		}
		Mesh()
		{
			;
		}
		void Subdivision()
		{
			//vector<Vertex> vertices;
			vector<Edge> new_edges;
			vector<Triangle> new_triangles;
			Edge temp_e[3];
			GLuint e_id[3];
			Triangle temp_tri;
			for (int i = 0; i < triangles.size(); ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					edges[triangles[i].id[0 + j % 3]].split(vertices);
					edges[triangles[i].id[0 + (j+1) % 3]].split(vertices);//add new vertex
					temp_e[0].id[0] = edges[triangles[i].id[0 + j % 3]].common_id(edges[triangles[i].id[0 + (j + 1) % 3]]);
					temp_e[0].id[1] = edges[triangles[i].id[0 + j % 3]].split_id;
					temp_e[1].id[0] = edges[triangles[i].id[0 + j % 3]].common_id(edges[triangles[i].id[0 + (j + 1) % 3]]);
					temp_e[1].id[1] = edges[triangles[i].id[0 + (j+1) % 3]].split_id;
					temp_e[2].id[0] = edges[triangles[i].id[0 + j % 3]].split_id;
					temp_e[2].id[1] = edges[triangles[i].id[0 + (j + 1) % 3]].split_id;
					for (int k = 0; k < 3; ++k)
					{
						if (!temp_e[k].is_in_vector(new_edges, e_id[k]))
						{
							new_edges.push_back(temp_e[k]);
							e_id[k] = new_edges.size()-1;
						}
					}
					for (int k = 0; k < 3; k++)
					{
						temp_tri.id[k] = e_id[k];
					}
					new_triangles.push_back(temp_tri);
				}
				for (int j = 0; j < 3; ++j)
				{
					temp_e[j].id[0]= edges[triangles[i].id[0 + j % 3]].split_id;
					temp_e[j].id[1] = edges[triangles[i].id[0 + (j+1) % 3]].split_id;
				}
				for (int k = 0; k < 3; ++k)
				{
					if (!temp_e[k].is_in_vector(new_edges, e_id[k]))
					{
						new_edges.push_back(temp_e[k]);
						e_id[k] = new_edges.size() - 1;
					}
				}
				for (int k = 0; k < 3; k++)
				{
					temp_tri.id[k] = e_id[k];
				}
			}
			edges = new_edges;
			triangles = new_triangles;
			UpdateData();
		}
		void Subdivision(unsigned int n)
		{
			for (int i = 0; i < n; ++i)
			{
				Subdivision();
			}
		}
		void UpdateData()
		{
			v_datas.clear();
			indices.clear();

		}
	};
}

/*
*  octahedron
static GLfloat vtx[] =
{
   0.0f, -1.0f,  0.0f,
   1.0f,  0.0f,  0.0f,
   0.0f,  0.0f,  1.0f,
  -1.0f,  0.0f,  0.0f,
   0.0f,  0.0f, -1.0f,
   0.0f,  1.0f,  0.0f
};

static GLuint idx[] =
{
  0,    1,    2,
  0,    2,    3,
  0,    3,    4,
  0,    4,    1,
  1,    5,    2,
  2,    5,    3,
  3,    5,    4,
  4,    5,    1
};
*/