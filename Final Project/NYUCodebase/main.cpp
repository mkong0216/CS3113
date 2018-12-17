#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "glm/gtc/matrix_transform.hpp"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#include "GameLevel.hpp"
#include "utils.hpp"

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

// Setting up game
GameLevel gameLevel;
Mix_Music *music;
Mix_Chunk *collision;

void SetUpTextures () {
    // Loading textures
    gameLevel.fontTexture = LoadTexture(RESOURCE_FOLDER "sprites/font1.png", true);
    gameLevel.buttonTexture = LoadTexture(RESOURCE_FOLDER "sprites/greySheet.png", true);
    gameLevel.spriteTexture = LoadTexture(RESOURCE_FOLDER "sprites/spritesheet_rgba.png", false);
    gameLevel.selectPlayerOne = LoadTexture(RESOURCE_FOLDER "sprites/blue_sliderDown.png", true);
    gameLevel.selectPlayerTwo = LoadTexture(RESOURCE_FOLDER "sprites/green_sliderDown.png", true);
    gameLevel.labelTexture = LoadTexture(RESOURCE_FOLDER "sprites/blue_button03.png", true);
    gameLevel.SetUp();
}

void SetUp () {
    // Setting up display window
    int viewportWidth = 640;
    int viewportHeight = 360;
    
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Final Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, viewportWidth, viewportHeight, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, viewportWidth, viewportHeight);
    
    program.Load(RESOURCE_FOLDER "vertex_textured.glsl", RESOURCE_FOLDER "fragment_textured.glsl");
    SetUpTextures();
    
    // Setting up sound
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS(RESOURCE_FOLDER "sounds/marching_cat.mp3");
    Mix_VolumeMusic(30);
    Mix_PlayMusic(music, -1);
    
    collision = Mix_LoadWAV(RESOURCE_FOLDER "sounds/collision2.wav");
    gameLevel.SetUpSound(collision);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Setting up matrices
    projectionMatrix = glm::ortho(-1.777f, 1.777f, -1.0f, 1.0f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetModelMatrix(modelMatrix);
    
    glClearColor(0.67, 0.85, 0.90, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(program.programID);
}

void ProcessEvents (SDL_Event &event) {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE || gameLevel.getGameMode() == STATE_GAME_OVER) {
            done = true;
        }
        gameLevel.ProcessInput(event);
    }
    
    glClear(GL_COLOR_BUFFER_BIT);
}

void Update () {
    // Fixing the timestep
    elapsed += accumulator;
    if (elapsed < FIXED_TIMESTEP) {
        accumulator = elapsed;
    }

    while (elapsed >= FIXED_TIMESTEP) {
        gameLevel.Update(FIXED_TIMESTEP);
        elapsed -= FIXED_TIMESTEP;
    }
    
    glClearColor(0.67, 0.85, 0.90, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    accumulator = elapsed;
}

void Render () {
    gameLevel.Render(program);
    SDL_GL_SwapWindow(displayWindow);
}

void CleanUp () {
    SDL_Quit();
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
    
    CleanUp();
    return 0;
}
