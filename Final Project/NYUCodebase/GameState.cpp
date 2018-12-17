//
//  GameState.cpp
//  NYUCodebase
//
//  Created by Mandy Kong on 12/13/18.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#include "GameState.hpp"

const int NUM_TILES_WIDTH = 20;
glm::mat4 viewMatrixPlayer = glm::mat4(1.0f);
glm::vec3 quitButton = glm::vec3(-0.0f, 0.0f, 0.0f);
glm::vec3 playButton = glm::vec3(0.0f, 0.0f, 0.0f);

int playerCoins[5] = { 169, 170, 171, 172, 173 };
int playerIndices[5] = { 19, 49, 79, 109, 139 };

void GameState::setFloorBlocks() {
    int grassTopIndex = 123;
    int grassBottomIndex = 152;
    
    int waterTopIndex = 10;
    int waterBottomIndex = 40;

    float posX = -1.777f;
    float posY = -0.57f;

    for (int i = 0; i < 80; i++) {
        Entity block;
        block.entityType = ENTITY_PLATFORM;
        block.sprite = sprites;
        
        int index = (i < NUM_TILES_WIDTH) ? grassTopIndex : grassBottomIndex;

        if (i % NUM_TILES_WIDTH == 0 && i != 0) {
            posX = -1.777f;
            posY -= 0.535f;
        }
        
        block.position = glm::vec3(posX, posY, 0.0f);
        
        if (i > 6 && i < 9) {
            block.entityType = ENTITY_PITFALL;
            index = waterTopIndex;
        } else if (i > 26 && i < 29) {
            index = waterBottomIndex;
        }

        block.animationIndex = index;
        floor[i] = block;
        posX += 0.535f;
    }
}

void GameState::setPlatformBlocks() {
    float posX = 3.5f;
    float posY = 0.6f;

    for (int i = 0; i < 6; i++) {
        Entity block;
        block.entityType = ENTITY_PLATFORM;
        block.sprite = sprites;
        block.size = glm::vec3(0.45f, 0.45f, 0.0f);
        block.animationIndex = 193;
        block.position = glm::vec3(posX, posY, 0.0f);
        
        if (i % 4 == 0) {
            block.item = ITEM_COIN;
            block.animationIndex = 133;
        } else if (i == 5) {
            block.item = ITEM_BUTTON;
            block.animationIndex = 161;
        }

        platforms[i] = block;
        
        if (i % 3 == 0 && i != 0) {
            posX += 1.5f;
        } else {
            posX += 0.40f;
        }
    }
}

void GameState::setPlayerHearts(Entity hearts[]) {
    float heartPosX = -1.7f;
    for (int i = 0; i < 3; i++) {
        Entity heart;
        heart.entityType = ENTITY_PLATFORM;
        heart.sprite = sprites;
        heart.size = glm::vec3(0.3f, 0.3f, 0.0f);
        heart.animationIndex = 373;
        heart.position = glm::vec3(heartPosX, 0.85f, 0.0f);
        hearts[i] = heart;
        heartPosX += 0.3f;
    }
}

void GameState::setEnemies() {
    fish.entityType = ENTITY_ENEMY;
    fish.sprite = sprites;
    fish.isStatic = false;
    fish.position = glm::vec3(floor[28].position[0] - 0.2f, floor[28].position[1] + 0.2f, 0.0f);
    fish.velocity = glm::vec3(0.0f, 4.5f, 0.0f);
    fish.animationIndex = 410;
    
    worm.entityType = ENTITY_ENEMY_MOVE;
    worm.sprite = sprites;
    worm.isStatic = false;
    worm.position = glm::vec3(floor[28].position[0], 0.0f, 0.0f);
    worm.velocity = glm::vec3(-1.0f, 0.0f, 0.0f);
    worm.animationIndex = -1;
    worm.walkingIndices = { 229, 230, 231 };
}

