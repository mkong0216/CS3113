//
//  GameLevel.cpp
//  NYUCodebase
//
//  Created by Mandy Kong on 10/21/18.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#include "GameLevel.hpp"
#include "GameState.hpp"
#include "ShaderProgram.h"
#include "SheetSprite.hpp"
#include "utilities.hpp"
#define MAX_BULLETS 30

glm::mat4 gameMatrix = glm::mat4(1.0f);
int bulletIndex = 0;
int winScore = 10 * (10 + 20 + 30);

float deltaEnemyPosX = 0.0f;
float deltaEnemyPosY = 0.0f;
float playerDirectionX = 0.0f;
float enemyDirectionX = 1.0f;

// Position of "Play Again?" in { left, right, top, bottom }
float playAgainPos[] = { -0.55f, 0.55f, 0.05f, -0.09f };
float playAgainFontSize = 0.2f;
float playAgainFontSpacing = -0.1f;

enum GameLevelMode { STATE_GAME_CONTINUE, STATE_GAME_WIN, STATE_GAME_LOSE };
GameLevelMode levelMode = STATE_GAME_CONTINUE;

void shootBullets(GameState &state) {
    state.bullets[bulletIndex].position = state.player.position;
    bulletIndex++;
    
    if (bulletIndex > MAX_BULLETS - 1) {
        bulletIndex = 0;
    }
}

void GameLevel::ProcessInput(SDL_Event &event, float elapsed) {
    playerDirectionX = 0.0f;
    if (event.type == SDL_KEYDOWN) {
        // When pressing both SPACE and either LEFT or RIGHT, only shootBullets gets called?
        if (event.key.keysym.sym == SDLK_SPACE) {
            shootBullets(state);
        }

        if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT) {
            playerDirectionX = (event.key.keysym.sym == SDLK_RIGHT) ? 1.0f : -1.0f;
        }
    } else if (event.type == SDL_KEYUP) {
        playerDirectionX = 0.0f;
    } else if ((event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN)) {
        float mouseX = (event.type == SDL_MOUSEBUTTONDOWN) ? event.button.x : event.motion.x;
        float mouseY = (event.type == SDL_MOUSEBUTTONDOWN) ? event.button.y : event.motion.y;
        
        float unitX = (((float)mouseX / 640.0f) * 3.554f ) - 1.777f;
        float unitY = (((float)(360-mouseY) / 360.0f) * 2.0f ) - 1.0f;
        
        // Increase font size of "Play Again?" text when hovered over text
        if (unitX >= playAgainPos[0] && unitX <= playAgainPos[1] && unitY <= playAgainPos[2] && unitY >= playAgainPos[3]) {
            playAgainFontSize = 0.25f;
            playAgainFontSpacing = -0.15f;
            
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                // If "Play Again?" is selected, reset game state.
                state.resetGame();
                levelMode = STATE_GAME_CONTINUE;
            }
        } else {
            playAgainFontSize = 0.20f;
            playAgainFontSpacing = -0.1f;
        }
    }
}

void updateEnemyPositions (Entity enemies[], float elapsed) {
    if (enemies[4].position[0] >= 1.65f) {
        enemyDirectionX = -1.0f;
    } else if (enemies[0].position[0] <= -1.65f) {
        enemyDirectionX = 1.0f;
    }
    
    bool hitEdge = (enemies[4].position[0] >= 1.65f || enemies[0].position[0] <= -1.65f);

    for (int i = 0; i < 30; i++) {
        enemies[i].position[0] += (elapsed * enemies[i].velocity[0] * enemyDirectionX);
        if (hitEdge) {
            enemies[i].position[1] += (elapsed * enemies[i].velocity[1]);
        }
    }
}

void updatePlayerScore (GameState &state) {
    // Checking if any of the bullets collided with an enemy sprite.
    for (int i = 0; i < bulletIndex; i++) {
        for (int j = 0; j < 30; j++) {
            if (state.enemies[j].points != 0 && state.collisionCheck(state.bullets[i], state.enemies[j])) {
                // Updating score, making bullet disappear, and changing collided enemy sprite's points to 0.
                state.score += state.enemies[j].points;
                state.bullets[i].position = glm::vec3(-2000.0f, 0.0f, 0.0f);
                state.enemies[j].points = 0;
            }
        }
    }
    
    // If all enemy sprites were hit, change mode to STATE_GAME_WIN
    if (state.score == winScore) {
        levelMode = STATE_GAME_WIN;
    }
}

