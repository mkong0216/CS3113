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

SDL_Window* displayWindow;
ShaderProgram program;
SDL_Event event;

glm::mat4 projectionMatrix = glm::mat4(1.0f);
glm::mat4 viewMatrix = glm::mat4(1.0f);

bool done = false;
bool gameStarted = false;
float lastFrameTicks = 0.0f;
float elapsed;

// Global pong and paddle matrices
glm::mat4 pongMatrix;
glm::mat4 paddleOneMatrix;
glm::mat4 paddleTwoMatrix;

// Setting up position and velocity variables
float paddleOnePosY = 0.0f;
float paddleTwoPosY = 0.0f;
float pongPosX = 0.0f;
float pongPosY = 0.0f;
float pongAngle = 45.0f;

float deltaPaddleY = 0.0f;  // Change between current and previous keyboard press position
float deltaPongX = 0.0f;    // Change in velocity in X direction
float deltaPongY = 0.0f;    // Change in velocity in Y direction
float deltaMouseY = 0.0f;

float directionX = 0.0f;
float directionY = 0.0f;

// Checks if pong ball collided with an object (wall or paddle)
bool collisionCheck (std::string object, float objectY, float pongX, float pongY) {
    float objectX;
    float pongSize = 0.1f;
    float paddleWidth = 0.025f; // Only consider it colliding if it hits front half of paddle
    float paddleHeight = 0.4f;

    if (object == "player1") {
        objectX = -1.625f; // Only consider it colliding if it hits the front half of the paddle
    } else if (object == "player2") {
        objectX = 1.625f;
    }
    
    float distanceX = fabs(pongX - objectX) - (paddleWidth + pongSize) / 2;
    float distanceY = fabs(pongY - objectY) - (paddleHeight + pongSize) / 2;

    return (distanceX < 0 && distanceY < 0);
}

void SetUp () {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 360);
    
    program.Load(RESOURCE_FOLDER "vertex.glsl", RESOURCE_FOLDER "fragment.glsl");
    projectionMatrix = glm::mat4(1.0f);
    viewMatrix = glm::mat4(1.0f);
    
    // Initializing pong and paddle matrices
    pongMatrix = glm::mat4(1.0f);
    paddleOneMatrix = glm::mat4(1.0f);
    paddleTwoMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-1.777f, 1.777f, -1.0f, 1.0f, -1.0f, 1.0f);
}

void ProcessEvents () {
    // Calculating elapsed time
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    deltaMouseY = 0.0f;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP || event.type == SDL_MOUSEMOTION) {
            if (event.type == SDL_MOUSEMOTION) {
                // Calculate mouse Y position according to world coordinate system
                float unitY = (((float)(360 - event.motion.y) / 360.0f) * 2.0f) - 1.0f;
                deltaMouseY = unitY - paddleTwoPosY;
                paddleTwoPosY = unitY;
            }
            
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_s) {
                    if (event.key.keysym.sym == SDLK_s) {
                        deltaPaddleY = -6.0f * elapsed;
                    } else {
                        deltaPaddleY = 6.0f * elapsed;
                    }
                }
            } else if (event.type == SDL_KEYUP) {
                deltaPaddleY = 0.0f;
            }
        } else if (event.type == SDL_MOUSEBUTTONDOWN && !gameStarted) {
            // If game has not started yet and mouse has clicked, start game and initialize pong velocity
            gameStarted = true;
            float startAngle = 45.0f;
            directionX = cosf(startAngle * 3.14159 / 180);
            directionY = sinf(startAngle * 3.14159 / 180);
        }
    }
}

void UpdatePaddles () {
    // Calculating next paddle positions
    float nextPaddlePosY = paddleOnePosY + deltaPaddleY;
    // If paddle will collide with top or bottom wall, do not move further
    if (nextPaddlePosY >= 1.0f || nextPaddlePosY <= -1.0f) {
        deltaPaddleY = 0.0f;
    } else {
        paddleOnePosY = nextPaddlePosY;
    }
}

