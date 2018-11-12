//
//  GameLevel.hpp
//  NYUCodebase
//
//  Created by Mandy Kong on 11/10/18.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#ifndef GameLevel_hpp
#define GameLevel_hpp

#include <stdio.h>
#include <SDL.h>
#include "ShaderProgram.h"
#include "SheetSprite.hpp"
#include "Entity.hpp"

enum GameState { STATE_GAME_START, STATE_GAME_WIN, STATE_GAME_LOSE, STATE_GAME_EXIT };

class GameLevel {
public:
    // Methods
    GameLevel();

    void SetUp();
    void ProcessInput(SDL_Event event);
    void Update(float elapsed);
    void Render(ShaderProgram &program);
    
    void CollisionCheckY();
    void CollisionCheckX();
    
    void updateScoreAndHealthPos(bool atLeftEdge, bool atRightEdge);
    void RenderGameEnd(ShaderProgram &program);

    // Attributes
    SheetSprite spritesheet;
    SheetSprite alphabet;
    
    // Game Entities
    Entity player;
    Entity building[10];
    Entity door[2];
    Entity key;
    Entity button;
    Entity enemy;
    Entity platforms[75];
    Entity coins[10];
    Entity blocks[6];

    Entity hearts[3];
    Entity score[2];
    
    Entity leftEdge;
    Entity rightEdge;
    
    // Game State
    GameState state;
};

#endif /* GameLevel_hpp */
