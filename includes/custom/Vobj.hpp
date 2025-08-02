
#ifndef Vobj_h
#define Vobj_h

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
class Vobj
{
public:
    int ID;
    unsigned int texture;

    int texPosInProg;
    int matPosInProg;
    int MVPatPosInProg;



    std::vector<int> sizes;
    std::vector<unsigned int> VBOs;
    std::vector<std::vector<float>> VBOdata;
    int bCount;
    unsigned int program;
    glm::mat4 Mat;
    glm::mat4 MVPat;


public:
    unsigned int VAO;
    // float Verte
public:
    // constructor
    Vobj(int);
    // creates buffer
    void createBuffer(int);
    // creates VAO
    void createVAO();
    // sets the buffer's data inside the Vobj
    void setObjectData(int, std::vector<float>, int);
    void addObjectData(int, std::vector<float>, int);
    // binds the buffer
    void bindBuffer(int);
    // binds the VAO
    void bindVAO();
    // sets buffer data
    void bufferData(int);
    // sends the buffer data to the given layout locations
    void attribPointer(int);
    // enables said VAO
    void enableBuffer(int);
    // draw the VAO of Vobj,finally
    void drawVAO();
    // combine some of the above functions quickly to replace the other objects after being rendered
    void quickDraw();

    // makes Vshader, Fshader and program from sources without all the clutter
    void assignProgram(unsigned int P);

    // set object texture
    void setTexture(unsigned int);

    // transform the matrix
    void transMat(glm::mat4);
    void setMat(glm::mat4);
    void setMVPat(glm::mat4);

    //transform the points themselves
    void transBuffer(int, glm::mat4);

    // set the uniform positions for quickDraw
    void setTexProgPos(int);
    void setMatProgPos(int);
    void setMVPatProgPos(int);
    void assignProgramAndPos(unsigned int);

    //directly make an object out of a template mo5tasar
    void useTemplate( std::function<glm::mat4(float u, float v)> matmap ,
    glm::vec4,glm::vec4, int );
    void useCubeTemplate (glm::vec3);
    void addTemplate( std::function<glm::mat4(float u, float v)> matmap ,
    glm::vec4,glm::vec4, int );
    
};


unsigned int makeVShader(const char *);
unsigned int makeFShader(const char *);
unsigned int makeProgram(unsigned int, unsigned int);

//tranform the points in a float array
std::vector<float> transList4(std::vector<float>, glm::mat4);

void adham(GLFWwindow *, double &, double &, glm::vec4 &, glm::vec4 &);
glm::mat4 rotateAbout(glm::mat4, float, glm::vec3, glm::vec4);

// bismillah: textures
void makeImgTexture(const char *);
void makeTexture(const uint8_t *, uint8_t, uint8_t);
void makeEmptyTexture(int, int);
void quickTexture( unsigned int &tex, const uint8_t *data, uint8_t width, uint8_t height);
void quickTexture( unsigned int &tex, const char* image, uint8_t width, uint8_t height);
void quickTexture( unsigned int &tex, int width, int height);

void activeTex(int, int);

//    mesh. <---------------------
std::vector<float> mesh(std::function<glm::mat4(float u, float v)> matmap,glm::vec4, int);
std::vector<float> meshTexMap(int);
#endif

// textures misc
std::vector<uint8_t> checkerboard(int patnum, glm::vec4 C1, glm::vec4 C2);

// for array pointer declaration, type = Type
// arr_name[size];
// type *ptr_name = &arr_name;

// Important: the pointer points at the first member of the array,
// for other elems, increment the pointer address by one or so.
// example: cout << *ptr_name; cout<< *(ptr_name + 1);

// for dynamic arrays: use vectors
/*
vector<string> cars = {"mercedes","volvo"};
cars[0] = "mercedes";
cars[1] = "volvo";
cars.pushback("tesla"); <--- adds an element
cars.popback(); <--- deletes an element
cars.size(); <--- gets the size of the vector


//functions as parameters<--------------------------------------------------
#include <bits/stdc++.h>
using namespace std;

int add(int x, int y) { return x + y; }
int mul(int x, int y) { return x * y; }

// Function that takes function as arguemnt
int invoke(int x, int y, function<int(int, int)> f) {
    return f(x, y);
}


int main() {

    // Pass required function as parameter
      // using its name
    cout << "Addition: ";
    cout << invoke(20, 10, &add) << '\n';

    cout << "Multiplication: ";
    cout << invoke(20, 10, &mul) << '\n';

    return 0;
}

// overloading the operators for custom types ( + - / * ) aka matrices

returnType operator symbol (arguments) {
    ... .. ...
}


*/