void UpdatePongBall () {
    // Calculating and translating new pong positions
    deltaPongX = directionX * elapsed * 5.0f;
    deltaPongY = directionY * elapsed * 5.0f;
    pongPosX += deltaPongX;
    pongPosY += deltaPongY;
    
    // Checking if pong collided with paddles
    bool collidePaddleOne = collisionCheck("player1", paddleOnePosY, pongPosX, pongPosY);
    bool collidePaddleTwo = collisionCheck("player2", paddleTwoPosY, pongPosX, pongPosY);
    
    // Checking for collisions (walls or paddles)
    if ((pongPosY + 0.05 >= 1.0f && pongPosY >= 1.0f) || (pongPosY - 0.05 <= -1.0f && pongPosY <= -1.0f)) {
        // Pong ball hit top wall or bottom wall
        pongAngle = pongAngle * -1; // 45 degrees becomes -45 degrees
        directionX = cosf(pongAngle * 3.14159 / 180);
        directionY = sinf(pongAngle * 3.14159 / 180);
    } else if (collidePaddleOne || collidePaddleTwo) {
        // Pong ball hit player one or player two's paddle
        float deltaAngle = 90;
        if (pongAngle < 0) {
            deltaAngle = (collidePaddleOne) ? deltaAngle : deltaAngle * -1;
        } else {
            deltaAngle = (collidePaddleOne) ? deltaAngle * -1 : deltaAngle;
        }
        
        pongAngle += deltaAngle;
        directionX = cosf(pongAngle * 3.14159 / 180);
        directionY = sinf(pongAngle * 3.14159 / 180);
    } else if (pongPosX + 0.05 >= 1.777f || pongPosX - 0.05 <= -1.777f) {
        // Pong ball reached left or right walls meaning a player was able to score
        // Reset pong back to center and change gameStarted to false
        pongMatrix = glm::mat4(1.0f);
        pongPosX = 0.0f;
        pongPosY = 0.0f;
        deltaPongX = 0.0f;
        deltaPongY = 0.0f;
        directionX = 0.0f;
        directionY = 0.0f;
        pongAngle = 45.0f;
        
        gameStarted = false;
    }
}

void Update () {
    glClear(GL_COLOR_BUFFER_BIT);
    
    program.SetModelMatrix(pongMatrix);
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    UpdatePaddles();
    UpdatePongBall();
}

void Render () {
    // Drawing Paddle One
    paddleOneMatrix = glm::translate(paddleOneMatrix, glm::vec3(0.0f, deltaPaddleY, 0.0f));
    program.SetModelMatrix(paddleOneMatrix);
    
    float paddle1[] = {-1.7f, -0.2f, -1.6f, 0.2f, -1.7f, 0.2f, -1.7f, -0.2f, -1.6f, -0.2f, -1.6f, 0.2f};
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, paddle1);
    glEnableVertexAttribArray(program.positionAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    // Drawing Paddle Two
    paddleTwoMatrix = glm::translate(paddleTwoMatrix, glm::vec3(0.0f, deltaMouseY, 0.0f));
    program.SetModelMatrix(paddleTwoMatrix);
    
    float paddle2[] = {1.6f, -0.2f, 1.7f, 0.2f, 1.6f, 0.2f, 1.6f, -0.2f, 1.7f, -0.2f, 1.7f, 0.2f};
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, paddle2);
    glEnableVertexAttribArray(program.positionAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    // Drawing Pong Ball
    pongMatrix = glm::translate(pongMatrix, glm::vec3(deltaPongX, deltaPongY, 0.0f));
    program.SetModelMatrix(pongMatrix);
    
    float pong[] = {-0.05f, -0.05f, 0.05f, 0.05f, -0.05f, 0.05f, -0.05f, -0.05f, 0.05f, -0.05f, 0.05f, 0.05f};
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, pong);
    glEnableVertexAttribArray(program.positionAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
    SDL_GL_SwapWindow(displayWindow);
}

int main(int argc, char *argv[])
{
    SetUp();
    while (!done) {
        ProcessEvents();
        Update();
        Render();
    }
    
    SDL_Quit();
    return 0;
}
