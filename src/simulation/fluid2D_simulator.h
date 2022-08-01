//
// Created by Zaiqiang Wu on 2022/07/29.
//

#ifndef PHYSICALENGINE_FLUID2D_SIMULATOR_H
#define PHYSICALENGINE_FLUID2D_SIMULATOR_H
#include "mathlib/vector3.h"
#include "mathlib/vector2.h"

using Vector3=vector3<float>;
using Vector2=vector2<float>;

class Fluid2D
{
public:
    int height;
    int width;
    Vector3* color;
    float* u_x;
    float* u_y;
    float delta_t;
    float grid_size;
    Fluid2D(int h, int w)
    {
        height=h;
        width=w;
        color=new Vector3[h*w];
        u_x=new float[h*(w+1)];
        u_y=new float[(h+1)*w];
        delta_t=0.1f;
        grid_size =1.0f;
        init_color();
        init_velocity();

    }
    void step()
    {
        advection();
        projection();
    }
    ~Fluid2D()
    {
        delete [] color;
        delete [] u_x;
        delete [] u_y;
    }
private:
    void init_color()
    {
        for(int h=0;h<height;++h)
        {
            for(int w=0;w<width;++w)
            {
                if (h+w>height)
                {
                    color[h*width+w][0]=1.0;
                    color[h*width+w][1]=1.0;
                    color[h*width+w][2]=1.0;
                }
                else
                {
                    color[h*width+w][0]=0.0;
                    color[h*width+w][1]=0.0;
                    color[h*width+w][2]=0.0;
                }
            }
        }
    }
    void init_velocity()
    {
        for(int h=0;h<height;++h)
        {
            for(int w=0;w<width+1;++w)
            {
                if(w==0||w==width)//boundary
                {
                    u_x[h*(width+1)+w]=0.0;
                }
                else
                {
                    if(w>width/2)
                    {
                        u_x[h*(width+1)+w]=1.0;
                    }
                    else
                    {
                        u_x[h*(width+1)+w]=0.0;
                    }
                }
            }
        }
        for(int h=0;h<height+1;++h)
        {
            for(int w=0;w<width;++w)
            {
                if(h==0||h==height)
                {
                    u_y[h*width+w]=0.0;
                }
                else
                {
                    if(h>height/2)
                    {
                        u_y[h*width+w]=1.0;
                    }
                    else
                    {
                        u_y[h*width+w]=0.0;
                    }
                }
            }
        }
    }
    void advection()
    {
        //semi-lagrangian
        //for color
        for(int h=0;h<height;++h)
        {
            for(int w=0;w<width;++w)
            {
                Vector2 p=Vector2();
                p[0]=((float)w+0.5)*grid_size;
                p[1]=((float)h+0.5)*grid_size;
                Vector2 v=Vector2();
                v[0]=(u_x[h*(width+1)+w]+u_x[h*(width+1)+w+1])*0.5f;
                v[1]=u_y[h*width+w]+u_y[(h+1)*width+w];
                Vector2 old_p=back_trace(x,y,v_x,v_y);

            }
        }

    }
    Vector2 back_trace(Vector2 p, Vector2 v)
    {
        old_x = x-v_x*delta_t;
        old_y=y-v_y*delta_t;
        float t;
        clipping(x,y,old_x,old_y,t);
        old_x=x+(old_x-x)*t;
        old_y=y+(old_y-y)*t;
    }

    void clipping(float x0, float y0, float x1, float y1, float &t)
    {
        float eps = std::numeric_limits<float>::epsilon();
        float eps_x =eps:(-eps)?(x1-x0)>0;
        float eps_y =eps:(-eps)?(y1-y0)>0;
        float t_x1=-x0/(x1-x0+eps_x);
        float t_x2=((float)width-x0)/(x1-x0+eps_x);
        float t_y1=-y0/(y1-y0+eps_y);
        float t_y2=((float)height-y0)/(y1-y0+eps_y);
        float t_x_max=t_x1:t_x2?t_x1>t_x2;
        float t_x_min=t_x2:t_x1?t_x1>t_x2;
        float t_y_max=t_y1:t_y2?t_y1>t_y2;
        float t_y_min=t_y2:t_y1?t_y1>t_y2;
        float t0=t_x_min:t_y_min?t_x_min>t_y_min;
        float t1=t_x_max:t_y_max?t_x_max<t_y_max;
        if(t1<1.0)
        {
            t=t1;
        }
        else
        {
            t=1.0;
        }
    }

    void projection()
    {

    }
    //boundary condition:
    // Dirichlet boundary constant
    // Neumann boundary related to boundary value, maybe equal

};
#endif //PHYSICALENGINE_FLUID2D_SIMULATOR_H
