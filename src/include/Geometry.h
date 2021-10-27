#pragma once
#include<vector>
#define GLEW_STATIC
#include <GL/glew.h>
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
			for (int i = 0; i < num_faces; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					temp_e.id[0] = idx[3 * i +  j%3];
					temp_e.id[1] = idx[3 * i +  (j+1)%3];
					if (!temp_e.is_in_vector(edges, temp_id))
					{
						edges.push_back(temp_e);
						temp_id = edges.size();
					}
				}
			}

		}
		Mesh()
		{
			;
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