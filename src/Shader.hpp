#pragma once
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <GLAD\glad.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;
namespace shader
{
class Shader
    {
    private:
        char* vertexShader;
        char* fragmentShader;


        char* defaultVShader = 
        "#version 330 core\n"
         "layout (location = 0) in vec3 aPos;\n"
         "uniform mat4 view;\n"
         "uniform mat4 projection;\n"
         "out vec4 vColor;\n "
         "void main()\n"
         "{\n"
         "   gl_Position = projection * view * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
         "    vColor = vec4(aPos.xyz,1.0f);\n"
         "}\0";

         char* defaultFShader = 
         "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec4 vColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(0.0,0.0,0.0,1.0);\n"
        "}\n\0";
        

    public:
    Shader(char* vShader,char* fShader)
    {
        if (vShader == "")
        {
         vertexShader = defaultVShader;
         
        } else vertexShader = vShader;

    
        if (fShader == "")
        {
         fragmentShader = defaultFShader;
         
        } else fragmentShader = fShader;
    
    }

    GLuint getVertexShader()
    {
        GLuint vShader;
        vShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vShader, 1, &vertexShader, NULL);
        glCompileShader(vShader);
        int vsuc;
        char vInfo[512];
        glGetShaderiv(vShader, GL_COMPILE_STATUS, &vsuc);
        if (!vsuc)
        {
            glGetShaderInfoLog(vShader, 512, NULL, vInfo);
            std::cout << vInfo << endl;
        }
        return vShader;
    }
    GLuint getFragmentShader()
    {
       GLuint fShader; 
        fShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fShader, 1, &fragmentShader, NULL);
        glCompileShader(fShader);
        int fsuc;
        char fInfo[512];
        glGetShaderiv(fShader, GL_COMPILE_STATUS, &fsuc);
        if (!fsuc)
        {
            glGetShaderInfoLog(fShader, 512, NULL, fInfo);
            std::cout << fInfo << endl;
        }   
        return fShader;
    }

    GLuint getProgramShader(GLuint vShader,GLuint fShader)
    {
        GLuint sProgram;
        sProgram = glCreateProgram();
        glAttachShader(sProgram, vShader);
        glAttachShader(sProgram, fShader);
        glLinkProgram(sProgram);

        int gsuc;
        char gInfo[512];
        glGetProgramiv(sProgram, GL_COMPILE_STATUS, &gsuc);
        if (!gsuc)
        {
            glGetProgramInfoLog(sProgram, 512, NULL, gInfo);
            std::cout << gInfo << endl;
        }
        return sProgram;
    }

    };
}