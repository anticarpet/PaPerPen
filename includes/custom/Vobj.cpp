
#include "../../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include "../../includes/glm/glm.hpp"
#include <iostream>
#include "../../includes/glm/gtc/matrix_transform.hpp"
// #define STB_IMAGE_IMPLEMENTATION
#include "../../includes/stb/stb_image.h"

#include <vector>
#include <string>
#include <functional>
#include "Vobj.hpp"

Vobj::Vobj(int x)
{
    int ID = x;
    int bCount = 0;

    int texPosInProg;
    int matPosInProg;
    int MVPatPosInProg;

    unsigned int VAO;
    std::vector<int> sizes;
    glGenVertexArrays(1, &VAO);
    // glBindVertexArray(VAO);
    std::vector<unsigned int> VBOs;
    std::vector<std::vector<float>> VBOdata;
    unsigned int program;

    // matrices
    glm::mat4 MVP = glm::mat4(1.0f);
    glm::mat4 clipSpaceMat = glm::mat4(1.0f);
}
/*
for reference:

layout location in shader
index inside array      -------------> are all the same integer
the ID of createBuffer

*/

void Vobj::createBuffer(int layout)
{
    VBOs.push_back(1);
    VBOdata.push_back({});
    sizes.push_back(1);
    glGenBuffers(1, &VBOs[layout]);
}

void Vobj::createVAO()
{
    glGenVertexArrays(1, &VAO);
}

void Vobj::setObjectData(int layout, std::vector<float> data, int size)
{
    VBOdata[layout] = data;
    sizes[layout] = size;
}

void Vobj::addObjectData(int layout, std::vector<float> data, int size)
{
    for (int i = 0; i < data.size(); i++)
    {
        VBOdata[layout].push_back(data[i]);
    }
}

void Vobj::bindVAO()
{
    glBindVertexArray(VAO);
}

void Vobj::bindBuffer(int layout)
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[layout]);
}

void Vobj::bufferData(int layout)
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[layout]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * VBOdata[layout].size(), &VBOdata[layout][0], GL_STATIC_DRAW);
}

void Vobj::attribPointer(int layout)
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[layout]);
    glVertexAttribPointer(layout, sizes[layout], GL_FLOAT, GL_FALSE, 0, (void *)0);
}

void Vobj::enableBuffer(int layout)
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[layout]);
    glEnableVertexAttribArray(layout);
}

void Vobj::drawVAO()
{
    glBindVertexArray(VAO);
    glUseProgram(program);

    glDrawArrays(GL_TRIANGLES, 0, VBOdata[0].size() / 3);
}

void Vobj::quickDraw()
{
    glBindVertexArray(VAO);
    for (int i = 0; i < VBOs.size(); i++)
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
        bufferData(i);
        attribPointer(i);
        enableBuffer(i);
    }
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(texPosInProg, 0);
    glUniformMatrix4fv(matPosInProg, 1, GL_FALSE, &Mat[0][0]);
    glUniformMatrix4fv(MVPatPosInProg, 1, GL_FALSE, &MVPat[0][0]);
    drawVAO();
}

// for shaders

void Vobj::assignProgram(unsigned int P)
{
    program = P;
}

void Vobj::setTexture(unsigned int tex)
{
    texture = tex;
}

void Vobj::transMat(glm::mat4 matrix)
{
    Mat = matrix * Mat;
    // this shit is why I have a B in algebra.....
}

void Vobj::setMat(glm::mat4 matrix)
{
    Mat = matrix;
}

void Vobj::setMVPat(glm::mat4 matrix)
{
    MVPat = matrix;
}

// getting the uniform positions in shader for the main matrix and the texture

void Vobj::setTexProgPos(int pos)
{
    texPosInProg = pos;
}

void Vobj::setMatProgPos(int pos)
{
    matPosInProg = pos;
}

void Vobj::setMVPatProgPos(int pos)
{
    MVPatPosInProg = pos;
}

void Vobj::assignProgramAndPos(unsigned int P){
    program = P;
    texPosInProg = glGetUniformLocation(program, "uTexture");
    matPosInProg = glGetUniformLocation(program, "rix");
    MVPatPosInProg = glGetUniformLocation(program, "MVP");



}

unsigned int makeVShader(const char *VShaderSource)
{
    int success;
    char infoLog[512];
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &VShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    return vertexShader;
}

unsigned int makeFShader(const char *FShaderSource)
{
    int success;
    char infoLog[512];
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &FShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    return fragmentShader;
}

unsigned int makeProgram(unsigned int V, unsigned int F)
{
    int success;
    char infoLog[512];
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, V);
    glAttachShader(shaderProgram, F);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
    return shaderProgram;
}

