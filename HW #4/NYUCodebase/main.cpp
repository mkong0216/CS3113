#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "SheetSprite.hpp"
#include "GameLevel.hpp"

SDL_Window* displayWindow;
bool done = false;

// Keeping track of time frames
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

float lastFrameTicks = 0.0f;
float elapsed;
float accumulator = 0.0f;

// Shader Program and Matrices
ShaderProgram program;
glm::mat4 projectionMatrix = glm::mat4(1.0f);
glm::mat4 viewMatrix = glm::mat4(1.0f);
glm::mat4 modelMatrix = glm::mat4(1.0f);

// Setting up spritesheets and GameLevel object
SheetSprite spritesheet(30, 30);
SheetSprite alphabet(16, 16);
GameLevel platformGame;

// INSTRUCTIONS TO PLAY GAME:
// Arrow keys to move left and right
// Space bar to jump
// Goal of game: find key to door

void SetUp () {
    // Setting up display window
    int viewportWidth = 640;
    int viewportHeight = 360;
    
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, viewportWidth, viewportHeight, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, viewportWidth, viewportHeight);
    
    program.Load(RESOURCE_FOLDER "vertex_textured.glsl", RESOURCE_FOLDER "fragment_textured.glsl");
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Loading spritesheets
    spritesheet.LoadTexture(RESOURCE_FOLDER "spritesheet_rgba.png");
    alphabet.LoadTexture(RESOURCE_FOLDER "text.png");

    platformGame.spritesheet = spritesheet;
    platformGame.alphabet = alphabet;

    // Setting entity properties
    platformGame.SetUp();

    // Setting up matrices
    projectionMatrix = glm::ortho(-1.777f, 1.777f, -1.0f, 1.0f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetModelMatrix(modelMatrix);
    
    glUseProgram(program.programID);
}

void ProcessEvents (SDL_Event &event) {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE || platformGame.state == STATE_GAME_EXIT) {
            done = true;
        } else {
            platformGame.ProcessInput(event);
        }
    }
    
    glClear(GL_COLOR_BUFFER_BIT);
}

void Update () {
    // Set background to Light Blue
    glClearColor(0.67, 0.85, 0.90, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Fixing the timestep
    while (elapsed >= FIXED_TIMESTEP) {
        if (platformGame.state == STATE_GAME_START) {
            platformGame.Update(FIXED_TIMESTEP);
        }

        elapsed -= FIXED_TIMESTEP;
    }
    
    accumulator = elapsed;
}

void Render () {
    if (platformGame.state == STATE_GAME_START) {
        platformGame.Render(program);
    } else {
        platformGame.RenderGameEnd(program);
    }

    SDL_GL_SwapWindow(displayWindow);
}

int main(int argc, char *argv[])
{
    SetUp();
    
    SDL_Event event;
    while (!done) {
        ProcessEvents(event);
        Update();
        Render();
    }
    
    SDL_Quit();
    return 0;
}
