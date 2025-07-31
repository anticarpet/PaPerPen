#ifndef shader_h
#define shader_h

#include "../../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include "../../includes/glm/glm.hpp"

#include "../../includes/glm/gtc/matrix_transform.hpp"
// #define STB_IMAGE_IMPLEMENTATION
// #include "../../includes/stb/stb_image.h"

#include <iostream>
#include <vector>
#include <functional>
#include <string>

class shader{
    unsigned int Vshader;
    unsigned int Fshader;
    public:
    unsigned int Program;

    //funcs
    public:
    shader();
    void makeVshader(char* VshSource);
    void makeFshader(char* FshSource);
    void makeProgram();
    

};


#endif