//
//  Entity.cpp
//  NYUCodebase
//
//  Created by Mandy Kong on 11/8/18.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#include "Entity.hpp"

// Walking animation index
float animationElapsed = 0.0f;
float framesPerSecond = 30.0f;

const int walking[] = { 110, 118, 119 };
const int beeFlying[] = { 354, 355 };

Entity::Entity() {
    acceleration = glm::vec3(0.0f, -9.81f, 0.0f);
    animationIndex = 0;
    velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    resetCollisions();
}

Entity::Entity(EntityType type, bool isStatic) {
    entityType = type;
    isStatic = isStatic;
    
    animationIndex = 0;
    acceleration = glm::vec3(0.0f, -9.81f, 0.0f);
    velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    resetCollisions();
}

void Entity::resetCollisions() {
    collidedBottom = false;
    collidedTop = false;
    collidedLeft = false;
    collidedRight = false;
}

void Entity::ProcessInput(SDL_Event event) {
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    switch (entityType) {
        case ENTITY_PLAYER:
            if (keys[SDL_SCANCODE_LEFT] && !collidedLeft) {
                velocity[0] = -2.0f;
            } else if (keys[SDL_SCANCODE_RIGHT] && !collidedRight) {
                velocity[0] = 2.0f;
            } else {
                velocity[0] = 0.0f;
            }
            
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.scancode == SDL_SCANCODE_SPACE && collidedBottom && !collidedTop) {
                    velocity[1] = 3.0f;
                }
            }
            break;
        case ENTITY_ENEMY:
        case ENTITY_PLATFORM:
        default:
            break;
    }
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

int Entity::collisionY (Entity &entity) {
    float y_distance = position[1] - entity.position[1];
    float halfHeight1 = size[1] / 2;
    float halfHeight2 = entity.size[1] / 2;
    
    float penetration = fabs(y_distance - halfHeight1 - halfHeight2);
    
    if (y_distance < 0) {
        collidedTop = true;
        return COLLIDED_TOP;
    } else if (y_distance > 0) {
        position[1] += penetration;
        collidedBottom = true;
        return COLLIDED_BOTTOM;
    } else {
        return COLLIDED_NONE;
    }
}

int Entity::collisionX(Entity &entity) {
    float x_distance = fabs(position[0] - entity.position[0]);
    float halfWidth1 = size[0] / 2;
    float halfWidth2 = entity.size[0] / 2;

    float penetration = fabs(x_distance - halfWidth1 - halfWidth2);

    if (velocity[0] < 0) {
        position[0] += penetration;
        collidedLeft = true;
        return COLLIDED_LEFT;
    } else if (velocity[0] > 0) {
        position[0] -= penetration;
        collidedRight = true;
        return COLLIDED_RIGHT;
    } else {
        return COLLIDED_NONE;
    }
}

void Entity::Update(float elapsed) {
    if (!isStatic) {
        switch (entityType) {
            case ENTITY_PLAYER:
                // If velocity_x != 0, updating walking index
                if (velocity[0] != 0.0f) {
                    setAnimationIndex(elapsed, 3);
                }

                break;
            case ENTITY_ENEMY:
                position[0] += velocity[0] * elapsed;
                setAnimationIndex(elapsed, 2);
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

    switch (entityType) {
        case ENTITY_PLAYER:
            sprite.DrawSpriteSheetSprite(program, walking[animationIndex]);
            break;
        case ENTITY_ENEMY:
            sprite.DrawSpriteSheetSprite(program, beeFlying[animationIndex]);
            break;
        case ENTITY_PLATFORM:
            sprite.DrawSpriteSheetSprite(program, index);
            break;
        default:
            break;
    }
}


