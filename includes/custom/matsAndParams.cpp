#include "../../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include "../../includes/glm/glm.hpp"
#include <iostream>
#include "../../includes/glm/gtc/matrix_transform.hpp"
// #define STB_IMAGE_IMPLEMENTATION
#include "../../includes/stb/stb_image.h"

#include <vector>
#include <string>
#include <math.h>
#include <functional>
#include "matsAndParams.hpp"
#include "Vobj.hpp"

glm::mat4 sphere(float u, float v)
{
    glm::mat4 I = glm::mat4(1.0f);
    float PI = M_PI;
    I = glm::rotate(glm::mat4(1.0f), (float)(1.0 * PI * u), glm::vec3(1, 0, 0)) * I;
    I = glm::rotate(glm::mat4(1.0f), (float)(2.0 * PI * v), glm::vec3(0, 1, 0)) * I;
    return I;
}

glm::mat4 donut(float u, float v)
{
    glm::mat4 I = glm::mat4(1.0f);
    float PI = M_PI;
    I = rotateAbout(glm::mat4(1.0f), (float)(2.0 * PI * u), glm::vec3(1, 0, 0), glm::vec4(0,2,0,1)) * I;
    I = glm::rotate(glm::mat4(1.0f), (float)(2.0 * PI * v), glm::vec3(0, 0, 1)) * I;
    return I;
}

glm::mat4 cylinderF(float u, float v)
{
    glm::mat4 I = glm::mat4(1.0f);
    float PI = M_PI;
    I = glm::rotate(glm::mat4(1.0f), (float)(2.0 * PI * u), glm::vec3(0, 1, 0)) * I;
    I = glm::translate(glm::mat4(1.0f),glm::vec3(0,v,0)) * I;
    return I;
}

glm::mat4 disk(float u, float v)
{
    glm::mat4 I = glm::mat4(1.0f);
    float PI = M_PI;
    I = glm::translate(glm::mat4(1.0f),glm::vec3(u,0,0)) * I;
    I = glm::rotate(glm::mat4(1.0f), (float)(2.0 * PI * v), glm::vec3(0, 1, 0)) * I;
    
    return I;
}


