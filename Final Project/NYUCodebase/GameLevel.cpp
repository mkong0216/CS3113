//
//  GameLevel.cpp
//  NYUCodebase
//
//  Created by Mandy Kong on 12/13/18.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#include "GameLevel.hpp"

GameLevel::GameLevel () {
    mode = STATE_MAIN_MENU;
}

void GameLevel::SetUp() {
    multiplayer = false;

    // Setting up spritesheets
    menu.SetUp(fontTexture, buttonTexture);
    platformGame.SetUp(spriteTexture, fontTexture, selectPlayerOne, selectPlayerTwo, labelTexture);
}

void GameLevel::SetUpSound(Mix_Chunk *collision) {
    platformGame.collision = collision;
}

void GameLevel::Update(float elapsed) {
    switch (mode) {
        case STATE_MAIN_MENU:
            multiplayer = menu.Update();
            platformGame.multiplayer = multiplayer;
            break;
        case STATE_GAME_START:
            platformGame.Update(elapsed);
            break;
        default:
            break;
    }
}

void GameLevel::ProcessInput(SDL_Event &event) {
    switch (mode) {
        case STATE_MAIN_MENU:
            mode = menu.ProcessInput(event) ? STATE_GAME_START : STATE_MAIN_MENU;
            break;
        case STATE_GAME_START:
            platformGame.ProcessInput(event);
            if (platformGame.quitGame) {
                mode = STATE_GAME_OVER;
            }
            break;
        default:
            break;
    }
}

void GameLevel::Render(ShaderProgram &program) {
    switch (mode) {
        case STATE_MAIN_MENU:
            menu.Render(program);
            break;
        case STATE_GAME_START:
            platformGame.Render(program);
            break;
        default:
            break;
    }
}

GameMode GameLevel::getGameMode() {
    return mode;
}
