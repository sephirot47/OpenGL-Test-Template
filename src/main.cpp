#define GL_GLEXT_PROTOTYPES

#include <iostream>
#include <SDL2/SDL.h>
#include <cstring>
#include <GL/gl.h>
#include <GL/glext.h>

#include "../include/glm/glm.hpp"
#include "../include/Shader.h"
#include "../include/ShaderProgram.h"

using namespace std;
using namespace glm;

const float triMesh[9] = {-1.0f, -1.0f, 0.0f,
                           0.0f,  1.0f, 0.0f,
                           1.0f, -1.0f, 0.0f};


const  int width = 640, height = 480;

GLuint vboId, vaoId, frameBuffId, texId,
       shaderProgramId, vShaderId, fShaderId;

Shader *vshader, *fshader;
ShaderProgram *program;


void Init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);

    vshader = new Shader(); if( !vshader->Create("vshader", VertexShader) ) std::cout << "FUUUU" << std::endl;
    fshader = new Shader(); if( !fshader->Create("fshader", FragmentShader) ) std::cout << "FUUUU" << std::endl;

    program = new ShaderProgram();
    program->AttachShader(*vshader);
    program->AttachShader(*fshader);
    program->Link();

    //Creamos vbo
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triMesh), triMesh, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Creamos vao
    glEnableClientState(GL_VERTEX_ARRAY);
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program->GetId(), "position"));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDisableClientState(GL_VERTEX_ARRAY);
}

float appTime = 0.0, rot = 0.0;

void RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vaoId);
    program->Use();

    mat4 transform(1.0f);
    vec3 axis = vec3(0.0, 1.0, 0.0), translate(0.0, 0.0, -3.0), scale(0.5);
    mat4 T = glm::translate(transform, translate);
    mat4 R = glm::rotate_slow(transform, rot, axis);
    mat4 S = glm::scale(transform, scale);
    transform = T * R * S;

    mat4 projection(1.0f);
    projection = perspective(45.0f * 3.1415f/180.0f, 1.0f, 0.1f, 100.0f);

    glUniform1f(glGetUniformLocation(program->GetId(), "time"), appTime);
    glUniformMatrix4fv(glGetUniformLocation(program->GetId(), "transform"), 1, GL_FALSE, value_ptr(transform));
    glUniformMatrix4fv(glGetUniformLocation(program->GetId(), "projection"), 1, GL_FALSE, value_ptr(projection));

    glDrawArrays(GL_TRIANGLES, 0, 3);
    program->UnUse();
    glBindVertexArray(0);

    appTime += 0.03;
    rot += 0.09;
}


int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* win;
    win = SDL_CreateWindow("Pruebas OpenGL", 100, 100, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    SDL_GLContext context;
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    context = SDL_GL_CreateContext(win);

    Init();
    bool running = true;
    while(running)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
                running = false;
        }

        RenderScene();
        SDL_GL_SwapWindow(win);
        SDL_Delay(25);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
