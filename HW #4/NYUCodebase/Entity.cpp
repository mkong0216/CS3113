//
//  Entity.cpp
//  NYUCodebase
//
//  Created by Mandy Kong on 11/10/18.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#include "Entity.hpp"

// Walking animation index
float animationElapsed = 0.0f;
float framesPerSecond = 30.0f;

const int playerWalking[] = { 110, 118, 119 };
const int fishJump[] = { 410, 411 };

Entity::Entity() {
    animationIndex = 0;
    isStatic = true;
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    acceleration = glm::vec3(0.0f, -9.81f, 0.0f);
    velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(0.4f, 0.4f, 0.0f);
    size = glm::vec3(0.4f, 0.4f, 0.0f);
    itemType = ITEM_NONE;
    numCoins = 0;
    hasKey = false;
    health = 3;
    setCollisions();
}

Entity::Entity(EntityType type, bool isStatic) : entityType(type), isStatic(isStatic) {
    animationIndex = 0;
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    acceleration = glm::vec3(0.0f, -9.81f, 0.0f);
    velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(0.4f, 0.4f, 0.0f);
    size = glm::vec3(0.4f, 0.4f, 0.0f);
    itemType = ITEM_NONE;
    numCoins = 0;
    hasKey = false;
    health = 3;
    setCollisions();
}

void Entity::setCollisions() {
    collidedBottom = false;
    collidedTop = false;
    collidedLeft = false;
    collidedRight = false;
}

void Entity::setAnimationIndex (float elapsed, int numFrames) {
    animationElapsed += elapsed;
    if (animationElapsed > 1.0 / framesPerSecond) {
        ++animationIndex;
        animationElapsed = 0.0f;
        
        if (animationIndex > numFrames - 1) {
            animationIndex = 0;
        }
    }
}

bool Entity::CollidesWith(Entity &entity) {
    float distanceX = fabs(position[0] - entity.position[0]) - ((size[0] + entity.size[0]) / 2);
    float distanceY = fabs(position[1] - entity.position[1]) - ((size[1] + entity.size[1]) / 2);

    return (distanceX <= 0 && distanceY <= 0);
}

bool Entity::collisionX (Entity &entity) {
    if (CollidesWith(entity)) {
        float x_distance = fabs(position[0] - entity.position[0]);
        float halfWidth1 = size[0] / 2;
        float halfWidth2 = entity.size[0] / 2;
        
        float penetration = fabs(x_distance - halfWidth1 - halfWidth2);
        
        if (velocity[0] < 0.0f) {
            position[0] += penetration;
            collidedLeft = true;
        } else if (velocity[0] > 0.0f) {
            position[0] -= penetration;
            collidedRight = true;
        }
        
        return true;
    } else {
        return false;
    }
}

bool Entity::collisionY(Entity &entity) {
    if (CollidesWith(entity)) {
        float y_distance = position[1] - entity.position[1];
        float halfHeight1 = size[1] / 2;
        float halfHeight2 = entity.size[1] / 2;
        
        float penetration = fabs(y_distance - halfHeight1 - halfHeight2);
//        penetration += 0.075f;

        if (y_distance < 0) {
//            position[1] -= penetration;
            collidedTop = true;
        } else if (y_distance > 0) {
            position[1] += penetration;
            collidedBottom = true;
        } else {
            collidedBottom = true;
            collidedTop = true;
        }
        
        return true;
    } else {
        return false;
    }
}

void Entity::Update(float elapsed) {
    if (!isStatic) {
        switch (entityType) {
            case ENTITY_ITEM:
            case ENTITY_PLAYER:
                if (!collidedBottom) {
                    velocity[1] += acceleration[1] * elapsed;
                }
                
                if (velocity[0] != 0.0f) {
                    setAnimationIndex(elapsed, 3);
                }

                break;
            case ENTITY_ENEMY:
                velocity[1] += acceleration[1] * elapsed;
                position[1] += velocity[1] * elapsed;
                if (velocity[1] < 0.0f) {
                    animationIndex = 1;
                }
            default:
                break;
        }
    }
}

void Entity::Render(ShaderProgram &program, SheetSprite &sprite) {
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, scale);
    program.SetModelMatrix(modelMatrix);
    
    int index;
    if (entityType == ENTITY_PLAYER) {
        index = playerWalking[animationIndex];
    } else if (entityType == ENTITY_ENEMY) {
        index = fishJump[animationIndex];
    } else {
        index = animationIndex;
    }

    sprite.DrawSpriteSheetSprite(program, index);
}