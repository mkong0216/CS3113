//
//  GameLevel.cpp
//  NYUCodebase
//
//  Created by Mandy Kong on 11/8/18.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#include "GameLevel.hpp"

int NUM_TILES_WIDTH = 20;
glm::mat4 viewMatrix1 = glm::mat4(1.0f);

void GameLevel::SetUpMushroomPlatform () {
    float positionX = -1.7f;
    float positionY = -0.3f;

    // First ten entities are the two mushroom heads
    for (int i = 0; i < NUM_TILES_WIDTH / 2; i++) {
        Entity mushroom(ENTITY_PLATFORM, true);
        mushroom.index = 361 + (i % 5);
        
        // Next mushroom move towards right of screen
        if (i % 5 == 0 && i != 0) {
            positionX = 0.98f;
        }
        
        // Next mushroom head
        if (i % 5 == 2 || i % 5 == 4) {
            mushroom.index -= 1;
        }

        mushroom.position = glm::vec3(positionX, positionY, 0.0f);
        mushroom.scale = glm::vec3(0.2f, 0.2f, 0.0f);
        mushroom.size = glm::vec3(0.2f, 0.2f, 0.0f);
        
        mushrooms[i] = mushroom;
        positionX += 0.18f;
    }
    
    // Last six entities are the mushroom stems
    int mushroomStems[] = { 428, 429, 426 };
    int stemIndex = 0;
    positionX = -1.34f;
    positionY = -0.4f;

    for (int j = NUM_TILES_WIDTH / 2; j < 16; j++) {
        Entity mushroom(ENTITY_PLATFORM, true);
        if (stemIndex % 3 == 0) {
            stemIndex = 0;
            positionX *= -1;
            positionY = -0.4f;
        }
        
        mushroom.index = mushroomStems[stemIndex];
        stemIndex++;
        
        mushroom.position = glm::vec3(positionX, positionY, 0.0f);
        mushroom.scale = glm::vec3(0.2f, 0.2f, 0.0f);
        mushroom.size = glm::vec3(0.2f, 0.2f, 0.0f);
        
        mushrooms[j] = mushroom;
        positionY -= 0.18f;
    }
}

void GameLevel::SetUpBottomPlatform() {
    float positionX = -1.7f;
    float positionY = -0.78f;
    for (int i = 0; i < NUM_TILES_WIDTH * 5; i++) {
        Entity block(ENTITY_PLATFORM, true);
        if (i % 20 == 0 && i != 0) {
            positionX = -1.7f;
            positionY -= 0.18f;
        }
        
        block.index = (i < 20) ? 123 : 152;
        block.position = glm::vec3(positionX, positionY, 0.0f);
        block.scale = glm::vec3(0.2f, 0.2f, 0.0f);
        block.size = glm::vec3(0.2f, 0.2f, 0.0f);
        
        bottomFloor[i] = block;
        positionX += 0.18f;
    }
}

void GameLevel::SetUpLevelPlatforms() {
    float positionX = -1.7f;
    float positionY = 0.7f;

    for (int i = 0; i < NUM_TILES_WIDTH / 2; i++) {
        Entity block(ENTITY_PLATFORM, true);
        block.index = 497;
        // Next platform move towards right of screen
        if (i % 5 == 0 && i != 0) {
            positionX = 0.98f;
        }
        
        block.position = glm::vec3(positionX, positionY, 0.0f);
        block.scale = glm::vec3(0.2f, 0.2f, 0.0f);
        block.size = glm::vec3(0.2f, 0.2f, 0.0f);
        
        platforms[i] = block;
        positionX += 0.18f;
    }
}

void SetUpPlayer (Entity &player) {
    player.isStatic = false;
    player.entityType = ENTITY_PLAYER;
    player.position = glm::vec3(-1.5f, -0.5f, 0.0f);
    player.scale = glm::vec3(0.4f, 0.4f, 0.0f);
    player.size = glm::vec3(0.4f, 0.4f, 0.0f);
}

void SetUpBee (Entity &bee) {
    bee.isStatic = false;
    bee.entityType = ENTITY_ENEMY;
    bee.position = glm::vec3(0.0f, 0.1f, 0.0f);
    bee.scale = glm::vec3(0.4f, 0.4f, 0.0f);
    bee.size = glm::vec3(0.4f, 0.4f, 0.0f);
    bee.velocity = glm::vec3(1.0f, 0.0f, 0.0f);
}

