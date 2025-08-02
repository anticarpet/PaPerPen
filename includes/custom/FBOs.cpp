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
#include "FBOs.hpp"

FBO::FBO(){
    std::vector<Vobj*> queue;
    std::vector<int> texts;
    unsigned int fbo;
    unsigned int tex;
    unsigned int rbo;
    glm::mat4 MVPat = glm::mat4(1.0f);

   

}
//you might want to visit this for errors later

void FBO::createFBO(){
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

}

void FBO::bindTex(unsigned int &texture, int width, int height){

    tex = texture;
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // const uint8_t FtexData = NULL;
   
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
    



}

void FBO::use(){
    glBindFramebuffer(GL_FRAMEBUFFER,fbo);
}

void FBO::addQueue(Vobj *Vob, int texture){
    queue.push_back(Vob);
    texts.push_back(texture);
}

void FBO::render(){
    glBindFramebuffer(GL_FRAMEBUFFER,fbo);
    //glBindTexture(GL_TEXTURE_2D, tex);

    for (int i = 0; i < queue.size(); i++)
    {
        //glBindFramebuffer(GL_FRAMEBUFFER,fbo);
        (*queue[i]).setMVPat(MVPat);
        (*queue[i]).setTexture(texts[i]);
        (*queue[i]).quickDraw();
    }
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    //glBindTexture(GL_TEXTURE_2D, 0);
    
}

void FBO::clearTex(float R, float G, float B, float A){

    glClearColor(R / 255, G / 255, B / 255, A);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);


}

void FBO::setMVPat(glm::mat4 MVPMat){
    MVPat = MVPMat;
}

void FBO::makeMVPat(glm::vec3 pos, glm::vec3 dir, glm::mat4 proj){
    glm::mat4 CameraMatrix = glm::lookAt(
        pos, // the position of your camera, in world space
        dir,                    // where you want to look at, in world space
        glm::vec3(0, 1, 0)                 // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
    );

    MVPat = proj * CameraMatrix * glm::mat4(1.0f);

}

void SCREEN::render(){
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    //glBindTexture(GL_TEXTURE_2D, tex);

    for (int i = 0; i < queue.size(); i++)
    {
        //glBindFramebuffer(GL_FRAMEBUFFER,fbo);
        (*queue[i]).setMVPat(MVPat);
        (*queue[i]).setTexture(texts[i]);
        (*queue[i]).quickDraw();
    }
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    //glBindTexture(GL_TEXTURE_2D, 0);

}