glm::mat4 rotateAbout(glm::mat4 mat, float ang, glm::vec3 vec, glm::vec4 poi)
{
    mat *= glm::translate(glm::mat4(1.0f), glm::vec3(1.0f * poi.x, 1.0f * poi.y, 1.0f * poi.z));
    mat *= glm::rotate(glm::mat4(1.0f), ang, vec);
    mat *= glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f * poi.x, -1.0f * poi.y, -1.0f * poi.z));
    return mat;
}

void adham(GLFWwindow *window, double &xPos, double &yPos, glm::vec4 &cPos, glm::vec4 &dir)
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    float speed = 0.4f;
    int state = glfwGetKey(window, GLFW_KEY_E);
    int w = glfwGetKey(window, GLFW_KEY_W);
    int a = glfwGetKey(window, GLFW_KEY_A);
    int s = glfwGetKey(window, GLFW_KEY_S);
    int d = glfwGetKey(window, GLFW_KEY_D);
    int shift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
    int space = glfwGetKey(window, GLFW_KEY_SPACE);
    glm::mat4 cameraRotation = glm::mat4(1.0f);
    glm::vec3 perp = glm::normalize(glm::cross(glm::vec3(cPos.x, cPos.y, cPos.z) - glm::vec3(dir.x, dir.y, dir.z), glm::vec3(0, 1, 0)));
    glm::vec3 para = 1.0f * glm::normalize(glm::cross(glm::vec3(perp.x, perp.y, perp.z), glm::vec3(0, 1, 0)));
    if (state != GLFW_PRESS)
    {
        // perp = glm::normalize(glm::cross(glm::vec3(dir.x, dir.y, dir.z), glm::vec3(0, 1, 0)));
        glfwGetCursorPos(window, &xPos, &yPos);
        glfwSetCursorPos(window, 400, 300);
        double dx = *&xPos - 400;
        double dy = *&yPos - 300;
        // cameraRotation = rotateAbout(glm::mat4(1.0f), 0.01f * dy, *&perp, cPos);
        cameraRotation = rotateAbout(glm::mat4(1.0f), -0.01f * dx, glm::vec3(0, 1, 0), cPos);
        dir = cameraRotation * dir;
        glm::vec3 perp = glm::normalize(glm::cross(glm::vec3(cPos.x, cPos.y, cPos.z) - glm::vec3(dir.x, dir.y, dir.z), glm::vec3(0, 1, 0)));
        perp = glm::vec3(perp.x, perp.y, perp.z);
        cameraRotation = rotateAbout(glm::mat4(1.0f), 0.01f * dy, *&perp, cPos);
        dir = cameraRotation * dir;
        // std::cout << "x: "<< dx << "\n" << "y: " << dy << "\n" << "\n";
    }
    if (w == GLFW_PRESS)
    {
        dir = glm::translate(glm::mat4(1.0f), 0.1f * speed * para) * dir;
        cPos = glm::translate(glm::mat4(1.0f), 0.1f * speed * para) * cPos;
    }
    if (a == GLFW_PRESS)
    {
        dir = glm::translate(glm::mat4(1.0f), 0.1f * speed * perp) * dir;
        cPos = glm::translate(glm::mat4(1.0f), 0.1f * speed * perp) * cPos;
    }
    if (s == GLFW_PRESS)
    {
        dir = glm::translate(glm::mat4(1.0f), -0.1f * speed * para) * dir;
        cPos = glm::translate(glm::mat4(1.0f), -0.1f * speed * para) * cPos;
    }
    if (d == GLFW_PRESS)
    {
        dir = glm::translate(glm::mat4(1.0f), -0.1f * speed * perp) * dir;
        cPos = glm::translate(glm::mat4(1.0f), -0.1f * speed * perp) * cPos;
    }
    if (shift == GLFW_PRESS)
    {
        dir = glm::translate(glm::mat4(1.0f), -0.1f * speed * glm::vec3(0, 1, 0)) * dir;
        cPos = glm::translate(glm::mat4(1.0f), -0.1f * speed * glm::vec3(0, 1, 0)) * cPos;
    }
    if (space == GLFW_PRESS)
    {
        dir = glm::translate(glm::mat4(1.0f), 0.1f * speed * glm::vec3(0, 1, 0)) * dir;
        cPos = glm::translate(glm::mat4(1.0f), 0.1f * speed * glm::vec3(0, 1, 0)) * cPos;
    }
}

