//
// Created by Zaiqiang Wu on 2022/07/31.
//


#ifndef PHYSICALENGINE_AABB_H
#define PHYSICALENGINE_AABB_H
#include "mathlib/vector2.h"
#include "graphics/ray.h"

template <class T>
class AABB_2D
{
public:
    AABB_2D(T x_min, T x_max, T y_min, T y_max)
    {
        this->x_min=x_min;
        this->x_max=x_max;
        this->y_min=y_min;
        this->y_max=y_max;
    }
    ~AABB_2D()
    {
        ;
    }
    bool clipping(Ray2D<T> ray,T& t0, T& t1)
    {
        bool isHit;
        T x0=ray.o[0];
        T x1=ray.d[0];
        T y0=ray.o[1];
        T y1=ray.d[1];
        T eps = std::numeric_limits<T>::epsilon();
        T eps_x =eps:(-eps)?(x1-x0)>0;
        T eps_y =eps:(-eps)?(y1-y0)>0;
        T t_x1=(x_min-x0)/(x1-x0+eps_x);
        T t_x2=(x_max-x0)/(x1-x0+eps_x);
        T t_y1=(y_min-y0)/(y1-y0+eps_y);
        T t_y2=(y_max-y0)/(y1-y0+eps_y);
        T t_x_max=t_x1:t_x2?t_x1>t_x2;
        T t_x_min=t_x2:t_x1?t_x1>t_x2;
        T t_y_max=t_y1:t_y2?t_y1>t_y2;
        T t_y_min=t_y2:t_y1?t_y1>t_y2;
        t0=t_x_min:t_y_min?t_x_min>t_y_min;
        t1=t_x_max:t_y_max?t_x_max<t_y_max;
        isHit=t0<t1;
        return isHit;
    }
private:
    T x_min;
    T x_max;
    T y_min;
    T y_max;

};
#endif //PHYSICALENGINE_AABB_H
