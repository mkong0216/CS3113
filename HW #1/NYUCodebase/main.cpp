#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "glm/gtc/matrix_transform.hpp"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;

GLuint LoadTexture(const char *filePath) {
    int w, h, comp;
    unsigned char* image = stbi_load(filePath, &w, &h, &comp, STBI_rgb_alpha);
    
    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }
    
    GLuint retTexture;
    glGenTextures(1, &retTexture);
    glBindTexture(GL_TEXTURE_2D, retTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    stbi_image_free(image);
    return retTexture;
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 360);
    
    ShaderProgram program;
    program.Load(RESOURCE_FOLDER "vertex_textured.glsl", RESOURCE_FOLDER "fragment_textured.glsl");
    
    float lastFrameTicks = 0.0f;
    float positionX = 0.0f;

    float pacmanPosX = -1.277f;
    float rightEdge = 1.277f;
    float leftEdge = -1.277f;

    float moveLeft = true;
    bool hideCherry = false;

    GLuint ghostTexture = LoadTexture(RESOURCE_FOLDER "sprites/ghost.png");
    GLuint pacmanTexture = LoadTexture(RESOURCE_FOLDER "sprites/pacman.png");
    GLuint cherryTexture = LoadTexture(RESOURCE_FOLDER "sprites/cherry.png");

    glm::mat4 projectionMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix = glm::mat4(1.0f);

    glm::mat4 modelMatrix_ghost = glm::mat4(1.0f);
    glm::mat4 modelMatrix_pacman = glm::mat4(1.0f);
    glm::mat4 modelMatrix_cherry = glm::mat4(1.0f);

    projectionMatrix = glm::ortho(-1.777f, 1.777f, -1.0f, 1.0f, -1.0f, 1.0f);
    glUseProgram(program.programID);

    SDL_Event event;
    bool done = false;
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
        }
    
        glClear(GL_COLOR_BUFFER_BIT);

        program.SetProjectionMatrix(projectionMatrix);
        program.SetViewMatrix(viewMatrix);

        float ticks = (float)SDL_GetTicks()/1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        if (moveLeft == true) {
            positionX += elapsed * 0.01;
        } else {
            positionX -= elapsed * 0.01;
        }
        
        // Calculate where pacman is based on positionX
        pacmanPosX += positionX;
        
        if (pacmanPosX >= rightEdge) {
            moveLeft = false;
            hideCherry = true;
            positionX = 0;
        } else if (pacmanPosX <= leftEdge) {
            moveLeft = true;
            hideCherry = false;
            positionX = 0;
        }

        // Drawing cherry sprite
        program.SetModelMatrix(modelMatrix_cherry);
        glBindTexture(GL_TEXTURE_2D, cherryTexture);
        
        float vertices[] = {1.277f, -0.5, 1.777f, 0.0, 1.277, 0.0, 1.277f, -0.5, 1.777f, -0.5, 1.777f, 0.0};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
        glEnableVertexAttribArray(program.positionAttribute);
        
        float texCoords[] = {0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0};
        glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
        glEnableVertexAttribArray(program.texCoordAttribute);
        
        if (hideCherry == false) {
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        // Drawing ghost sprite
        modelMatrix_ghost = glm::translate(modelMatrix_ghost, glm::vec3(positionX, 0.0f, 0.0f));
        program.SetModelMatrix(modelMatrix_ghost);
        glBindTexture(GL_TEXTURE_2D, ghostTexture);
        
        float vertices2[] = {-1.777f, -0.5, -1.277f, 0.0, -1.777, 0.0, -1.777f, -0.5, -1.277f, -0.5, -1.277f, 0.0};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices2);
        glEnableVertexAttribArray(program.positionAttribute);
        
        float flippedCoords[] = {1.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0};
        if (moveLeft == false) {
            glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, flippedCoords);
        } else {
            glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
        }

        glEnableVertexAttribArray(program.texCoordAttribute);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Drawing pacman sprite
        modelMatrix_pacman = glm::translate(modelMatrix_pacman, glm::vec3(positionX, 0.0f, 0.0f));
        program.SetModelMatrix(modelMatrix_pacman);
        glBindTexture(GL_TEXTURE_2D, pacmanTexture);
        
        float vertices3[] = {-1.277f, -0.5, -0.777f, 0.0, -1.277, 0.0, -1.277f, -0.5, -0.777f, -0.5, -0.777f, 0.0};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices3);
        glEnableVertexAttribArray(program.positionAttribute);
        
        float texCoords2[] = {0.0, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0};
        float flippedCoords2[] = {1.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};
        
        if (moveLeft == false) {
            glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, flippedCoords2);
        } else {
            glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords2);
        }

        glEnableVertexAttribArray(program.texCoordAttribute);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisableVertexAttribArray(program.positionAttribute);
        glDisableVertexAttribArray(program.texCoordAttribute);
        
        SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}