void checkIfGameOver (GameState &state) {
    for (int i = 0; i < 30; i++) {
        if (state.enemies[i].points != 0 && state.collisionCheck(state.endLine, state.enemies[i])) {
            levelMode = STATE_GAME_LOSE;
        }
    }
}

void updatePlayerPos (Entity &player, float elapsed) {
    float playerPosX = player.position[0] + (player.velocity[0] * elapsed * playerDirectionX);
    if (playerPosX >= -1.7f && playerPosX <= 1.7f) {
        player.position[0] = playerPosX;
    }
}

void updateBullets (Entity bullets[], float elapsed) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].position[1] += bullets[i].velocity[1] * elapsed;
    }
}

void GameLevel::Update(float elapsed) {
    checkIfGameOver(state);
    updatePlayerPos(state.player, elapsed);
    updateBullets(state.bullets, elapsed);
    updateEnemyPositions(state.enemies, elapsed);
    updatePlayerScore(state);
}

void drawEnemySprites(Entity enemies[], ShaderProgram &program) {
    for (int i = 0; i < 30; i++) {
        if (enemies[i].points != 0) {
            enemies[i].Draw(program);
        }
    }
}

void drawBullets (Entity bullets[], ShaderProgram &program) {
    glUseProgram(program.programID);
    
    for (int i = 0; i < MAX_BULLETS; i++) {
        gameMatrix = glm::mat4(1.0f);
        gameMatrix = glm::translate(gameMatrix, bullets[i].position);
        program.SetModelMatrix(gameMatrix);
        
        float vertices[] = {-0.02f, -0.05f, 0.0f, 0.05f, -0.02f, 0.05f, -0.02f, -0.05f, 0.0f, -0.05f, 0.0f, 0.05f};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
        glEnableVertexAttribArray(program.positionAttribute);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(program.positionAttribute);
    }
}

void renderLine (ShaderProgram &program, Entity &endLine) {
    gameMatrix = glm::mat4(1.0f);
    gameMatrix = glm::translate(gameMatrix, endLine.position);
    program.SetModelMatrix(gameMatrix);
    
    float vertices[] = {-1.77f, -0.01f, 1.77f, 0.01f, -1.77f, 0.01f, -1.77f, -0.01f, 1.77f, -0.01f, 1.77f, 0.01f };
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program.positionAttribute);
}

void renderScore(GameState &state, ShaderProgram &program) {
    gameMatrix = glm::mat4(1.0f);
    gameMatrix = glm::translate(gameMatrix, glm::vec3(-1.7f, 0.85f, 0.0f));
    program.SetModelMatrix(gameMatrix);
    std::string score = "SCORE: " + std::to_string(state.score);
    DrawText(program, state.fontTexture, score, 0.2f, -0.1f);
}

void renderEndScreen (ShaderProgram &program, int fontTexture) {
    std::string label = (levelMode == STATE_GAME_WIN) ? "You Won!" : "Game Over!";
    
    gameMatrix = glm::mat4(1.0f);
    gameMatrix = glm::translate(gameMatrix, glm::vec3(-0.5f, 0.25f, 0.0f));
    program.SetModelMatrix(gameMatrix);
    DrawText(program, fontTexture, label, 0.2f, -0.1f);
    
    gameMatrix = glm::translate(gameMatrix, glm::vec3(0.0f, -0.25f, 0.0f));
    program.SetModelMatrix(gameMatrix);
    DrawText(program, fontTexture, "Play Again?", playAgainFontSize, playAgainFontSpacing);
}

void GameLevel::Render() {
    switch (levelMode) {
        case STATE_GAME_CONTINUE:
            state.player.Draw(texturedProgram);
            drawEnemySprites(state.enemies, texturedProgram);
            drawBullets(state.bullets, untexturedProgram);
            renderScore(state, texturedProgram);
            renderLine(untexturedProgram, state.endLine);
            break;
        case STATE_GAME_WIN:
        case STATE_GAME_LOSE:
            renderEndScreen(texturedProgram, state.fontTexture);
            break;
    }
}


