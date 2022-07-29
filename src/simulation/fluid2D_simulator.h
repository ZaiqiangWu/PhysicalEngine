//
// Created by Zaiqiang Wu on 2022/07/29.
//

#ifndef PHYSICALENGINE_FLUID2D_SIMULATOR_H
#define PHYSICALENGINE_FLUID2D_SIMULATOR_H

class Fluid2D
{
public:
    int height;
    int width;
    float* color;
    Fluid2D(int h, int w)
    {
        height=h;
        width=w;
        color=new float[h*w*3];
    }
    ~Fluid2D()
    {
        delete [] color;
    }
private:

};
#endif //PHYSICALENGINE_FLUID2D_SIMULATOR_H