void makeImgTexture(const char *image)
{

    int textureWidth, textureHeight, textureLayers;
    uint8_t *pixelData = stbi_load(image, &textureWidth, &textureHeight, &textureLayers, 0);
    // std::cout << pixelData[1000];

    // stbi_image_free(pixelData);

    // parameters
    bool pixelated = 1;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, pixelated ? GL_NEAREST : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, pixelated ? GL_NEAREST : GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    // make texture
    // texture slots
    // glUseProgram(shaderProgram);

    uint32_t MipLevel = 0;
    uint32_t internalFormat = GL_RGBA32F;
    uint32_t width = textureWidth;
    uint32_t height = textureHeight;

    uint32_t border = 0;
    uint32_t format = GL_RGBA;
    uint32_t type = GL_UNSIGNED_BYTE;
    // insert pixel data
    glTexImage2D(GL_TEXTURE_2D, MipLevel, internalFormat, width, height, border, format, type, pixelData);
    glGenerateMipmap(GL_TEXTURE_2D);

    // stbi_image_free(pixelData);
}

void makeTexture(const uint8_t *data, uint8_t width, uint8_t height)
{

    // stbi_image_free(pixelData);

    // parameters
    bool pixelated = 1; 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, pixelated ? GL_NEAREST : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, pixelated ? GL_NEAREST : GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // make texture
    // texture slots
    // glUseProgram(shaderProgram);

    uint32_t MipLevel = 0;
    uint32_t internalFormat = GL_RGBA;
    uint32_t border = 0;
    uint32_t format = GL_RGBA;
    uint32_t type = GL_UNSIGNED_BYTE;
    // insert pixel data
    glTexImage2D(GL_TEXTURE_2D, MipLevel, internalFormat, width, height, border, format, type, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // stbi_image_free(pixelData);
}

void makeEmptyTexture(int width, int height){
   
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
void activeTex(int slot, int ID)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, ID);
}

// bismillah: mesh
//  mesh() takes in a function and returns the array(s) of Vpos, texPos, and normals
//  matmap() takes in u and v, is a custom defined function and it returns a matrix transformation
//  based on u and v, so that the for loop is inside mesh() defining vec3s from that function

std::vector<float> mesh(std::function<glm::mat4(float u, float v)> matmap, glm::vec4 naught, int Res)
{
    std::vector<float> array;
    // int count;
    std::vector<std::vector<glm::vec4>> UVGrid;
    std::vector<glm::vec4> subVector;
    glm::vec4 pos0 = naught;
    glm::vec4 calcPos;
    // 1- make the UVgrid of vec4s
    for (float u = 0.0; u < Res + 1; u++) // makes res + 1 iterations
    {
        subVector = {};
        for (float v = 0.0; v < Res + 1; v++) // makes res + 1 iterations
        {

            calcPos = matmap(u / Res, v / Res) * pos0;
            subVector.push_back(calcPos);
        };
        UVGrid.push_back(subVector);
        // return array;
    };
    // 2- make triangles out of the UVGrid
    for (int u = 0; u < Res; u++) // makes res iterations
    {

        for (int v = 0; v < Res; v++) // makes res iterations
        {
            // this is why a tesselation and geometry shader are needed
            array.push_back(UVGrid[u][v].x);
            array.push_back(UVGrid[u][v].y);
            array.push_back(UVGrid[u][v].z);

            array.push_back(UVGrid[u + 1][v].x);
            array.push_back(UVGrid[u + 1][v].y);
            array.push_back(UVGrid[u + 1][v].z);

            array.push_back(UVGrid[u][v + 1].x);
            array.push_back(UVGrid[u][v + 1].y);
            array.push_back(UVGrid[u][v + 1].z);

            array.push_back(UVGrid[u + 1][v + 1].x);
            array.push_back(UVGrid[u + 1][v + 1].y);
            array.push_back(UVGrid[u + 1][v + 1].z);

            array.push_back(UVGrid[u + 1][v].x);
            array.push_back(UVGrid[u + 1][v].y);
            array.push_back(UVGrid[u + 1][v].z);

            array.push_back(UVGrid[u][v + 1].x);
            array.push_back(UVGrid[u][v + 1].y);
            array.push_back(UVGrid[u][v + 1].z);
        };

        // return array;
    };
    // UVGrid.clear();
    // subVector.clear();
    return array;

    // return subVector;
}

std::vector<float> meshTexMap(int Res)
{
    std::vector<float> array;
    for (float u = 0.0; u < Res; u++) // makes res iterations
    {

        for (float v = 0.0; v < Res; v++) // makes res iterations
        {
            // this is why a tesselation and geometry shader are needed
            array.push_back((u) / Res);
            array.push_back((v) / Res);

            array.push_back((u + 1.0f) / Res);
            array.push_back((v) / Res);

            array.push_back((u) / Res);
            array.push_back((v + 1.0f) / Res);

            array.push_back((u + 1.0f) / Res);
            array.push_back((v + 1.0f) / Res);

            array.push_back((u + 1.0f) / Res);
            array.push_back((v) / Res);

            array.push_back((u) / Res);
            array.push_back((v + 1.0f) / Res);
        };

        // return array;
    };
    return array;
}