void GameState::setCoinsAndButton() {
    float coinPosX = floor[6].position[0] + 0.1f;
    float coinPosY = 0.45f;

    for (int i = 0; i < 10; i++) {
        Entity coin;
        coin.entityType = ENTITY_ITEM;
        coin.item = ITEM_COIN;
        coin.sprite = sprites;
        coin.size = glm::vec3(0.3f, 0.3f, 0.0f);
        coin.animationIndex = 179;
        coin.position = glm::vec3(coinPosX, coinPosY, 0.0f);
        
        if (i < 2) {
            coinPosY += 0.15f;
        } else if (i < 4) {
            coinPosY -= 0.15f;
        } else if (i == 4) {
            coinPosX = platforms[3].position[0] + 0.05;
            coinPosY = platforms[3].position[1] + 0.4f;
        } else if (i < 6) {
            coinPosY += 0.2f;
        } else if (i == 6) {
            coinPosY -= 0.2f;
        } else {
            int index = (i == 7) ? 0 : 4;
            coinPosX = platforms[index].position[0] - 0.35f;
            coinPosY = platforms[index].position[1];
        }

        coins[i] = coin;
        coinPosX += 0.35f;
    }
    
    button.entityType = ENTITY_ITEM;
    button.sprite = sprites;
    button.item = ITEM_BUTTON;
    button.animationIndex = 105;
    button.position = platforms[5].position;
    button.size = glm::vec3(0.3f, 0.3f, 0.0f);
    
    key.entityType = ENTITY_ITEM;
    key.item = ITEM_KEY;
    key.sprite = sprites;
    key.animationIndex = -1;
    key.position = glm::vec3(button.position[0] + 0.4f, 3.0f, 0.0f);
}

void GameState::setBuilding() {
    float buildingPosX = floor[NUM_TILES_WIDTH - 2].position[0];
    float buildingPosY = -0.1f;
    int initialBuildingIndex = 750;
    int numRow = 0;
    for (int i = 0; i < 10; i++) {
        Entity block;
        block.sprite = sprites;
        block.entityType = ENTITY_PLATFORM;
        block.size = glm::vec3(0.5f, 0.5f, 0.0f);
        // Next row
        if (i != 0 && i % 3 == 0 && i < 9) {
            numRow++;
            buildingPosY += 0.45f;
            buildingPosX = floor[NUM_TILES_WIDTH - 2].position[0];
            if (numRow < 2) {
                initialBuildingIndex -= 30;
            } else {
                buildingPosX = floor[NUM_TILES_WIDTH - 3].position[0];
                initialBuildingIndex = 783;
            }
        }
        
        block.position = glm::vec3(buildingPosX, buildingPosY, 0.0f);
        if (numRow < 2) {
            block.animationIndex = initialBuildingIndex + (i % 3);
        } else {
            buildingPosX += 0.08f;
            block.animationIndex = (i % 3 == 0 && i < 9) ? initialBuildingIndex : initialBuildingIndex + 1;
        }
        
        buildingPosX += 0.365f;
        building[i] = block;
    }
}

void GameState::setDoor() {
    for (int i = 0; i < 2; i++) {
        Entity block;
        block.entityType = ENTITY_DOOR;
        block.sprite = sprites;
        block.size = glm::vec3(0.5f, 0.5f, 0.0f);
        block.animationIndex = (i % 2 == 0) ? 730 : 700;
        block.position = (i % 2 == 0) ? building[1].position : building[4].position;
        door[i] = block;
    }
}

void GameState::SetUp(GLuint spriteTexture, GLuint fontTexture, GLuint playerOneArrow, GLuint playerTwoArrow, GLuint labelTexture) {
    SheetSprite gameSprites(30, 30, spriteTexture);
    sprites = gameSprites;
    level = 0;
    alphabet = fontTexture;

    playersChosen = false;
    quitGame = false;
    playerOneIndex = 19;
    playerTwoIndex = 19 + 30;
    
    selectPlayerOne = playerOneArrow;
    selectPlayerTwo = playerTwoArrow;
    
    // Setting up all game entities
    
    // 0) Setting up left and right edges and label sign
    SheetSprite label(1, 1, labelTexture);
    labelSign.entityType = ENTITY_LABEL;
    labelSign.showLabel = false;
    labelSign.isStatic = false;
    labelSign.position = glm::vec3(0.0f, 2.0f, 0.0f);
    labelSign.sprite = label;
    labelSign.animationIndex = -1;

    leftEdge.position = glm::vec3(-1.8f, 0.0f, 0.0f);
    leftEdge.size = glm::vec3(0.0f, 2.0f, 0.0f);
    rightEdge.position = glm::vec3(8.5f, 0.0f, 0.0f);
    rightEdge.size = glm::vec3(0.0f, 2.0f, 0.0f);
    // 1) Setting up the floor
    setFloorBlocks();
    // 2) Setting up platforms for player(s) to jump on
    setPlatformBlocks();
    // 3) Setting up fish enemy
    setEnemies();
    // 4) Setting up player's health hearts
    setPlayerHearts(playerOneHearts);
    setPlayerHearts(playerTwoHearts);
    // 5) Setting up coins
    setCoinsAndButton();
    // 6) Setting up building
    setBuilding();
    // 7) Setting up door of building
    setDoor();
    
}

