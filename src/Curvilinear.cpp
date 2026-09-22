#include <iostream>
#include <cmath>
#include <cstdlib>
#include <GLAD\glad.h>
#include <GLFW\glfw3.h>
#include "Shader.hpp"
#include "GLcamera.hpp"

using namespace std;
using namespace shader;
const GLuint CLIENTWIDTH = 900;
const GLuint CLIENTHEIGHT = 900;

void processInput(GLFWwindow *window,Camera* cam)
{
const float SPEED = 0.1f;
    
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {;}

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {;}

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {cam->ProcessKeyboard(FORWARD,0.1);;}

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {cam->ProcessKeyboard(BACKWARD,0.1);;}
    
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {cam->ProcessKeyboard(RIGHT,0.1);;}

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {cam->ProcessKeyboard(LEFT,0.1);;}
}

size_t to1D(size_t x,size_t y,size_t z,size_t n)
    {
        return y*n*n + z*n + x;
    }

GLfloat F(GLfloat u,GLfloat v,GLfloat w)
{
   // return u*sin(2*M_PI*v)*cos(2*M_PI*w);//Spherical
   // return u;//Cartisian
   //return u*cos(2*M_PI*v);//Cylindrical
   // return (u*u+v*v)/2;//paraboic cylindrical
    return  0.5*cosh(u)*cos(2*M_PI*v);
    

}

GLfloat G(GLfloat u,GLfloat v,GLfloat w)
{
   // return u*sin(2*M_PI*v)*sin(2*M_PI*w); //Spherical
   //return v;//cartisian
  // return u*sin(2*M_PI*v);//cylindrical
 // return u*v;//Parabolic cylindrical
 
 return 0.5*sinh(u)*sin(2*M_PI*v);
}

GLfloat H(GLfloat u, GLfloat v, GLfloat w)
    {
      //Spherical//  return u*cos(2*M_PI*v);
    //return w;//cartisian
    //return w;//cylindrical
    return w;//parabolic cylindrical

    }

GLfloat* FillCoordinates(unsigned long long n,   GLfloat (*xfunc)(GLfloat,GLfloat,GLfloat )  ,   GLfloat (*yfunc)(GLfloat,GLfloat,GLfloat )  ,  GLfloat (*zfunc)(GLfloat,GLfloat,GLfloat))
    {
        GLfloat* result = (GLfloat*)malloc(3*n*n*n*sizeof(GLfloat));  
        size_t l = 0;
        for (size_t  j = 0;j<n;j++)
            {
            for (size_t k = 0;k<n;k++)
                {
                for (size_t i = 0;i < n;i++)
                    {
                        GLfloat u = (GLfloat)i/n;
                        GLfloat v = (GLfloat)j/n;
                        GLfloat w = (GLfloat)k/n;
                     result[l + 0] = 2*xfunc(u,v,w) - 1;
                     result[l + 1] = 2*yfunc(u,v,w) - 1;
                     result[l + 2] = 2*zfunc(u,v,w) - 1;
                     l+=3;
                     //f(x,y,z) = result[ y*(n*n) + z*n) + x ]
                    }
                }
            }
            return result;
    }


GLuint* FillIndices(unsigned long long n)
    {
        GLuint* ind = (GLuint*)calloc( (6*(n-1)*n*n),sizeof(GLuint)       );
        size_t m = 0;
            for (size_t k = 0;k<n;k++)
            {
                for (size_t j = 0;j<n;j++)
                    {
                    for(size_t i = 0;i<n -1 ;i++)
                        {
                        ind[m+0] = k*n*n + j*n + i ;
                        ind[m+1] = k*n*n + j*n + i + 1 ;
                        m+=2;
                        } 
                     }
            }

             for (size_t k = 0;k<n;k++)
            {
                for (size_t j = 0;j<n;j++)
                    {
                    for(size_t i = 0;i<n -1 ;i++)
                        {
                        ind[m+0] = k*n*n + i*n + j ;
                        ind[m+1] = k*n*n + (i+1)*n + j ;
                        m+=2;
                        } 
                     }
            }

             for (size_t k = 0;k<n;k++)
            {
                for (size_t j = 0;j<n;j++)
                    {
                    for(size_t i = 0;i<n -1 ;i++)
                        {
                        ind[m+0] = i*n*n + j*n + k ;
                        ind[m+1] = (i+1)*n*n + j*n + k  ;
                        m+=2;
                        } 
                     }
            }
        
        return ind;
    }

int main()
{
glfwInit();
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

GLFWwindow* win = glfwCreateWindow(CLIENTWIDTH,CLIENTHEIGHT,"Curvilinear ",NULL,NULL);
glfwMakeContextCurrent(win);
gladLoadGL();
glViewport(0,0,CLIENTWIDTH,CLIENTHEIGHT);cout<<"GLFW initialized "<<endl;


GLuint vShader;  
GLuint fShader;
GLuint sProgram;

char* fcode = "";
char* vcode = "";

Shader shade(fcode,vcode);
vShader =  shade.getVertexShader();
fShader =  shade.getFragmentShader();
sProgram = shade.getProgramShader(vShader,fShader);

   
    GLuint VBO;
    GLuint VAO;
    GLuint VEO;
    size_t n = 12;
    GLfloat* GridPoints = FillCoordinates(n,F,G,H);
    GLuint* indices = FillIndices(n);


     glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); 
    glGenBuffers(1, &VEO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 3*n*n*n*sizeof(GLfloat), GridPoints, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*(n-1)*n*n*sizeof(GLuint), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 3 * sizeof(GLfloat), (void *)0);

    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
   // glBindVertexArray(0);

int viewLoc = glGetUniformLocation(sProgram, "view");
int projectLoc = glGetUniformLocation(sProgram, "projection");
mat4 proj = glm::ortho(-1.5f,1.5f, -1.5f,1.5f, -1000.0f,1000.0f );
GLfloat time = glfwGetTime();
GLfloat fps = 60.0f;
    Camera cam(0.0f,F(0,0,0),0.0f   ,  0.0f,1.0f,0.0f  , 0.0f,0.0f);
while (!glfwWindowShouldClose(win))
{

    if (time + 1/fps <= glfwGetTime())
    {
        time = glfwGetTime();
        double x,y;
                    glfwGetCursorPos(win,&x,&y);
                    double dx = x - (float)CLIENTWIDTH/2 ;
                    double dy = y - (float)CLIENTHEIGHT/2 ;
                    
                    cam.ProcessMouseMovement(-dx,dy);
                   

                    glfwSetCursorPos(win,CLIENTWIDTH/2.0f,CLIENTHEIGHT/2.0f);
                    
                    mat4 view = mat4(1.0f);  
                   

                    //Note to self: Make use of the zbuffer to stop overyapping 
                    view = cam.GetViewMatrix();
                    glUniformMatrix4fv(viewLoc , 1 , GL_FALSE , value_ptr(view));
                    glUniformMatrix4fv(projectLoc , 1 , GL_FALSE , value_ptr(proj));
                    
                    processInput(win,&cam);
    }

    glfwPollEvents();
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glUseProgram(sProgram);
            glBindVertexArray(VAO);
            glDrawElements(GL_LINES, 6*(n-1)*n*n , GL_UNSIGNED_INT, 0);
            glfwSwapBuffers(win);
}

}