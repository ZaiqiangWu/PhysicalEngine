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

#include "ft2build.h"
#include FT_FREETYPE_H

#include "shader.h"


/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};


class Text
{
public:
    Text()
    {
        shader=Shader("../shader/text/shader.vs", "../shader/text/shader.fs");
        if (FT_Init_FreeType(&ft))
        {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        }

        // find path to font
        std::string font_name = FileSystem::getPath("resources/fonts/Arial.ttf");
        if (font_name.empty())
        {
            std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
        }
        if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
            return -1;
        }
        else {
            // set size to load glyphs as
            FT_Set_Pixel_Sizes(face, 0, 48);

            // disable byte-alignment restriction
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            // load first 128 characters of ASCII set
            for (unsigned char c = 0; c < 128; c++)
            {
                // Load character glyph
                if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                {
                    std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                    continue;
                }
                // generate texture
                unsigned int texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(
                        GL_TEXTURE_2D,
                        0,
                        GL_RED,
                        face->glyph->bitmap.width,
                        face->glyph->bitmap.rows,
                        0,
                        GL_RED,
                        GL_UNSIGNED_BYTE,
                        face->glyph->bitmap.buffer
                );
                // set texture options
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                // now store character for later use
                Character character = {
                        texture,
                        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                        static_cast<unsigned int>(face->glyph->advance.x)
                };
                Characters.insert(std::pair<char, Character>(c, character));
            }
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        // destroy FreeType once we're finished
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }
    ~Text()
    {

    }
    void RenderText(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color)
    {

    }

private:
    std::map<GLchar, Character> Characters;
    unsigned int VAO, VBO;
    Shader shader;
    FT_Library ft;
    FT_Face face;
};

#endif //PHYSICALENGINE_TEXT_H