void Vobj::useTemplate(std::function<glm::mat4(float u, float v)> matmap, glm::vec4 Po, glm::vec4 No, int Res)
{

    createVAO();
    createBuffer(0);
    std::vector<float> spher = mesh(matmap, Po, Res);
    setObjectData(0, spher, 3);

    createBuffer(1);
    std::vector<float> sphernorm = mesh(matmap, No, Res);
    setObjectData(1, sphernorm, 3);

    createBuffer(2);
    setObjectData(2, meshTexMap(Res), 2);
}

void Vobj::useCubeTemplate(glm::vec3 dims){

    float x = dims.x; float y = dims.y; float z = dims.z;

    createVAO();
    createBuffer(0);
    std::vector<float> cube = {
        //+x
        x,y,z,
        x,y,-1*z,
        x,-1*y,z,
        x,-1*y,-1*z,
        x,y,-1*z,
        x,-1*y,z,

        //-x
        -1*x,y,z,
        -1*x,y,-1*z,
        -1*x,-1*y,z,
        -1*x,-1*y,-1*z,
        -1*x,y,-1*z,
        -1*x,-1*y,z,

        //+y
        x,y,z,
        x,y,-1*z,
        -1*x,y,z,
        -1*x,y,-1*z,
        x,y,-1*z,
        -1*x,y,z,

        //-y
        x,-1*y,z,
        x,-1*y,-1*z,
        -1*x,-1*y,z,
        -1*x,-1*y,-1*z,
        x,-1*y,-1*z,
        -1*x,-1*y,z,

        //+z
        x,y,z,
        x,-1*y,z,
        -1*x,y,z,
        -1*x,-1*y,z,
        x,-1*y,z,
        -1*x,y,z,

        //-z
        x,y,-1*z,
        x,-1*y,-1*z,
        -1*x,y,-1*z,
        -1*x,-1*y,-1*z,
        x,-1*y,-1*z,
        -1*x,y,-1*z,

       


    };
    setObjectData(0, cube, 3);

    createBuffer(1);
    std::vector<float> cubeNorm = {
        1,0,0,
        1,0,0,
        1,0,0,
        1,0,0,
        1,0,0,
        1,0,0,

        -1,0,0,
        -1,0,0,
        -1,0,0,
        -1,0,0,
        -1,0,0,
        -1,0,0,

        0,1,0,
        0,1,0,
        0,1,0,
        0,1,0,
        0,1,0,
        0,1,0,

        0,-1,0,
        0,-1,0,
        0,-1,0,
        0,-1,0,
        0,-1,0,
        0,-1,0,

        0,0,1,
        0,0,1,
        0,0,1,
        0,0,1,
        0,0,1,
        0,0,1,

        0,0,-1,
        0,0,-1,
        0,0,-1,
        0,0,-1,
        0,0,-1,
        0,0,-1,


    };
    setObjectData(1, cubeNorm, 3);

    createBuffer(2);

    std::vector<float> texMap = {
        1,1,
        0,1,
        1,0,
        0,0,
        1,0,
        0,1,

         1,1,
        0,1,
        1,0,
        0,0,
        1,0,
        0,1,

         1,1,
        0,1,
        1,0,
        0,0,
        1,0,
        0,1,

         1,1,
        0,1,
        1,0,
        0,0,
        1,0,
        0,1,

         1,1,
        0,1,
        1,0,
        0,0,
        1,0,
        0,1,

         1,1,
        0,1,
        1,0,
        0,0,
        1,0,
        0,1,
    };
    setObjectData(2, texMap, 2);

}

void Vobj::addTemplate(std::function<glm::mat4(float u, float v)> matmap, glm::vec4 Po, glm::vec4 No, int Res){

    std::vector<float> spher = mesh(matmap, Po, Res);
    addObjectData(0, spher, 3);

    createBuffer(1);
    std::vector<float> sphernorm = mesh(matmap, No, Res);
    addObjectData(1, sphernorm, 3);

    createBuffer(2);
    addObjectData(2, meshTexMap(Res), 2);


}

//doing the points themselves

std::vector<float> transList4(std::vector<float> list, glm::mat4 mat){
    glm::vec4 vectTemp ;
    std::vector<float> newList;
    for (int i = 0; i < list.size()/3; i++)
    {
        vectTemp = glm::vec4(list[3*i],list[3*i+1],list[3*i+2],1.0f);
        vectTemp = mat * vectTemp;
        newList.push_back(vectTemp.x);
        newList.push_back(vectTemp.y);
        newList.push_back(vectTemp.z);
        //newList.push_back(vectTemp.w);

    }
    return newList;

}

void Vobj::transBuffer(int layout, glm::mat4 mat){
    VBOdata[layout] = transList4(VBOdata[layout],mat);
}