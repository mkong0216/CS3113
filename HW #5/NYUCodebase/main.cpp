#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include <math.h>
#include "ShaderProgram.h"
#include "glm/gtc/matrix_transform.hpp"

#include "SheetSprite.hpp"
#include "Entity.hpp"
#include "GameState.hpp"
#include "utilities.hpp"
#include "MainMenu.hpp"
#include "GameLevel.hpp"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

using namespace std;

SDL_Window* displayWindow;
ShaderProgram texturedProgram;
ShaderProgram untexturedProgram;
SDL_Event event;

glm::mat4 projectionMatrix = glm::mat4(1.0f);
glm::mat4 viewMatrix = glm::mat4(1.0f);

bool done = false;
float lastFrameTicks = 0.0f;
float elapsed;

// Space Invaders entities
GameMode mode;
GameState state;
MainMenu mainMenu;
GameLevel gameLevel;

// Textures
GLuint fontTexture;
GLuint spritesheet;

void SetUpGameState () {
    // Initializing game entities, load textures, and setting state to gameLevel.
    state.SetUpEnemies(spritesheet);
    state.SetUpPlayer(spritesheet);
    state.SetUpBullets();
    state.SetUpEndLine();

    state.fontTexture = fontTexture;
    state.spritesheet = spritesheet;
    
    gameLevel.state = state;
    gameLevel.texturedProgram = texturedProgram;
    gameLevel.untexturedProgram = untexturedProgram;
}

void SetUpMainMenu () {
    // Setting textures to MainMenu
    mainMenu.fontTexture = fontTexture;
    mainMenu.spritesheet = spritesheet;
    mainMenu.program = texturedProgram;
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
    
    texturedProgram.Load(RESOURCE_FOLDER "vertex_textured.glsl", RESOURCE_FOLDER "fragment_textured.glsl");
    untexturedProgram.Load(RESOURCE_FOLDER "vertex.glsl", RESOURCE_FOLDER "fragment.glsl");
    
    fontTexture = LoadTexture(RESOURCE_FOLDER "text.png");
    spritesheet = LoadTexture(RESOURCE_FOLDER "spaceinvaders.png");

    projectionMatrix = glm::ortho(-1.777f, 1.777f, -1.0f, 1.0f, -1.0f, 1.0f);
    
    // Setting up textured and untextured programs.
    texturedProgram.SetProjectionMatrix(projectionMatrix);
    texturedProgram.SetViewMatrix(viewMatrix);
    untexturedProgram.SetProjectionMatrix(projectionMatrix);
    untexturedProgram.SetViewMatrix(viewMatrix);

    // Using textured program first.
    glUseProgram(texturedProgram.programID);
    
    // Initializing SDL_mixer
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 );

    gameLevel.shooting = Mix_LoadWAV(RESOURCE_FOLDER "/sounds/shoot.wav");
    gameLevel.spaceinvaderkilled = Mix_LoadWAV(RESOURCE_FOLDER "/sounds/invaderkilled.wav");
    gameLevel.music = Mix_LoadMUS(RESOURCE_FOLDER "/sounds/spaceinvaders1.mpeg");
    Mix_PlayMusic(gameLevel.music, -1);
    // Start with main menu mode.
    mode = STATE_MAIN_MENU;

    SetUpGameState();
    SetUpMainMenu();
}

void ProcessEvents () {
    // Calculating elapsed time
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        } else {
            switch (mode) {
                case STATE_MAIN_MENU:
                    mode = mainMenu.ProcessInput(event);
                    break;
                case STATE_GAME_LEVEL:
                    gameLevel.ProcessInput(event, elapsed);
            }
        }
    }
}

void Update () {
    glClear(GL_COLOR_BUFFER_BIT);
    
    switch (mode) {
        case STATE_GAME_LEVEL:
            gameLevel.Update(elapsed);
            break;
        case STATE_MAIN_MENU:
            break;
    }
}



void Render () {
    switch (mode) {
        case STATE_MAIN_MENU:
            mainMenu.Render();
            break;
        case STATE_GAME_LEVEL:
            gameLevel.Render();
            break;
    }

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
    
    Mix_FreeChunk(gameLevel.shooting);
    Mix_FreeChunk(gameLevel.spaceinvaderkilled);
    Mix_FreeMusic(gameLevel.music);
    
    SDL_Quit();
    return 0;
}
