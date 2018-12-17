//
//  GameState.hpp
//  NYUCodebase
//
//  Created by Mandy Kong on 12/13/18.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#ifndef GameState_hpp
#define GameState_hpp

#include <stdio.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_opengl.h>
#include "ShaderProgram.h"
#include "utils.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "SheetSprite.hpp"
#include "Entity.hpp"

class GameState {
public:
    void SetUp(GLuint spriteTexture, GLuint fontTexture, GLuint playerOneArrow, GLuint playerTwoArrow, GLuint labelTexture);
    void setFloorBlocks();
    void setPlatformBlocks();
    void setPlayers(Entity &player, int playerIndex);
    void setEnemies();
    void setPlayerHearts(Entity hearts[]);
    void setCoinsAndButton();
    void setBuilding();
    void setDoor();
    void setNextLevel(int level);
    void resetGameEntities();

    void ProcessInput(SDL_Event event);

    void Update(float elapsed);
    void UpdatePlayerHeartsPos();

    void playAgain();

    void collisionCheck(Entity &player, float elapsed);

    void RenderPlayerChoices(ShaderProgram &program);
    void RenderGame(ShaderProgram &program);
    void RenderBlocks(ShaderProgram &program, Entity array[], int arraySize);
    
    void RenderGameOver(ShaderProgram &program);
    void RenderGameWin(ShaderProgram &program);

    void Render(ShaderProgram &program);

    bool multiplayer;
    bool playersChosen;
    bool quitGame;
    
    Mix_Chunk *collision;

private:
    int playerOneIndex;
    int playerTwoIndex;
    int playerWin;
    int level;

    Entity playerOne;
    Entity playerOneHearts[3];
    Entity playerTwo;
    Entity playerTwoHearts[3];

    Entity floor[80];
    Entity platforms[6];
    Entity coins[10];
    Entity button;
    Entity key;
    Entity building[10];
    Entity door[2];

    Entity fish;
    Entity worm;
    Entity icicle;
    Entity snow;

    Entity leftEdge;
    Entity rightEdge;
    
    Entity labelSign;
    
    SheetSprite sprites;
    GLuint alphabet;
    GLuint selectPlayerOne;
    GLuint selectPlayerTwo;
};

#endif /* GameState_hpp */
