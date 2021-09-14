//
// Created by wuzaiqiang on 21-9-14.
//

#ifndef PHYSICALENGINE_TEXT_H
#define PHYSICALENGINE_TEXT_H
#include <iostream>
#include <map>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <shader.h>

class Text: public Object
{
public:
    Text()
    {

    }
    ~Text()
    {

    }
private:

};

#endif //PHYSICALENGINE_TEXT_H
