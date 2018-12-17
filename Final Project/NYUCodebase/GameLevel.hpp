//
//  GameLevel.hpp
//  NYUCodebase
//
//  Created by Mandy Kong on 12/13/18.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#ifndef GameLevel_hpp
#define GameLevel_hpp

#include <stdio.h>
#include <SDL_mixer.h>
#include "ShaderProgram.h"
#include "MainMenu.hpp"
#include "GameState.hpp"

enum GameMode { STATE_MAIN_MENU, STATE_PLAYER_SELECT, STATE_GAME_START, STATE_GAME_OVER, STATE_GAME_QUIT };

class GameLevel {
public:
    GameLevel();
    void SetUp();
    void SetUpSound(Mix_Chunk *collision);
    void Render(ShaderProgram &program);
    void Update(float elapsed);
    void ProcessInput(SDL_Event &event);

    GameMode getGameMode();
    
    GLuint fontTexture;
    GLuint buttonTexture;
    GLuint spriteTexture;
    GLuint labelTexture;

    GLuint selectPlayerOne;
    GLuint selectPlayerTwo;
private:
    GameMode mode;
    MainMenu menu;
    GameState platformGame;
    bool multiplayer;
};

#endif /* GameLevel_hpp */
