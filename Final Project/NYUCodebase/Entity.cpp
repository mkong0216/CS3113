//
//  Entity.cpp
//  NYUCodebase
//
//  Created by Mandy Kong on 12/13/18.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#include "Entity.hpp"
#include "utils.hpp"

Entity::Entity () {
    isStatic = true;
    animationIndex = 0;
    scale = glm::vec3(1.0f, 1.0f, 0.0f);
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    gravity = glm::vec3(0.0f, -9.81f, 0.0f);
    friction = glm::vec3(0.0f, 0.0f, 0.0f);
    size = glm::vec3(0.6f, 0.6f, 0.0f);
    animationElapsed = 0.0f;
    health = 3;
    numCoins = 0;
    hasKey = false;
    item = ITEM_NONE;
    resetCollisions();
}

void Entity::resetCollisions() {
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;
    collidedTop = false;
}

bool Entity::collidesWith(Entity &entity) {
    float distanceX = fabs(position[0] - entity.position[0]) - ((size[0] + entity.size[0]) / 2);
    float distanceY = fabs(position[1] - entity.position[1]) - ((size[1] + entity.size[1]) / 2);
    
    return (distanceX <= 0 && distanceY <= 0);
}

bool Entity::collisionX (Entity &entity) {
    if (collidesWith(entity)) {
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
    if (collidesWith(entity) && entity.entityType == ENTITY_PLATFORM) {
        float y_distance = fabs(position[1] - entity.position[1]);
        float halfHeight1 = size[1] / 2;
        float halfHeight2 = entity.size[1] / 2;
        
        float penetration = fabs(y_distance - halfHeight1 - halfHeight2);
        
        if (position[1] <= entity.position[1]) {
            position[1] -= (penetration + 0.005f);
            collidedTop = true;
        } else if (position[1] > entity.position[1]) {
            position[1] += (penetration + 0.005f);
            collidedBottom = true;
        }

        return true;
    } else {
        return false;
    }
}

float framesPerSecond = 20.0f;

void Entity::setAnimationIndex (float elapsed, int numFrames) {
    animationElapsed += elapsed;
    if (animationElapsed > 1.0f / framesPerSecond) {
        ++animationIndex;
        animationElapsed = 0.0f;
        
        if (animationIndex > numFrames - 1) {
            animationIndex = 0;
        }
    }
}

bool goingUp = false;
float animationTime = 0.0f;
void Entity::Update(float elapsed) {
    if (animationIndex == -1) return;

    if (!isStatic) {
        switch (entityType) {
            case ENTITY_LABEL: {
                animationTime = animationTime + elapsed;
                float animationValue = mapValue(animationTime, 0.0f, 1.0f, 0.0f, 1.0f);
                float yPos;
                
                if (goingUp) {
                    yPos = easeOut(0.0f, 2.0f, animationValue);
                } else {
                    yPos = easeIn(2.0f, 0.0f, animationValue);
                }
                
                position[1] = yPos;
                
                if (goingUp && animationTime >= 1.0f) {
                    animationIndex = -1;
                    animationTime = 0.0f;
                    goingUp = false;
                } else if (!goingUp && animationTime >= 3.0f) {
                    goingUp = true;
                    animationTime = 0.0f;
                }

                break;
            }
            case ENTITY_PLAYER:
                if (!collidedBottom) {
                    velocity += gravity * elapsed;
                }
        
                if (velocity[0] != 0.0f) {
                    setAnimationIndex(elapsed, 3);
                }
                
                break;
            case ENTITY_ENEMY_MOVE:
                if (velocity[0] != 0.0f) {
                    setAnimationIndex(elapsed, 3);
                }
                position += velocity * elapsed;
                break;
            case ENTITY_ENEMY:
                velocity += gravity * elapsed;
                position += velocity * elapsed;
                animationIndex = (velocity[1] < 0.0f) ? 411 : 410;
                break;
            default:
                break;
        }
    }
}

void Entity::Render(ShaderProgram &program) {
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    program.SetModelMatrix(modelMatrix);

    switch (entityType) {
        case ENTITY_PLAYER:
        case ENTITY_ENEMY_MOVE:
            sprite.DrawSpriteSheetSprite(program, walkingIndices[animationIndex], size[0]);
            break;
        case ENTITY_PITFALL:
        case ENTITY_PLATFORM:
        case ENTITY_ENEMY:
        case ENTITY_ITEM:
        case ENTITY_DOOR:
            if (animationIndex != -1) {
                sprite.DrawSpriteSheetSprite(program, animationIndex, size[0]);
            }
            break;
        default:
            break;
    }
}