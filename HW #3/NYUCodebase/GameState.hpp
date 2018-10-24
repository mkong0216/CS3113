//
//  GameState.hpp
//  NYUCodebase
//
//  Created by Mandy Kong on 10/21/18.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#ifndef GameState_hpp
#define GameState_hpp

#include <stdio.h>
#include "Entity.hpp"
#include "utilities.hpp"

class GameState {
public:
    Entity player;
    Entity enemies[30];
    Entity bullets[30];
    Entity endLine;
    int score;

    SpriteDimensions spaceInvaders;
    GLuint fontTexture;
    GLuint spritesheet;
    
    void SetUpEnemies(int spritesheet);
    void SetUpPlayer(int spritesheet);
    void SetUpBullets();
    void SetUpEndLine();

    void resetGame();
    bool collisionCheck(Entity &object1, Entity &object2);
};

#endif /* GameState_hpp */