void GameState::playAgain() {
    resetGameEntities();
    level = 1;
    playerOne.health = 3;
    playerTwo.health = 3;
    playerOne.numCoins = 0;
    playerTwo.numCoins = 0;
    playerOne.hasKey = false;
    playerTwo.hasKey = false;

    // 1) Setting up the floor
    setFloorBlocks();
    // 2) Setting up platforms for player(s) to jump on
    setPlatformBlocks();
    // 3) Setting up fish enemy
    setEnemies();
    // 4) Setting up player's health hearts
    setPlayerHearts(playerOneHearts);
    setPlayerHearts(playerTwoHearts);
    // 5) Setting up coins
    setCoinsAndButton();
    // 6) Setting up building
    setBuilding();
    // 7) Setting up door of building
    setDoor();
}

void GameState::resetGameEntities() {
    // reset floor
    for (int i = 0; i < 80; i++) {
        floor[i].animationIndex = -1;
        floor[i].entityType = ENTITY_PLATFORM;
    }
    
    // reset platform blocks
    for (int i = 0; i < 6; i++) {
        platforms[i].animationIndex = -1;
        platforms[i].entityType = ENTITY_PLATFORM;
        platforms[i].item = ITEM_NONE;
    }
    
    // reset fish
    fish.animationIndex = -1;
    worm.animationIndex = -1;
    snow.animationIndex = -1;
    icicle.animationIndex = -1;

    //reset coins, button, and ket
    for (int i = 0; i < 10; i++) {
        coins[i].animationIndex = -1;
    }
    button.animationIndex = -1;
    key.animationIndex = -1;
    
    // reset building
    for (int i = 0; i < 10; i++) {
        building[i].animationIndex = -1;
    }
    
    // reset building door
    for (int i = 0; i < 2; i++) {
        door[i].animationIndex = -1;
    }
    
    playerOne.hasKey = false;
    playerOne.position = glm::vec3(-1.4f, 0.0f, 0.0f);
    if (multiplayer) {
        playerTwo.position = glm::vec3(-0.9f, 0.0f, 0.0f);
        playerTwo.hasKey = false;
    }
    
    key.position = glm::vec3(button.position[0] + 0.4f, 3.0f, 0.0f);
}

void GameState::setNextLevel(int level) {
    resetGameEntities();

    switch (level) {
        case 2: {
            int sweetsTopIndex = 497;
            int sweetsBottomIndex = 526;
            
            // Updating floors
            for (int i = 0; i < 80; i++) {
                int index = (i < NUM_TILES_WIDTH) ? sweetsTopIndex : sweetsBottomIndex;
                floor[i].animationIndex = index;
            }
            
            // Updating building
            int buildingIndices[10] = { 586, 586, 586, 557, 557, 557, -1, 615, 616, 617 };
            for (int i = 0; i < 10; i++) {
                building[i].animationIndex = buildingIndices[i];
            }
            
            door[1].position[1] -= 0.15f;
            door[0].position[1] -= 0.05f;
            for (int i = 0; i < 2; i++) {
                door[i].size = glm::vec3(0.4f, 0.4f, 0.0f);
                door[i].animationIndex = 645 + i;
            }
            
            // Setting button
            button.entityType = ENTITY_ITEM;
            button.item = ITEM_BUTTON;
            button.position = platforms[5].position;
            button.animationIndex = 105;

            // Updating platform blocks
            for (int i = 0; i < 6; i++) {
                if (i < 5) {
                    platforms[i].animationIndex = 192;
                } else {
                    platforms[i].animationIndex = 161;
                    platforms[i].item = ITEM_BUTTON;
                }
            }
            
            // Setting player token blocks
            for (int i = 0; i < 4; i++) {
                coins[i].animationIndex = playerCoins[i];
                coins[i].position = platforms[i].position;
                platforms[i].item = ITEM_TOKEN;
            }
            
            worm.animationIndex = 0;
            labelSign.showLabel = true;
            labelSign.animationIndex = 0;
            break;
        }
        case 3: {
            int iceTopIndex = 483;
            int iceBottomIndex = 512;
            
            // Setting up floor
            for (int i = 0; i < 80; i++) {
                int index = (i < NUM_TILES_WIDTH) ? iceTopIndex : iceBottomIndex;
                floor[i].animationIndex = index;
                
                if (i > 6 && i < 11) {
                    floor[i].animationIndex = -1;
                    floor[i].entityType = ENTITY_PITFALL;
                } else if (i > 26 && i < 31) {
                    floor[i].animationIndex = 573;
                }
            }
            
            // Setting up building
            int buildingIndices[6] = { 583, 582, 581, 551, 552, 553 };
            for (int i = 0; i < 6; i++) {
                building[i].animationIndex = buildingIndices[i];
            }
            
            for (int i = 0; i < 3; i++) {
                int icePlatformIndices[3] = { 488, 518, 519 };
                platforms[i].entityType = ENTITY_PLATFORM;
                platforms[i].animationIndex = icePlatformIndices[i];
                platforms[i].position = glm::vec3(floor[26].position[0] + (0.3 * i), 0.45f, 0.0f);
                platforms[i].velocity[0] = 1.0f;
            }
            
            for (int i = 3; i < 6; i++) {
                platforms[i].animationIndex = 131;
                platforms[i].entityType = ENTITY_PLATFORM;
                platforms[i].item = (i % 3 == 1) ? ITEM_BUTTON : ITEM_ICICLE;
            }
            
            icicle.sprite = sprites;
            icicle.entityType = ENTITY_ITEM;
            icicle.item = ITEM_ICICLE;
            icicle.animationIndex = -1;
            icicle.position = glm::vec3(platforms[3].position[0], 3.0f, 0.0f);
            
            snow.sprite = sprites;
            snow.entityType = ENTITY_ITEM;
            snow.item = ITEM_ICICLE;
            snow.animationIndex = -1;
            snow.position = glm::vec3(platforms[5].position[0], 3.0f, 0.0f);

            button.position = platforms[4].position;
            button.entityType = ENTITY_ITEM;
            button.item = ITEM_BUTTON;
            button.animationIndex = 105;

            labelSign.showLabel = true;
            labelSign.animationIndex = 0;
            break;
        }
        default:
            break;
    }
}

