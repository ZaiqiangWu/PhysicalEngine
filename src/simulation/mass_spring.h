#pragma once
#include <iostream>
#include "vector3.h"
using Vector3 = vector3<float>;

using namespace std;
class MassSpring: public Object
{
public:
    MassSpring(vector<GLfloat>& vtx, vector<GLuint>& idx):
    vertices(vtx),
    indices(idx)
    {
        //Construct the original E
        vector<int> _E(indices.size()*2);
		for (int i=0; i<indices.size(); i+=3) 
		{
			_E[i*2+0]=indices[i+0];
			_E[i*2+1]=indices[i+1];
			_E[i*2+2]=indices[i+1];
			_E[i*2+3]=indices[i+2];
			_E[i*2+4]=indices[i+2];
			_E[i*2+5]=indices[i+0];
		}
		//Reorder the original edge list
		for (int i=0; i<_E.size(); i+=2)
			if(_E[i] > _E[i + 1]) 
				Swap(_E[i], _E[i+1]);
		//Sort the original edge list using quicksort
		Quick_Sort (_E.data(), 0, _E.size()/2-1);

		int e_number = 0;
		for (int i=0; i<_E.size(); i+=2)
			if (i == 0 || _E [i + 0] != _E [i - 2] || _E [i + 1] != _E [i - 1]) 
					e_number++;

		E = vector<int>(e_number * 2);
		for (int i=0, e=0; i<_E.size(); i+=2)
			if (i == 0 || _E [i + 0] != _E [i - 2] || _E [i + 1] != _E [i - 1]) 
			{
				E[e*2+0]=_E [i + 0];
				E[e*2+1]=_E [i + 1];
				e++;
			}

		L = vector<float>(E.size()/2);
		for (int e=0; e<E.size()/2; e++) 
		{
			int v0 = E[e*2+0];
			int v1 = E[e*2+1];
			L[e]=(X[v0]-X[v1]).magnitude;
		}

		V = vector<Vector3>(vertices.size()/3);
		for (int i=0; i<V.size(); i++)
			V[i] = Vector3 (0, 0, 0);
    }
private:
    vector<GLfloat> &vertices;
    vector<GLuint> &indices;
    vector<Vector3> X;
    float 		t 		= 0.0333f;
	float 		mass	= 1;
	float		damping	= 0.999f;
	float       rho = 0.85f;
	float 		spring_k = 8000;
	vector<int> E;
	vector<float> 	L;
	vector<Vector3> 	V;
	Vector3 g = Vector3(0, -10.0f, 0);
    void UpdateXfromVertices()
    {
        X = vector<Vector3>(vertices.size()/3);
        for(int i=0;i<X.size();++i)
        {
            X[i] = Vector3(vertices[i*3+0],vertices[i*3+1],vertices[i*3+2]);
        }
    }
    void UpdateVerticesfromX()
    {
        for(int i=0;i<X.size();++i)
        {
            vertices[i*3+0] = X[i].ptr()[0];
            vertices[i*3+1] = X[i].ptr()[1];
            vertices[i*3+2] = X[i].ptr()[2];
        }
    }
    void Quick_Sort(int* a, int l, int r)
	{
		int j;
		if(l<r)
		{
			j=Quick_Sort_Partition(a, l, r);
			Quick_Sort (a, l, j-1);
			Quick_Sort (a, j+1, r);
		}
	}

	int  Quick_Sort_Partition(int* a, int l, int r)
	{
		int pivot_0, pivot_1, i, j;
		pivot_0 = a [l * 2 + 0];
		pivot_1 = a [l * 2 + 1];
		i = l;
		j = r + 1;
		while (true) 
		{
			do ++i; while( i<=r && (a[i*2]<pivot_0 || a[i*2]==pivot_0 && a[i*2+1]<=pivot_1));
			do --j; while(  a[j*2]>pivot_0 || a[j*2]==pivot_0 && a[j*2+1]> pivot_1);
			if(i>=j)	break;
			Swap(a[i*2], a[j*2]);
			Swap(a[i*2+1], a[j*2+1]);
		}
		Swap (a [l * 2 + 0], a [j * 2 + 0]);
		Swap (a [l * 2 + 1], a [j * 2 + 1]);
		return j;
	}

	void Swap(int& a, int& b)
	{
		int temp = a;
		a = b;
		b = temp;
	}

};