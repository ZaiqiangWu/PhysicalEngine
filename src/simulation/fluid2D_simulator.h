//
// Created by Zaiqiang Wu on 2022/07/29.
//

#ifndef PHYSICALENGINE_FLUID2D_SIMULATOR_H
#define PHYSICALENGINE_FLUID2D_SIMULATOR_H
#include "mathlib/vector3.h"
#include "mathlib/vector2.h"
#include "graphics/AABB.h"
#include <cmath>

using AABB=AABB_2D<float>;
using Ray=Ray2D<float>;

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
    float* get_color_ptr()
    {
        return color[0].ptr();
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
                Vector2 old_p=back_trace(p,v);
                Vector3 old_color= get_interpolated_values(old_p);
                color[h*width+w]=old_color;
            }
        }

    }
    Vector3 get_interpolated_values(Vector2 p)//for value at grid center
    {
        p[0]=p[0]-0.5;
        p[1]=p[1]-0.5;
        float x_min=std::floor(p[0]);
        float x_max=x_min+1;
        float y_min=std::floor(p[1]);
        float y_max=y_min+1;
        Vector2 p0=Vector2(x_min,y_max);
        Vector2 p1=Vector2(x_max,y_max);
        Vector2 p2=Vector2(x_max,y_min);
        Vector2 p3=Vector2(x_min,y_min);
        if (x_min<0) x_min=0;
        if (y_min<0) y_min=0;
        Vector3 color0=color[(int)y_max*width+(int)x_min];
        Vector3 color1=color[(int)y_max*width+(int)x_max];
        Vector3 color2=color[(int)y_min*width+(int)x_max];
        Vector3 color3=color[(int)y_min*width+(int)x_min];
        float c0,c1,c2,c3;
        bilinear_interpolation(p,p0,p1,p2,p3,c0,c1,c2,c3);
        return color0*c0+color1*c1+color2*c2+color3*c3;
    }

    void bilinear_interpolation(Vector2 p,
                                Vector2 p0,
                                Vector2 p1,
                                Vector2 p2,
                                Vector2 p3,
                                float &c0,
                                float& c1,
                                float& c2,
                                float& c3)
    {
        Vector2 p03=(p0-p3).abs();
        float inv_S=1.0/(p03[0]*p03[1]);
        Vector2 p00=(p-p0).abs();
        Vector2 p11=(p-p1).abs();
        Vector2 p22=(p-p2).abs();
        Vector2 p33=(p-p3).abs();
        c0=p22[0]*p22[1]*inv_S;
        c1=p33[0]*p33[1]*inv_S;
        c2=p00[0]*p00[1]*inv_S;
        c3=p11[0]*p11[1]*inv_S;
    }
    Vector2 back_trace(Vector2 p, Vector2 v)
    {
        Vector2 old_p=p-v*delta_t;
        Ray ray(p,old_p);
        float t0,t1;
        AABB aabb(0,width,0,height);
        aabb.clipping(ray,t0,t1);
        if(t1<1)
            old_p=p*(1-t1)+old_p*t1;
        return old_p;
    }


    void projection()
    {

    }
    //boundary condition:
    // Dirichlet boundary constant
    // Neumann boundary related to boundary value, maybe equal

};
#endif //PHYSICALENGINE_FLUID2D_SIMULATOR_H
