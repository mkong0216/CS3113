//
//  GameLevel.cpp
//  NYUCodebase
//
//  Created by Mandy Kong on 11/10/18.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#include "GameLevel.hpp"

const int NUM_TILES_WIDTH = 25;
const int platformArraySize = 75;
const int coinsArraySize = 10;
const int buildingArraySize = 10;
const int blocksArraySize = 6;
const int numbers[] = { 434, 435, 436, 438, 439, 463, 464, 465, 466 };

glm::mat4 viewMatrixPlayer = glm::mat4(1.0f);

float exitPos[] = { -0.55f, 0.366f, -0.11f, -0.21f };
float exitFontSize = 0.2f;;
float exitFontSpacing = -0.1f;

GameLevel::GameLevel() {
    state = STATE_GAME_START;
//    state = STATE_GAME_WIN;
}

void GameLevel::SetUp() {
    // Set up game entities - player, door, enemies, platforms
    // Key appears after hitting button
    // Button appears after hitting specific block
    
    // Player
    player.entityType = ENTITY_PLAYER;
    player.isStatic = false;
    player.collidedBottom = true;
    
    // Player health
    float heartPosX = -1.6f;
    for (int i = 0; i < 3; i++) {
        Entity heart(ENTITY_ITEM, true);

        heart.position = glm::vec3(heartPosX, 1.2f, 0.0f);
        heart.scale = glm::vec3(0.3f, 0.3f, 0.0f);
        heart.size = glm::vec3(0.3f, 0.3f, 0.0f);
        heart.animationIndex = 373;
        
        heartPosX += 0.3f;
        hearts[i] = heart;
    }
    
    // Player coin score
    float digitPosX = 1.4f;
    for (int i = 0; i < 2; i++) {
        Entity digit(ENTITY_ITEM, true);
        digit.position = glm::vec3(digitPosX, 1.2f, 0.0f);
        digit.scale = glm::vec3(0.3f, 0.3f, 0.0f);
        digit.size = glm::vec3(0.3f, 0.3f, 0.0f);
        digit.animationIndex = numbers[0];
        
        digitPosX += 0.2f;
        score[i] = digit;
    }
    
    // Edge
    leftEdge.position = glm::vec3(-1.877f, 0.0f, 0.0f);
    leftEdge.size = glm::vec3(0.1f, 2.0f, 0.0f);
    rightEdge.position = glm::vec3(7.3f, 0.0f, 0.0f);
    rightEdge.size = glm::vec3(0.1f, 2.0f, 0.0f);

    // Platforms
    float posX = -1.7f;
    float posY = -0.04f;

    // Game Floor
    int grassTopIndex = 123;
    int grassBottomIndex = 152;

    // Pitfall 1
    int waterTopIndex = 10;
    int waterBottomIndex = 40;

    for (int i = 0; i < platformArraySize; i++) {
        Entity block(ENTITY_PLATFORM, true);
        
        int index = (i < NUM_TILES_WIDTH) ? grassTopIndex : grassBottomIndex;
        
        // Setting pitfall 1
        if (i > 6 && i < 9) {
            index = waterTopIndex;
        } else if (i > 31 && i < 34) {
            index = waterBottomIndex;
        }

        // Next row
        if (i % NUM_TILES_WIDTH == 0 && i != 0) {
            posX = -1.7f;
            posY -= 0.3612f;
        }

        block.position = glm::vec3(posX, posY, 0.0f);
        block.animationIndex = index;
        posX += 0.365f;
        platforms[i] = block;
    }
    
    // Blocks
    posX = platforms[10].position[0];
    posY = 0.8f;
    
    for (int i = 0; i < blocksArraySize; i++) {
        Entity block(ENTITY_PLATFORM, true);
        if (i % 4 == 0) {
            block.itemType = ITEM_COIN;
            block.animationIndex = 133;
        } else {
            block.animationIndex = (i == blocksArraySize - 1) ? 161 : 193;
        }
        
        if (i % 3 == 0 && i != 0) {
            posX += 1.5f;
        } else {
            posX += 0.315f;
        }
        
        block.size = glm::vec3(0.35f, 0.35f, 0.0f);
        block.scale = glm::vec3(0.35f, 0.35f, 0.0f);
        block.position = glm::vec3(posX, posY, 0.0f);
        
        blocks[i] = block;
    }
    
    // Button
    button.entityType = ENTITY_ITEM;
    button.itemType = ITEM_BUTTON;
    button.animationIndex = 105;
    button.position = blocks[5].position;
    button.size = glm::vec3(0.3f, 0.3f, 0.0f);
    button.scale = glm::vec3(0.3f, 0.3f, 0.0f);
    
    // Key
    key.entityType = ENTITY_ITEM;
    key.itemType = ITEM_KEY;
    key.animationIndex = -1;
    key.position = glm::vec3(button.position[0] + 0.4f, 3.0f, 0.0f);
    key.size = glm::vec3(0.3f, 0.3f, 0.0f);
    key.scale = glm::vec3(0.3f, 0.3f, 0.0f);

    // Enemies
    enemy.entityType = ENTITY_ENEMY;
    enemy.isStatic = false;
    enemy.position = platforms[33].position - 0.15f;
    enemy.velocity = glm::vec3(0.0f, 4.5f, 0.0f);

    // Coins
    posX = platforms[6].position[0] + 0.15f;
    posY = 0.7f;

    for (int i = 0; i < coinsArraySize; i++) {
        Entity coin(ENTITY_ITEM, true);
        coin.itemType = ITEM_COIN;
        coin.size = glm::vec3(0.25f, 0.25f, 0.0f);
        coin.scale = glm::vec3(0.25f, 0.25f, 0.0f);
        coin.animationIndex = 177;
        
        // Positioning coins
        if (i == 3) {
            posY = 1.1f;
            posX = blocks[2].position[0] + 0.15f;
        }
        
        if (i != 0 && i != 3) {
            if (i < 3) {
                posY = (i % 2 == 1) ? posY + 0.2f : posY - 0.2f;
                posX += 0.4f;
            } else if (i < 8) {
                posY = (i < 6) ? posY + 0.2f : posY - 0.2f;
                posX += 0.3f;
            }
        }
        
        if (i < 8) {
            coin.position = glm::vec3(posX, posY, 0.0f);
        } else {
            int blockIndex = (i == 8) ? 0 : 4;
            coin.position = blocks[blockIndex].position;
        }

        coins[i] = coin;
    }
    
    // Building
    posX = platforms[NUM_TILES_WIDTH - 3].position[0];
    posY = 0.3f;
    int initialBuildingIndex = 750;
    int numRow = 0;
    for (int i = 0; i < buildingArraySize; i++) {
        Entity block(ENTITY_BUILDING, true);
        // Next row
        if (i != 0 && i % 3 == 0 && i < 9) {
            numRow++;
            posY += 0.3612f;
            posX = platforms[NUM_TILES_WIDTH - 3].position[0];
            if (numRow < 2) {
                initialBuildingIndex -= 30;
            } else {
                posX = platforms[NUM_TILES_WIDTH - 4].position[0];
                initialBuildingIndex = 783;
            }
        }

        block.position = glm::vec3(posX, posY, 0.0f);
        if (numRow < 2) {
            block.animationIndex = initialBuildingIndex + (i % 3);
        } else {
            block.animationIndex = (i % 3 == 0 && i < 9) ? initialBuildingIndex : initialBuildingIndex + 1;
        }
        
        posX += 0.365f;
        building[i] = block;
    }
    
    // Door
    for (int i = 0; i < 2; i++) {
        Entity block(ENTITY_ITEM, true);
        block.itemType = ITEM_DOOR;
        
        block.animationIndex = (i % 2 == 0) ? 730 : 700;
        block.position = (i % 2 == 0) ? building[1].position : building[4].position;
        door[i] = block;
    }
}