void GameState::setPlayers(Entity &player, int playerIndex) {
    player.entityType = ENTITY_PLAYER;
    player.isStatic = false;
    player.collidedBottom = true;
    player.sprite = sprites;
    player.walkingIndices = { playerIndex + 1, playerIndex + 9, playerIndex + 10 };
}

void GameState::ProcessInput (SDL_Event event) {
    // Game started
    if (playersChosen) {
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_LEFT]) {
            playerOne.velocity[0] = -2.0f;
        } else if (keys[SDL_SCANCODE_RIGHT]) {
            playerOne.velocity[0] = 2.0f;
        } else {
            playerOne.velocity[0] = 0.0f;
        }
        
        if (multiplayer) {
            if (keys[SDL_SCANCODE_A]) {
                playerTwo.velocity[0] = -2.0f;
            } else if (keys[SDL_SCANCODE_D]) {
                playerTwo.velocity[0] = 2.0f;
            } else {
                playerTwo.velocity[0] = 0.0f;
            }
        }
        
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_UP:
                    if (playerOne.collidedBottom) {
                        playerOne.velocity[1] = 4.5f;
                    }
                    break;
                case SDL_SCANCODE_W:
                    if (multiplayer && playerTwo.collidedBottom) {
                        playerTwo.velocity[1] = 4.5f;
                    }
                    break;
                default:
                    break;
            }
        }
    }
    
    playerOne.resetCollisions();
    playerTwo.resetCollisions();

    // Selecting players
    if (event.type == SDL_KEYDOWN && !playersChosen) {
        switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_LEFT:
                if (playerOneIndex != 19) {
                    playerOneIndex -= 30;
                }
                break;
            case SDL_SCANCODE_RIGHT:
                if (playerOneIndex != 19 + 30 * 4) {
                    playerOneIndex += 30;
                }
                break;
            case SDL_SCANCODE_A:
                if (playerTwoIndex != 19) {
                    playerTwoIndex -= 30;
                }
                break;
            case SDL_SCANCODE_D:
                if (playerTwoIndex != 19 + 30 * 4) {
                    playerTwoIndex += 30;
                }
                break;
            case SDL_SCANCODE_SPACE:
                playersChosen = true;
                level = 1;
                labelSign.showLabel = true;
                labelSign.animationIndex = 0;

                playerOne.position = glm::vec3(-1.4f, 0.0f, 0.0f);
                setPlayers(playerOne, playerOneIndex);
                if (multiplayer) {
                    playerTwo.position = glm::vec3(-0.9f, 0.0f, 0.0f);
                    setPlayers(playerTwo, playerTwoIndex);
                }
                break;
            default:
                break;
        }
    }
    
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        float mouseX = (event.type == SDL_MOUSEBUTTONDOWN) ? event.button.x : event.motion.x;
        float mouseY = (event.type == SDL_MOUSEBUTTONDOWN) ? event.button.y : event.motion.y;
        
        float unitX = (((float)mouseX / 640.0f) * 3.554f ) - 1.777f;
        float unitY = (((float)(360-mouseY) / 360.0f) * 2.0f ) - 1.0f;
        
        if (unitX >= quitButton[0] - 0.6f && unitX <= quitButton[0] + 0.6f && unitY <= quitButton[1] + 0.2f && unitY >= quitButton[1] - 0.2f) {
            quitGame = true;
        } else if (playButton[1] != 0.0f && unitX >= playButton[0] - 0.6f && unitX <= playButton[0] + 0.6f && unitY <= playButton[1] + 0.2f && unitY >= playButton[1] - 0.2f) {
            playAgain();
            
        }
    }

}

