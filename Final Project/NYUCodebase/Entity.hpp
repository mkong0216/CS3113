//
//  Entity.hpp
//  NYUCodebase
//
//  Created by Mandy Kong on 12/13/18.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "ShaderProgram.h"
#include "utils.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "SheetSprite.hpp"

enum EntityType { ENTITY_PLAYER, ENTITY_PLATFORM, ENTITY_PITFALL, ENTITY_ENEMY, ENTITY_ITEM, ENTITY_DOOR, ENTITY_ENEMY_MOVE, ENTITY_LABEL };
enum ItemType { ITEM_NONE, ITEM_COIN, ITEM_KEY, ITEM_BUTTON, ITEM_TOKEN, ITEM_ICICLE };

class Entity {
public:
    Entity();
    void Update(float elapsed);
    void Render(ShaderProgram &program);
    
    void resetCollisions();
    bool collidesWith(Entity &entity);
    bool collisionX(Entity &entity);
    bool collisionY(Entity &entity);
    void setAnimationIndex(float elapsed, int numFrames);

    glm::vec3 position;
    glm::vec3 size;
    glm::vec3 velocity;
    glm::vec3 gravity;
    glm::vec3 friction;
    glm::vec3 scale;
    
    bool isStatic;
    EntityType entityType;
    ItemType item;
    
    int animationIndex;
    float animationElapsed;

    std::vector<int> walkingIndices;
    
    bool collidedTop;
    bool collidedBottom;
    bool collidedRight;
    bool collidedLeft;
    
    SheetSprite sprite;

    int health;
    int numCoins;
    bool hasKey;
    
    bool showLabel = false;
    
};

#endif /* Entity_hpp */