void GameLevel::ProcessInput(SDL_Event event) {
    if (state == STATE_GAME_START) {
        // Entity player is the only one that responds to events
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_LEFT]) {
            player.velocity[0] = -2.0f;
        } else if (keys[SDL_SCANCODE_RIGHT]) {
            player.velocity[0] = 2.0f;
        } else {
            player.velocity[0] = 0.0f;
        }
        
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_SPACE && player.collidedBottom) {
                player.velocity[1] = 3.5f;
            }
        }

        player.setCollisions();
        key.setCollisions();
    } else if (state == STATE_GAME_WIN || state == STATE_GAME_LOSE) {
        if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN) {
            float mouseX = (event.type == SDL_MOUSEBUTTONDOWN) ? event.button.x : event.motion.x;
            float mouseY = (event.type == SDL_MOUSEBUTTONDOWN) ? event.button.y : event.motion.y;
            
            float unitX = (((float)mouseX / 640.0f) * 3.554f ) - 1.777f;
            float unitY = (((float)(360-mouseY) / 360.0f) * 2.0f ) - 1.0f;
            
            // Increase font size of "Click to Play" text when hovered over text
            if (unitX >= exitPos[0] && unitX <= exitPos[1] && unitY <= exitPos[2] && unitY >= exitPos[3]) {
                exitFontSize = 0.20f;
                exitFontSpacing = -0.1f;
                
                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    state = STATE_GAME_EXIT;
                }
            } else {
                exitFontSize = 0.15f;
                exitFontSpacing = -0.05f;
            }
        }
    }

}