void SetUpEdges (Entity &leftEdge, Entity &rightEdge) {
    leftEdge.position = glm::vec3(-1.877f, 0.0f, 0.0f);
    leftEdge.entityType = ENTITY_FRAME;
    leftEdge.isStatic = true;
    leftEdge.size = glm::vec3(0.1f, 2.0f, 0.0f);

    rightEdge.position = glm::vec3(1.877f, 0.0f, 0.0f);
    rightEdge.entityType = ENTITY_FRAME;
    rightEdge.isStatic = true;
    rightEdge.size = glm::vec3(0.1f, 2.0f, 0.0f);
}

void GameLevel::SetUp() {
    SetUpBottomPlatform();
    SetUpMushroomPlatform();
    SetUpLevelPlatforms();
    SetUpEdges(leftEdge, rightEdge);
    SetUpPlayer(player);
    SetUpBee(bee);
}

void GameLevel::ProcessInput(SDL_Event event) {
    player.ProcessInput(event);
    player.resetCollisions();
    bee.resetCollisions();
}

void GameLevel::UpdatePlatforms(float elapsed) {
    for (int i = 0; i < NUM_TILES_WIDTH * 5; i++) {
        bottomFloor[i].Update(elapsed);
    }
    
    for (int j = 0; j < 16; j++) {
        mushrooms[j].Update(elapsed);
    }
    
    for (int i = 0; i < NUM_TILES_WIDTH / 2; i++) {
        platforms[i].Update(elapsed);
    }
}

void GameLevel::checkPlayerCollisions (float elapsed) {
    // Checking if player is still on bottom floor;
    // If velocity_y != 0, apply gravity
    if (player.velocity[1] != 0.0f && !player.collidedBottom) {
        player.velocity[1] += player.acceleration[1] * elapsed;
    }

    player.position[1] += player.velocity[1] * elapsed;

    for (int i = 0; i < NUM_TILES_WIDTH * 2; i++) {
        if (player.CollidesWith(bottomFloor[i])) {
            player.collisionY(bottomFloor[i]);
            break;
        }
    }
    
    // Checking if player jumped onto the mushroom platform or level platform
    for (int j = 0; j < NUM_TILES_WIDTH / 2; j++) {
        if (player.CollidesWith(mushrooms[j])) {
            player.collisionY(mushrooms[j]);
            break;
        } else if (player.CollidesWith(platforms[j])) {
            player.collisionY(platforms[j]);
            break;
        }
    }
    
    // Checking if player jumped onto moving bee
    if (player.CollidesWith(bee)) {
        player.collisionY(bee);
    }
    
    // Checking if player hit left or right screen edges
    player.position[0] += player.velocity[0] * elapsed;

    if (player.CollidesWith(leftEdge)) {
        player.collisionX(leftEdge);
    } else if (player.CollidesWith(rightEdge)) {
        player.collisionX(rightEdge);
    }
    
    // Checking if bee hit left or right screen edges
    if (bee.CollidesWith(leftEdge)) {
        bee.collisionX(leftEdge);
        bee.velocity[0] = 1.0f;
    } else if (bee.CollidesWith(rightEdge)) {
        bee.collisionX(rightEdge);
        bee.velocity[0] = -1.0f;
    }
}


void GameLevel::Update(float elapsed) {
    UpdatePlatforms(elapsed);
    bee.Update(elapsed);
    checkPlayerCollisions(elapsed);
    player.Update(elapsed);

    // Testing view matrix following player jumps
    viewMatrix1 = glm::mat4(1.0f);
    viewMatrix1 = glm::translate(viewMatrix1, glm::vec3(0.0f, -player.position[1], 0.0f));
}

void GameLevel::RenderPlatforms(ShaderProgram &program) {
    for (int j = 0; j < 16; j++) {
        if (mushrooms[j].index != -1) {
            mushrooms[j].Render(program, spritesheet);
        }
    }
    
    for (int i = 0; i < NUM_TILES_WIDTH * 5; i++) {
        bottomFloor[i].Render(program, spritesheet);
    }
    
    for (int i = 0; i < NUM_TILES_WIDTH / 2;  i++) {
        platforms[i].Render(program, spritesheet);
    }
}

void GameLevel::Render(ShaderProgram &program) {
    program.SetViewMatrix(viewMatrix1);
    RenderPlatforms(program);
    player.Render(program, spritesheet);
    bee.Render(program, spritesheet);
}
