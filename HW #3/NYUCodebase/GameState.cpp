//
//  GameState.cpp
//  NYUCodebase
//
//  Created by Mandy Kong on 10/21/18.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#include "GameState.hpp"
#include "Entity.hpp"
#include "SheetSprite.hpp"
#include "utilities.hpp"

#define MAX_BULLETS 30

void setEnemyPositions (Entity enemies[]) {
    float spaceBetweenX = 0.4f;
    float spaceBetweenY = 0.03f;
    float posX = -1.3f;
    float posY = -0.2f;

    for (int i = 0; i < 30; i++) {
        if (i % 5 == 0) {
            posX = -1.3f;
            posY += enemies[i-1].sprite.height + spaceBetweenY;
        }
        
        posX += spaceBetweenX;
        enemies[i].position = glm::vec3(posX, posY, 0.0f);
    }
}

// Object pooling the enemy sprites.
void GameState::SetUpEnemies(int spritesheet) {

    for (int i = 0; i < 30; i++) {
        Entity enemySprite;
        // Three types of enemies, include 10 of each.
        int enemyType = floor(i / 10);
        
        // Last enemy sprite is slightly smaller than other enemy sprites,
        // need to adjust accordingly.
        if (enemyType == 2) {
            spaceInvaders.spriteWidth = 100.0f;
            spaceInvaders.spriteHeight = 110.0f;
        }
        
        enemySprite.sprite = SheetSprite(spritesheet, enemyType / spaceInvaders.sheetWidth, enemyType * spaceInvaders.spriteHeight / spaceInvaders.sheetHeight, spaceInvaders.spriteWidth / spaceInvaders.sheetWidth, spaceInvaders.spriteHeight / spaceInvaders.sheetHeight, 0.15f);
        enemySprite.points = (enemyType + 1) * 10;
        enemySprite.size = glm::vec3(enemySprite.sprite.width, enemySprite.sprite.height, 0.0f);
        enemySprite.velocity = glm::vec3(0.3f, -2.0f, 0.0f);
        enemies[i] = enemySprite;
    }
    
    setEnemyPositions(enemies);
    spaceInvaders.resetSpriteDimensions();
}

// Setting up the player shooter.
void GameState::SetUpPlayer(int spritesheet) {
    player.sprite = SheetSprite(spritesheet, spaceInvaders.spriteWidth / spaceInvaders.sheetWidth, (spaceInvaders.spriteHeight * 6.25) / spaceInvaders.sheetHeight, 90.0f / spaceInvaders.sheetWidth, 70.0f / spaceInvaders.sheetHeight, 0.2f);
    
    player.size = glm::vec3(player.sprite.width, player.sprite.height, 0.0f);
    player.position = glm::vec3(0.0f, -0.8f, 0.0f);
    player.velocity = glm::vec3(3.0f, 0.0f, 0.0f);
}

// Setting up bullets array and initializing position all the way to left of screen.
void GameState::SetUpBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].position = glm::vec3(-2000.0f, player.position[1], 0.0f);
        bullets[i].size = glm::vec3(0.02f, 0.1f, 0.0f);
        bullets[i].velocity = glm::vec3(0.0f, 1.0f, 0.0f);
    }
}

void GameState::SetUpEndLine() {
    endLine.position = glm::vec3(0.0f, -0.6f, 0.0f);
    endLine.size = glm::vec3(1.77f * 2, 0.02f, 0.0f);
}

// Checks if object1 collided with object2.
bool GameState::collisionCheck(Entity &object1, Entity &object2) {
    float distanceX = fabs(object1.position[0] - object2.position[0]) - ((object1.size[0] + object2.size[0]) / 2);
    float distanceY = fabs(object1.position[1] - object2.position[1]) - ((object1.size[1] + object2.size[1]) / 2);

    return (distanceX < 0 && distanceY < 0);
}

// When resetting the game, reinitialize the enemy sprite points and the player score.
void GameState::resetGame() {
    score = 0;
    setEnemyPositions(enemies);
    for (int i = 0; i < 30; i++) {
        int enemyType = floor(i / 10);
        enemies[i].points = (enemyType + 1) * 10;
    }
}