void GameLevel::CollisionCheckY() {
    // If player went passed the floor for some reason, bring back to top
    if (player.position[1] <= platforms[NUM_TILES_WIDTH].position[1]) {
        player.position = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    // Checking if player collided with any platforms
    // If hit water, send back to beginning.
    for (int i = 0; i < platformArraySize; i++) {
        if (player.collisionY(platforms[i]) && i < 7 && i > 8) {
            player.velocity[1] = (player.collidedBottom) ? 0.0f : -1.0f;
            break;
        }
        
        if (key.collisionY(platforms[i])) {
            key.velocity[1] = (key.collidedBottom) ? 0.0f : -1.0f;
            key.velocity[1] = 0.0f;
        }
    }
    
    // Checking if player collided with any blocks
    for (int i = 0; i < blocksArraySize; i++) {
        if (player.collisionY(blocks[i])) {
            player.velocity[1] = (player.collidedBottom) ? 0.0f : -1.0f;
            if (player.collidedTop) {
                switch (blocks[i].itemType) {
                    case ITEM_NONE:
                        break;
                    case ITEM_COIN:
                        // Checking if collided block has coin
                        if (i == 0) {
                            coins[8].position[1] += 0.3f;
                            blocks[i].animationIndex += 30;
                            blocks[i].itemType = ITEM_NONE;
                        } else if (i == 4) {
                            coins[9].position[1] += 0.3f;
                            blocks[i].animationIndex += 30;
                            blocks[i].itemType = ITEM_NONE;
                        }
                        
                        break;
                    case ITEM_BUTTON:
                        // Checking if collided block has a button
                        button.position[1] += 0.3f;
                        blocks[i].animationIndex += 30;
                        blocks[i].itemType = ITEM_NONE;
                    default:
                        break;
                }
            }
        }
    }

    // Checking if fish collided with water
    if (enemy.collisionY(platforms[58]) && enemy.collidedBottom) {
        enemy.animationIndex = 0;
        enemy.velocity[1] = 3.5f;
    }
    
    // Checking if player collided with fish
    if (player.CollidesWith(enemy)) {
        for (int i = 0; i < 3; i++) {
            hearts[i].position[0] -= player.position[0];
        }
        
        player.velocity[1] = 0.0f;
        player.position = glm::vec3(0.0f, 0.0f, 0.0f);
        player.health -= 1;
        hearts[player.health].animationIndex = 375;
    }
    
    // Checking if player collided with coins
    for (int i = 0; i < coinsArraySize; i++) {
        if (coins[i].animationIndex != -1 && player.CollidesWith(coins[i])) {
            player.numCoins++;
            if (player.numCoins + 1 == 10) {
                score[0].animationIndex = numbers[1];
                score[1].animationIndex = numbers[0];
            } else {
                score[1].animationIndex = numbers[player.numCoins];
            }

            coins[i].animationIndex = -1;
            if (player.numCoins == 5) {
                blocks[5].itemType = ITEM_BUTTON;
                blocks[5].animationIndex = 131;
            }
        }
    }
    
    // Checking if player collided with button
    if (player.collisionY(button) && player.collidedBottom && button.animationIndex != 165) {
        button.animationIndex = 165;
        key.animationIndex = 45;
        key.velocity[1] = -2.0f;
    }
}

void GameLevel::CollisionCheckX() {
    // Checking if player collided with edge of screen
    if (player.collisionX(leftEdge) || player.collisionX(rightEdge)) {
        player.velocity[0] = 0.0f;
    }
}

void GameLevel:: updateScoreAndHealthPos (bool atLeftEdge, bool atRightEdge) {
    for (int i = 0; i < 3; i++) {
        float posY = player.position[1] + 0.85f;
        if (i < 2) {
            float digitPosX = player.position[0] + 1.4 + (i * 0.2f);
            if (atLeftEdge) {
                digitPosX = 1.4 + (i * 0.2f);
            } else if (atRightEdge) {
                digitPosX = 5.45f + 1.4 + (i * 0.2f);
            }

            score[i].position = glm::vec3(digitPosX, posY, 0.0f);
        }
        
        float heartPosX = player.position[0] - 1.6 + (i * 0.3);
        if (atLeftEdge) {
            heartPosX = -1.6f + (i * 0.3f);
        } else if (atRightEdge) {
            heartPosX = 5.45 - 1.6f + (i * 0.3f);
        }

          hearts[i].position = glm::vec3(heartPosX, posY, 0.0f);
    }
}

void GameLevel::Update(float elapsed) {
    if (player.health == 0) {
        state = STATE_GAME_LOSE;
    } else if (door[0].animationIndex == 731) {
        state = STATE_GAME_WIN;
    }

    // Updating game entities
    enemy.Update(elapsed);
    key.Update(elapsed);
    key.position[1] += key.velocity[1] * elapsed;

    // Checking player collisions by updating positions here
    player.Update(elapsed);
    player.position[1] += player.velocity[1] * elapsed;
    CollisionCheckY();
    player.position[0] += player.velocity[0] * elapsed;
    CollisionCheckX();

    if (player.CollidesWith(key)) {
        player.hasKey = true;
        key.velocity[1] = 0.0f;
        key.animationIndex = -1;
    }

    if ((player.CollidesWith(door[0]) || player.CollidesWith(door[1])) && player.hasKey) {
        door[0].animationIndex = 731;
        door[1].animationIndex = 701;
    }

    // Update view matrix to follow player
    viewMatrixPlayer = glm::mat4(1.0f);
    if (player.position[0] < 0.0f || player.position[0] > 5.45f) {
        bool atLeftEdge = (player.position[0] < 0.0f);
        float deltaPosX = (atLeftEdge) ? 0.0f : -5.45f;
        viewMatrixPlayer = glm::translate(viewMatrixPlayer, glm::vec3(deltaPosX, -player.position[1], 0.0f));
        updateScoreAndHealthPos(atLeftEdge, !atLeftEdge);
    } else {
        viewMatrixPlayer = glm::translate(viewMatrixPlayer, glm::vec3(-player.position[0], -player.position[1], 0.0f));
        updateScoreAndHealthPos(false, false);
    }
}

void GameLevel::RenderGameEnd (ShaderProgram &program) {
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.75f, 0.25f, 0.0f));
    program.SetModelMatrix(modelMatrix);
    
    std::string text;
    if (state == STATE_GAME_LOSE) {
        text = "GAME OVER";
    } else if (state == STATE_GAME_WIN) {
        text = "YOU WON!";
    }

    alphabet.DrawText(program, alphabet.textureID, text, 0.3, -0.1f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.25f, -0.4f, 0.0f));
    program.SetModelMatrix(modelMatrix);
    
    alphabet.DrawText(program, alphabet.textureID, "exit game", exitFontSize, exitFontSpacing);
}

