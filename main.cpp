#define GL_GLEXT_PROTOTYPES

#include <iostream>
#include <SDL2/SDL.h>
#include <cstring>
#include <GL/gl.h>
#include "glm/glm.hpp"
#include <GL/glext.h>
#include "resources.h"

using namespace std;
using namespace glm;

const  int width = 640, height = 480;

GLuint vboId, vaoId, frameBuffId, texId,
       shaderProgramId, vShaderId, fShaderId;

void Init()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);

    //Creamos vbo
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, triMesh, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Creamos vao
    glEnableClientState(GL_VERTEX_ARRAY);
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDisableClientState(GL_VERTEX_ARRAY);

    //Creamos shaders
    vShaderId = glCreateShader(GL_VERTEX_SHADER);
    int size = strlen(vertexShaderSource);
    glShaderSource(vShaderId, 1, &vertexShaderSource, &size);
    glCompileShader(vShaderId);
    int success = 0; glGetShaderiv(vShaderId, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vShaderId, GL_INFO_LOG_LENGTH, &maxLength);
        char* errorLog = new char[maxLength];
        glGetShaderInfoLog(vShaderId, maxLength, &maxLength, &errorLog[0]);
        std::cout << errorLog << std::endl;
        delete[] errorLog;
        glDeleteShader(vShaderId);
        return;
    }

    fShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    size = strlen(fragmentShaderSource);
    glShaderSource(fShaderId, 1, &fragmentShaderSource, &size);
    glCompileShader(fShaderId);
    success = 0; glGetShaderiv(fShaderId, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fShaderId, GL_INFO_LOG_LENGTH, &maxLength);
        char* errorLog = new char[maxLength];
        glGetShaderInfoLog(fShaderId, maxLength, &maxLength, &errorLog[0]);
        std::cout << errorLog << std::endl;
        delete[] errorLog;
        glDeleteShader(fShaderId);
        return;
    }

    //Creamos programa
    shaderProgramId = glCreateProgram();
    glAttachShader(shaderProgramId, vShaderId);
    glAttachShader(shaderProgramId, fShaderId);
    glLinkProgram(shaderProgramId);

    //Create texture and framebuff to render to
    glGenFramebuffers(1, &frameBuffId);
}

float appTime = 0.0, rot = 0.0;

void RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffId);

    glBindVertexArray(vaoId);
    glUseProgram(shaderProgramId);
    glUniform1f(0, appTime);
    glUniform1i(1, width);
    glUniform1i(2, height);
    mat4 transform = mat4(1.0f);
    vec3 axis = vec3(0.0, 1.0, 0.0);
    transform = glm::rotate_slow(transform, rot, axis);
    glUniformMatrix4fv(3, 16, GL_FALSE, value_ptr(transform));
    mat4 projection = mat4(1.0f);
    projection = perspectiveFov(45 * 3.1415/180, 1.0, 1.0, 0.1, -100.0);
    glUniformMatrix4fv(4, 16, GL_FALSE, value_ptr(projection));
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    appTime += 0.01;
    rot += 0.01;
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