void GameState::collisionCheck(Entity &player, float elapsed) {
    player.position[1] += player.velocity[1] * elapsed;
    
    // Checking collisions with platform blocks
    for (int i = 0; i < 6; i++) {
        if (platforms[i].animationIndex == -1) {
            continue;
        } else if (player.collisionY(platforms[i])) {
            player.velocity[1] = 0.0f;
            if (player.collidedTop && platforms[i].item != ITEM_NONE) {
                switch (platforms[i].item) {
                    case ITEM_COIN: {
                        int coinIndex = (i == 0) ? 8 : 9;
                        coins[coinIndex].position[1] += 0.4f;
                        platforms[i].animationIndex += 30;
                        platforms[i].item = ITEM_NONE;
                        break;
                    }
                    case ITEM_BUTTON:
                        button.position[1] += 0.35f;
                        platforms[i].animationIndex += 30;
                        platforms[i].item = ITEM_NONE;
                        break;
                    case ITEM_TOKEN:
                        coins[i].animationIndex += 1;
                        if (coins[i].animationIndex > 173) {
                            coins[i].animationIndex = playerCoins[0];
                        }
                        break;
                    case ITEM_ICICLE:
                        if (i == 3) {
                            icicle.animationIndex = 571;
                            icicle.velocity[1] = -2.0f;
                        } else if (i == 5) {
                            snow.animationIndex = 520;
                            snow.velocity[1] = -2.0f;
                        }

                        platforms[i].animationIndex += 30;
                        platforms[i].item = ITEM_NONE;

                    default:
                        break;
                }
            }
            break;
        }
    }
    
    // Checking collisions with floor
    for (int i = 0; i < 80; i++) {
        if (floor[i].animationIndex == -1) {
            continue;
        }

        if (player.collisionY(floor[i]) && floor[i].animationIndex != -1) {
            player.velocity[1] = 0.0f;
            
            if (level == 3 && i > 26 && i < 31 && player.health > 0) {
                player.health -= 1;
                player.position = glm::vec3(0.0f, 0.0f, 0.0f);

                Mix_VolumeChunk(collision, 20);
                Mix_PlayChannel(-1, collision, 0);
            }
            break;
        }
        
        if (key.collisionY(floor[i]) && floor[i].animationIndex != -1) {
            key.velocity[1] = 0.0f;
            break;
        }
        
        if (level == 3 && icicle.collisionY(floor[i]) && floor[i].animationIndex != -1) {
            icicle.velocity[1] = 0.0f;
            break;
        }
        
        if (level == 3 && snow.collisionY(floor[i]) && floor[i].animationIndex != -1) {
            snow.velocity[1] = 0.0f;
            snow.animationIndex = 521;
        }
    }
    
    // Checking collisions with coins
    for (int i = 0; i < 10; i++) {
        if (coins[i].animationIndex != -1 && player.collidesWith(coins[i])) {
            player.numCoins++;
            coins[i].animationIndex = -1;
        }
    }

    // Checking collisions with enemy fish
    if (level == 1 && player.health != 0 && player.collidesWith(fish) && fish.animationIndex != -1) {
        player.position = glm::vec3(0.0f, 0.0f, 0.0f);
        player.velocity[1] = 0.0f;
        player.health -= 1;
        
        Mix_VolumeChunk(collision, 20);
        Mix_PlayChannel(-1, collision, 0);
    }
    
    if (level == 2 && player.health != 0 && player.collidesWith(worm) && worm.animationIndex != -1) {
        player.position = glm::vec3(0.0f, 0.0f, 0.0f);
        player.velocity[1] = 0.0f;
        player.health -= 1;
        
        Mix_VolumeChunk(collision, 20);
        Mix_PlayChannel(-1, collision, 0);
    }
    
    // Checking collision with icicles/snow
    if (level == 3 && (player.collidesWith(icicle) || player.collidesWith(snow))) {
        player.health -= 1;
        player.position = glm::vec3(0.0f, 0.0f, 0.0f);
        player.velocity[1] = 0.0f;
        
        Mix_VolumeChunk(collision, 20);
        Mix_PlayChannel(-1, collision, 0);
    }

    // Checking collision with button
    if (player.collidesWith(button) && button.animationIndex != 165 && button.animationIndex != -1) {
        button.animationIndex = 165;
        key.animationIndex = 45;
        key.velocity[1] = -2.0f;
    }
    
    // Checking collision with key
    if (key.animationIndex != -1) {
        if (player.collidesWith(key)) {
            player.hasKey = true;
            key.animationIndex = -1;
            key.velocity[1] = 0.0f;
        }
    }
    
    for (int i = 0; i < 2; i++) {
        if (door[i].animationIndex == -1) {
            continue;
        } else if (player.collidesWith(door[i]) && player.hasKey) {
            door[0].animationIndex = 731;
            door[1].animationIndex = 701;
            level += 1;
            setNextLevel(level);
            break;
        }
    }
    
    if (level == 3 && player.collidesWith(building[0]) && player.hasKey) {
        level += 1;
        playersChosen = false;
        playerWin = (playerOne.hasKey) ? 1 : 2;
        setNextLevel(4);
    }
    
    player.position[0] += player.velocity[0] * elapsed;
    if (player.collisionX(leftEdge) || player.collisionX(rightEdge)) {
        player.velocity[0] = 0.0f;
    }
}

