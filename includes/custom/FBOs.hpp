#ifndef FBOs_h
#define FBOs_h

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
#include "Vobj.hpp"

class FBO{
    public:
    unsigned int tex;
    unsigned int rbo;
    unsigned int fbo;
    std::vector<Vobj*> queue;
    std::vector<int> texts;
    glm::mat4 MVPat;

    FBO();
    void createFBO(); //makes the fbo
    void bindTex(unsigned int&, int, int); //bind texture to render to
    void use();// bind that framebuffer, it should be useless
    public:
    void render();// render whats in its queue into the texture
    public:
    void addQueue(Vobj *, int);// add a Vobj ADDRESS into its queue
    void clearTex(float, float, float, float); //clears the texture for rendering

    void setMVPat(glm::mat4);
    void makeMVPat(glm::vec3, glm::vec3, glm::mat4);









};

class SCREEN : FBO{
    public:
    void render();
};

#endif