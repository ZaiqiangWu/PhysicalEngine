//
// Created by Zaiqiang Wu on 2022/07/31.
//

#ifndef PHYSICALENGINE_RAY_H
#define PHYSICALENGINE_RAY_H
#include "mathlib/vector2.h"
#include "mathlib/vector3.h"

template <class T>
class Ray2D
{
public:
    vector2<T> o;
    vector2<T> d;
    Ray2D(vector2<T> o, vector2<T> d)
    {
        this->o=o;
        this->d=d;
    }
};
#endif //PHYSICALENGINE_RAY_H