void GameState::UpdatePlayerHeartsPos() {
    if (playerOne.position[0] <= 5.56f) {
        float heartPosX = playerOne.position[0] - 0.3f;
        for (int i = 0; i < 3; i++) {
            playerOneHearts[i].position = glm::vec3(heartPosX, playerOne.position[1] + 0.85f, 0.0f);
            playerTwoHearts[i].position = glm::vec3(heartPosX, playerOne.position[1] + 0.5f, 0.0f);
            heartPosX += 0.3f;
        }
    } else {
        for (int i = 0; i < 3; i++) {
            playerOneHearts[i].position[1] = playerOne.position[1] + 0.85f;
            playerTwoHearts[i].position[1] = playerOne.position[1] + 0.5f;
        }
    }
    
    if (playerOne.health < 3) {
        playerOneHearts[playerOne.health].animationIndex = 375;
    }
    
    if (multiplayer && playerTwo.health < 3) {
        playerTwoHearts[playerTwo.health].animationIndex = 375;
    }
}

void GameState::Update(float elapsed) {
    if (playersChosen) {
        labelSign.Update(elapsed);
        fish.Update(elapsed);
        snow.position[1] += snow.velocity[1] * elapsed;
        icicle.position[1] += icicle.velocity[1] * elapsed;
        key.position[1] += key.velocity[1] * elapsed;
        
        worm.Update(elapsed);

        playerOne.Update(elapsed);
        if (multiplayer) {
            playerTwo.Update(elapsed);
        }
        
        // Fish collision with bottom
        if (fish.collisionY(floor[48]) && fish.collidedBottom) {
            fish.velocity[1] = 4.5f;
        }
        
        if (level == 2) {
            if (worm.collidesWith(leftEdge)) {
                worm.velocity[0] = 1.0f;
            } else if (worm.position[0] >= floor[28].position[0]) {
                worm.velocity[0] = -1.0f;
            }
        } else if (level == 3) {
            for (int i = 0; i < 3; i++) {
                platforms[i].position += platforms[i].velocity * elapsed;
                if (platforms[2].position[0] >= floor[11].position[0]) {
                    platforms[i].velocity[0] = -1.0f;
                } else if (platforms[0].position[0] <= floor[6].position[0]) {
                    platforms[i].velocity[0] = 1.0f;
                }
            }
        }

        collisionCheck(playerOne, elapsed);
        collisionCheck(playerTwo, elapsed);
        
        switch (level) {
            case 1: {
                int totalCoins = (multiplayer) ? playerOne.numCoins + playerTwo.numCoins : playerOne.numCoins;
                if (totalCoins == 7 && platforms[5].item != ITEM_NONE) {
                    platforms[5].animationIndex = 131;
                }
                
                break;
            }
            case 2: {
                int numPlayerOne = 0;
                int numPlayerTwo = 0;
                for (int i = 0; i < 4; i++) {
                    if (multiplayer) {
                        if (i % 2 == 0 && coins[i].animationIndex % 169 == (int) playerOneIndex / 30) {
                            ++numPlayerOne;
                        } else if (i % 2 == 1 && coins[i].animationIndex % 169 == (int)playerTwoIndex / 30) {
                            ++numPlayerTwo;
                        }
                    } else {
                        if (coins[i].animationIndex % 169 == (int) playerOneIndex / 30) {
                            ++numPlayerOne;
                        }
                    }
                }
                
                if ((multiplayer && numPlayerOne == 2 && numPlayerOne == numPlayerTwo) || (!multiplayer && numPlayerOne == 4)) {
                    if (platforms[5].item != ITEM_NONE) {
                        platforms[5].animationIndex = 131;
                    }
                }
                break;
            }
            default:
                break;
        }
        
        viewMatrixPlayer = glm::mat4(1.0f);
        if (playerOne.position[0] <= 5.56f) {
            viewMatrixPlayer = glm::translate(viewMatrixPlayer, glm::vec3(-playerOne.position[0] - 1.3f, -playerOne.position[1], 0.0f));
        } else {
            viewMatrixPlayer = glm::translate(viewMatrixPlayer, glm::vec3(-5.56 - 1.3f, -playerOne.position[1], 0.0f));
        }
        
        UpdatePlayerHeartsPos();
    }
}

