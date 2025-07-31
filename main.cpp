#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "includes/glm/glm.hpp"
#include <iostream>
#include "includes/glm/gtc/matrix_transform.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "includes/stb/stb_image.h"
#include "includes/custom/Vobj.hpp"
#include "includes/custom/FBOs.hpp"
#include "includes/custom/shaderSources.hpp"
#include "includes/custom/matsAndParams.hpp"
#include <array>
#include <vector>
#include <string>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()

{

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SElmanakhly", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    // vertex shader

    // link shaders
    int success;
    char infoLog[512];
    unsigned int vertexShader = makeVShader(vertexShaderSource);
    unsigned int fragmentShader = makeFShader(fragmentShaderSource);
    unsigned int shaderProgram = makeProgram(vertexShader, fragmentShader);

    unsigned int Vsh2 = makeVShader(Vshs2);
    unsigned int Fsh2 = makeFShader(Fshs2);
    unsigned int SP2 = makeProgram(Vsh2, Fsh2);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // glDepthMask(GL_TRUE);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float time = 0;
    int essam = glGetUniformLocation(shaderProgram, "essam");
    int rixPos = glGetUniformLocation(shaderProgram, "rix");
    int MVPos = glGetUniformLocation(shaderProgram, "MVP");
    int lightpos = glGetUniformLocation(shaderProgram, "light");
    glm::vec4 light = glm::vec4(6, 1, 3, 1);

    unsigned int VBO, VAO;
    Vobj first = Vobj(1);
    first.createVAO();
    std::vector<float> verts = {
        50,
        0,
        50,
        50,
        0,
        -50,
        -50,
        0,
        50,
        -50,
        0,
        -50,
        50,
        0,
        -50,
        -50,
        0,
        50,
    };

    std::vector<float> vertsn = {
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
        0, 1, 0

    };
    first.createBuffer(0);
    first.setObjectData(0, verts, 3);

    first.createBuffer(1);
    first.setObjectData(1, vertsn, 3);

    first.createBuffer(2);
    first.setObjectData(2, meshTexMap(1), 2);

    first.assignProgram(shaderProgram);

    // TEXTURES
    // pixeldata

    unsigned int pyr, sand;
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &pyr);
    glGenTextures(1, &sand);

    glBindTexture(GL_TEXTURE_2D, pyr);
    makeImgTexture("images/pyramid.png");

    // unsigned int met;
    // glGenTextures(1,&met);
    // glBindTexture(GL_TEXTURE_2D, met);
    // makeImgTexture("images/metal.png");

    // framebuffers wow
    FBO fbo1 = FBO();
    fbo1.createFBO(); // WORKS

    // now we attach to the Fbuffer, so make a texture
    unsigned int Ftex;
    glGenTextures(1, &Ftex);
    glBindTexture(GL_TEXTURE_2D, Ftex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    fbo1.bindTex(Ftex, 800, 600);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Fbuffer class

    // fbo1.addQueue()

    glBindTexture(GL_TEXTURE_2D, sand);

    std::vector<uint8_t> pattern;
    int patnum = 240;
    for (int j = 0; j < (patnum) / 2; j++)
    {
        /* code */

        for (int i = 0; i < (patnum) / 2; i++)
        {
            pattern.push_back(255);
            pattern.push_back(255);
            pattern.push_back(255);
            pattern.push_back(255);
            pattern.push_back(0);
            pattern.push_back(0);
            pattern.push_back(0);
            pattern.push_back(0);
        }
        for (int i = 0; i < (patnum) / 2; i++)
        {
            pattern.push_back(0);
            pattern.push_back(0);
            pattern.push_back(0);
            pattern.push_back(0);
            pattern.push_back(255);
            pattern.push_back(255);
            pattern.push_back(255);
            pattern.push_back(255);
        }
    }

    makeTexture(&pattern[0], patnum, patnum);

    // makeImgTexture("images/sand.png");
    // makeImgTexture("images/pyramid.png");
    int texPos1 = glGetUniformLocation(shaderProgram, "uTexture");

    // int texPos2 = glGetUniformLocation(shaderProgram, "uTexture");

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // attempt to make camera movement

    glm::vec4 dir = glm::vec4(0, 1, 0, 1);
    glm::mat4 cameraRotation = glm::mat4(1.0f);
    glm::vec3 perp = glm::normalize(glm::cross(glm::vec3(dir.x, dir.y, dir.z), glm::vec3(0, 1, 0)));
    glm::vec4 cPos = glm::vec4(10.001, 5, -10.001, 1);
    double xpos, ypos;
    // adham(window, xpos, ypos,cPos, perp);

    first.setTexProgPos(texPos1);
    first.setMatProgPos(rixPos);
    first.setMVPatProgPos(MVPos);
    first.setTexture(sand);

    first.setMat(glm::mat4(1.0f));   // resets matrix
    first.transMat(glm::mat4(1.0f)); // translates matrix wrt time

    // the donut

    Vobj three = Vobj(1);
    three.useTemplate(donut, glm::vec4(0, 1, 0, 1), glm::vec4(0, -1, 0, 0), 28);
    //three.transBuffer(0, glm::translate(glm::mat4(1.0f), glm::vec3(10, -2, 10)));

    
    // three.addTemplate(sphere, glm::vec4(0, 1, 0, 1), glm::vec4(0, 1, 0, 0), 28);
    three.assignProgramAndPos(shaderProgram);

    three.setTexture(sand);

    three.setMat(glm::translate(glm::mat4(1.0f), glm::vec3(0, 7, 0)));

    // // the cylinder face

    // Vobj four = Vobj(1);
    // four.useTemplate(cylinderF, glm::vec4(1, 0, 0, 1), glm::vec4(1, 0, 0, 0), 28);
    // four.assignProgramAndPos(shaderProgram);

    // four.setTexture(sand);

    // four.setMat(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.001, 0)));
    // four.transMat(glm::scale(glm::mat4(1.0f), glm::vec3(1, 5, 1)));

    // Vobj five = Vobj(1);                                                       // object creation
    // five.useTemplate(disk, glm::vec4(0, 0, 0, 1), glm::vec4(0, -1, 0, 0), 28); // object uses template
    // five.assignProgramAndPos(shaderProgram);                                   // use certain program
    // five.setTexture(sand);                                                     // use certain texture
    // five.setMat(glm::translate(glm::mat4(1.0f), glm::vec3(0, 5.001, 0)));      // set its position

    // Vobj six = Vobj(1);
    // six.useTemplate(disk, glm::vec4(0, 0, 0, 1), glm::vec4(0, 1, 0, 0), 28);
    // six.transBuffer(0, glm::translate(glm::mat4(1.0f), glm::vec3(0, 1, 0)));
    // six.addTemplate(cylinderF, glm::vec4(1, 0, 0, 1), glm::vec4(1, 0, 0, 0), 28);
    // six.addTemplate(disk, glm::vec4(0, 0, 0, 1), glm::vec4(0, -1, 0, 0), 28);
    // // six.addTemplate(cylinderF,glm::vec4(1,0,0,1),glm::vec4(1,0,0,0),28);

    // // six.useTemplate(cylinderF,glm::vec4(1,0,0,1),glm::vec4(1,0,0,0),28);
    // six.assignProgramAndPos(shaderProgram);

    // six.setTexture(sand);

    // six.setMat(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.001, 0)));
    // six.transMat(glm::scale(glm::mat4(1.0f), glm::vec3(1, 5, 1)));
    
    // //new beginnings

    Vobj seven = Vobj(1);
    seven.useCubeTemplate(glm::vec3(1,2,3));
    seven.transBuffer(0, glm::translate(glm::mat4(1.0f), glm::vec3(0, 3, 0)));
    seven.assignProgramAndPos(shaderProgram);
    seven.setTexture(sand);
    seven.setMat(glm::translate(glm::mat4(1.0f), glm::vec3(10, 0.001, 10)));
    //seven.transMat(glm::scale(glm::mat4(1.0f), glm::vec3(10, 5, 10)));


    // fbo1.addQueue(&three, sand);
    // fbo1.addQueue(&six, sand);
    fbo1.addQueue(&first, sand);
    // fbo1.addQueue(&seven, sand);
    // six.transMat(glm::translate(glm::mat4(1.0f), glm::vec3(0, 2.501, 10)));

    // six.transMat(glm::translate(glm::mat4(1.0f), glm::vec3(0, 5, 10)));

    glm::mat4 projectionMatrix = glm::perspective(
        (float)glm::radians(90.0f), // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
        4.0f / 3.0f,                // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar?
        0.1f,                       // Near clipping plane. Keep as big as possible, or you'll get precision issues.
        1000.0f                     // Far clipping plane. Keep as little as possible.
    );

    glm::mat4 CameraMatrix2 = glm::lookAt(
        glm::vec3(0.001, 20, 0.001), // the position of your camera, in world space
        glm::vec3(0, 0, 0),          // where you want to look at, in world space
        glm::vec3(0, 1, 0)           // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
    );

    glm::mat4 MVPmat2 = projectionMatrix * CameraMatrix2 * glm::mat4(1.0f);
    fbo1.setMVPat(MVPmat2);

    // the quad on screen.
    unsigned int SCREEN;
    glGenTextures(1, &SCREEN);
    glBindTexture(GL_TEXTURE_2D, SCREEN);
    makeEmptyTexture(800, 600);

    FBO screenFBO = FBO();
    screenFBO.createFBO();
    
    screenFBO.bindTex(SCREEN, 800, 600);
    screenFBO.addQueue(&first, Ftex);
    screenFBO.addQueue(&three, sand);
    // screenFBO.addQueue(&four, sand);
    // screenFBO.addQueue(&five, sand);
    // screenFBO.addQueue(&six, sand);
    screenFBO.addQueue(&seven, sand);

    Vobj QUAD = Vobj(1);
    QUAD.createVAO();
    

    std::vector<float> verts2 = {
        -1, -1, 0,
        1, -1, 0,
        -1, 1, 0,

        1, 1, 0,
        1, -1, 0,
        -1, 1, 0,


    };

    std::vector<float> vertsn2 = {
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,

        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
    };

    QUAD.createBuffer(0);
    QUAD.setObjectData(0, verts2, 3);
   
    QUAD.createBuffer(1);
    QUAD.setObjectData(1, vertsn2, 3);

    QUAD.createBuffer(2);
    QUAD.setObjectData(2, meshTexMap(1), 2);

    QUAD.setTexProgPos(texPos1);
    QUAD.setMatProgPos(rixPos);
    QUAD.setMVPatProgPos(MVPos);

    unsigned int VshQ = makeVShader(quadV);
    unsigned int FshQ = makeFShader(quadF);
    unsigned int SPQ = makeProgram(VshQ, FshQ);

    QUAD.assignProgram(SPQ);
    QUAD.setTexture(SCREEN);

    QUAD.setMat(glm::translate(glm::mat4(1.0f), glm::vec3(0, 10, 0)));

    // Vobj seven = Vobj(1);
    // seven.useTemplate(disk, glm::vec4(0, 0, 0, 1), glm::vec4(0, -1, 0, 0), 28);
    // seven.assignProgramAndPos(shaderProgram);                                   // use certain program
    // seven.setTexture(sand);                                                     // use certain texture
    // seven.setMat(glm::translate(glm::mat4(1.0f), glm::vec3(0, 15.001, 0)));  

    
    while (!glfwWindowShouldClose(window))
    {
        time++;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        // UNIFORMMMMMM

        // draw our first triangle
        // glUseProgram(shaderProgram);
        // uniformmmm
        adham(window, xpos, ypos, cPos, dir);

        glm::mat4 identity;
        glm::mat4 CameraMatrix = glm::lookAt(
            glm::vec3(cPos.x, cPos.y, cPos.z), // the position of your camera, in world space
            glm::vec3(dir),                    // where you want to look at, in world space
            glm::vec3(0, 1, 0)                 // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
        );
        // adham(window, xpos, ypos,cPos);

        // Generates a really hard-to-read matrix, but a normal, standard 4x4 matrix nonetheless

        identity = glm::mat4(1.0f);
        glm::mat4 MVPmat = projectionMatrix * CameraMatrix * identity;
        // glUniformMatrix4fv(rixPos, 1, GL_FALSE, &MVPmat[0][0]);
        //light = glm::rotate(glm::mat4(1.0f), (float)(0.01), glm::vec3(0, 1, 0)) * light;

        glUniform4f(lightpos, light.x, light.y, light.z, light.w);

        // rendering to the framebuffer

        // glBindFramebuffer(GL_FRAMEBUFFER, fbo1.fbo);
        fbo1.use();
        // six.transMat( rotateAbout(glm::mat4(1.0f),0.01,glm::vec3(0, 1, 0), glm::vec4(4,6,8,1)));
        fbo1.clearTex(255, 255, 255, 1);
        fbo1.render();

        // glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // framebuffer end

        screenFBO.use();
        screenFBO.clearTex(255, 255, 255, 1);
        screenFBO.setMVPat(MVPmat);
        screenFBO.render();

        
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);

       

        QUAD.setMVPat(glm::mat4(1.0f));
        QUAD.quickDraw();

        // seven.setMVPat(MVPmat);
        // seven.quickDraw();

        // pyramids

        // sec.quickDraw();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

/*
new main:
- misc
- clear
- update cameraMatrix 1 and 2
- render every object in a framebuffer
- render the main framebuffer
- misc
*/