void GameLevel::Render(ShaderProgram &program) {
    // Set view matrix
    program.SetViewMatrix(viewMatrixPlayer);
    
    // Render game entities

    // Hide button behind a block
    button.Render(program, spritesheet);
    
    // If button is hit, render key
    if (key.animationIndex != -1) {
        key.Render(program, spritesheet);
    }

    // Rendering platforms
    for (int i = 0; i < platformArraySize; i++) {
        platforms[i].Render(program, spritesheet);
    }
    
    // Rendering coins
    for (int i = 0; i < coinsArraySize; i++) {
        if (coins[i].animationIndex != -1) {
            coins[i].Render(program, spritesheet);
        }
    }
    
    // Rendering building
    for (int i = 0; i < buildingArraySize; i++) {
        building[i].Render(program, spritesheet);
    }
    
    // Rendering building door
    for (int i = 0; i < 2; i++) {
        door[i].Render(program, spritesheet);
    }
    
    // Rendering blocks
    for (int i = 0; i < blocksArraySize; i++) {
        blocks[i].Render(program, spritesheet);
    }
    
    // Rendering player health
    for (int i = 0; i < 3; i++) {
        hearts[i].Render(program, spritesheet);
    }
    
    for (int i = 0; i < 2; i++) {
        score[i].Render(program, spritesheet);
    }

    // Render player
    player.Render(program, spritesheet);
    
    // Render enemy
    enemy.Render(program, spritesheet);
}