void GameState::RenderPlayerChoices(ShaderProgram &program) {
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    int spriteIndex = 19;
    float posX = -1.2f;

    for (int i = 0; i < 5; i++) {
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(posX, 0.0f, 0.0f));
        program.SetModelMatrix(modelMatrix);
        
        sprites.DrawSpriteSheetSprite(program, spriteIndex, 0.6f);
        
        if (spriteIndex == playerOneIndex) {
            modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(posX - 0.01f, 0.5f, 0.0f));
            program.SetModelMatrix(modelMatrix);
            DrawArrow(program, selectPlayerOne);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.02f, 0.0f));
            program.SetModelMatrix(modelMatrix);
            DrawText(program, alphabet, "1", 0.15f, -0.05f);
        }
        
        if (spriteIndex == playerTwoIndex && multiplayer) {
            modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(posX - 0.01f, 0.5f, 0.0f));
            program.SetModelMatrix(modelMatrix);
            DrawArrow(program, selectPlayerTwo);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.02f, 0.0f));
            program.SetModelMatrix(modelMatrix);
            DrawText(program, alphabet, "2", 0.15f, -0.05f);
        }
        
        spriteIndex += 30;
        posX += 0.6f;
    }
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.0f, -0.6f, 0.0f));
    program.SetModelMatrix(modelMatrix);
    DrawText(program, alphabet, "press space to start", 0.2f, -0.1f);
}

void GameState::RenderBlocks(ShaderProgram &program, Entity array[], int arraySize) {
    for (int i = 0; i < arraySize; i++) {
        array[i].Render(program);
    }
}

void GameState::RenderGameWin(ShaderProgram &program) {
    viewMatrixPlayer = glm::mat4(1.0f);
    program.SetViewMatrix(viewMatrixPlayer);

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.5f, 0.5f, 0.0f));
    program.SetModelMatrix(modelMatrix);
    
    std::string winner = (playerWin == 1) ? "player one" : "player two";
    DrawText(program, alphabet, "Congratulations!", 0.3f, -0.1f);
    
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.2f, -0.4f, 0.0f));
    program.SetModelMatrix(modelMatrix);
    DrawText(program, alphabet, "You won, " + winner, 0.3f, -0.15f);
    
    if (playerWin == 1) {
        playerOne.position = glm::vec3(0.0f, -0.5f, 0.0f);
        playerOne.Render(program);
    } else {
        playerTwo.position = glm::vec3(0.0f, -0.5f, 0.0f);
        playerTwo.Render(program);
    }
}

void GameState::RenderGameOver(ShaderProgram &program) {
    viewMatrixPlayer = glm::mat4(1.0f);
    program.SetViewMatrix(viewMatrixPlayer);
    
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.0f, 0.5f, 0.0f));
    program.SetModelMatrix(modelMatrix);
    
    DrawText(program, alphabet, "GAME OVER!", 0.3f, -0.1f);
}

void GameState::RenderGame(ShaderProgram & program) {
    program.SetViewMatrix(viewMatrixPlayer);
    
    if (labelSign.animationIndex != -1) {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, labelSign.position);
        program.SetModelMatrix(modelMatrix);
        DrawLabel(program, labelSign.sprite.getTextureID());
        
        std::string levelText;
        std::string hint;

        switch (level) {
            case 1: {
                levelText = "LEVEL ONE:";
                hint = "Lucky Seven";
                break;
            }
            case 2: {
                levelText = "LEVEL TWO:";
                hint = (multiplayer) ? "Pair of Friends" : "FOURever Alone";
                break;
            }
            case 3: {
                levelText = "LEVEL THREE:";
                hint = "Finders Keepers";
                break;
            }
            default:
                break;
        }
        
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.6f, 0.1f, 0.0f));
        program.SetModelMatrix(modelMatrix);
        DrawText(program, alphabet, levelText, 0.2f, -0.1f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -0.2f, 0.0f));
        program.SetModelMatrix(modelMatrix);
        DrawText(program, alphabet, hint, 0.2f, -0.1f);
    }

    // Rendering button;
    button.Render(program);
    // Rendering key;
    key.Render(program);
    // Rendering floor
    RenderBlocks(program, floor, 80);
    // Rendering building
    RenderBlocks(program, building, 10);
    // Render building door
    RenderBlocks(program, door, 2);
    
    
    switch (level) {
        case 1:
            // Rendering coins
            RenderBlocks(program, coins, 10);
            // Rendering platform
            RenderBlocks(program, platforms, 6);
            // Rendering fish
            fish.Render(program);
            break;
        case 2:
            RenderBlocks(program, platforms, 6);
            RenderBlocks(program, coins, 4);
            worm.Render(program);
            break;
        case 3:
            RenderBlocks(program, platforms, 6);
            icicle.Render(program);
            snow.Render(program);
            break;
        default:
            break;
    }
    
    // Rendering hearts
    RenderBlocks(program, playerOneHearts, 3);
    playerOne.Render(program);
    if (multiplayer) {
        playerTwo.Render(program);
        RenderBlocks(program, playerTwoHearts, 3);
    }
}

void GameState::Render(ShaderProgram &program) {
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    if (playerOne.health == 0 || (multiplayer && playerTwo.health == 0)) {
        RenderGameOver(program);
        
        modelMatrix = glm::mat4(1.0f);
        playButton = glm::vec3(0.0f, -0.1f, 0.0f);
        modelMatrix = glm::translate(modelMatrix, playButton);
        program.SetModelMatrix(modelMatrix);
        DrawQuitButton(program, labelSign.sprite.getTextureID());

        modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.15f, 0.0f, 0.0f));
        program.SetModelMatrix(modelMatrix);
        DrawText(program, alphabet, "PLAY", 0.15f, -0.05f);
        
        modelMatrix = glm::mat4(1.0f);
        quitButton = glm::vec3(0.0f, -0.4f, 0.0f);
    } else if (level == 4) {
        RenderGameWin(program);
        
        modelMatrix = glm::mat4(1.0f);
        playButton = glm::vec3(-1.0f, -0.4f, 0.0f);
        modelMatrix = glm::translate(modelMatrix, playButton);
        program.SetModelMatrix(modelMatrix);
        DrawQuitButton(program, labelSign.sprite.getTextureID());

        modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.15f, 0.0f, 0.0f));
        program.SetModelMatrix(modelMatrix);
        DrawText(program, alphabet, "PLAY", 0.15f, -0.05f);
        
        modelMatrix = glm::mat4(1.0f);
        quitButton = glm::vec3(1.0f, -0.4f, 0.0f);
    } else if (!playersChosen) {
        RenderPlayerChoices(program);
        
        modelMatrix = glm::mat4(1.0f);
        quitButton = glm::vec3(1.3f, 0.8f, 0.0f);
    } else {
        RenderGame(program);
        
        modelMatrix = glm::mat4(1.0f);
        if (playerOne.position[0] <= 5.56f) {
            quitButton = glm::vec3(playerOne.position[0], playerOne.position[1] - 0.8f, 0.0f);
        } else {
            quitButton = glm::vec3(5.56f, playerOne.position[1] - 0.8f, 0.0f);
        }
    }
    
    modelMatrix = glm::translate(modelMatrix, quitButton);
    program.SetModelMatrix(modelMatrix);
    DrawQuitButton(program, labelSign.sprite.getTextureID());
    
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.15f, 0.0f, 0.0f));
    program.SetModelMatrix(modelMatrix);
    DrawText(program, alphabet, "QUIT", 0.15f, -0.05f);
}