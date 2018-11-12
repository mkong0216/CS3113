//
//  Entity.hpp
//  NYUCodebase
//
//  Created by Mandy Kong on 11/10/18.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>
#include <SDL.h>
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "SheetSprite.hpp"

enum EntityType { ENTITY_PLAYER, ENTITY_ENEMY, ENTITY_PLATFORM, ENTITY_ITEM, ENTITY_BUILDING };
enum ItemType { ITEM_COIN, ITEM_KEY, ITEM_BUTTON, ITEM_DOOR, ITEM_NONE };

class Entity {
public:
    // Methods
    Entity();
    Entity(EntityType type, bool isStatic);

    void Update(float elapsed);
    void Render(ShaderProgram &program, SheetSprite &sprite);
    
    void setCollisions();
    bool CollidesWith(Entity &entity);
    bool collisionX(Entity &entity);
    bool collisionY(Entity &entity);

    void setAnimationIndex(float elapsed, int numFrames);

    //Attributes
    glm::vec3 position;
    glm::vec3 size;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 scale;
    
    bool isStatic;
    EntityType entityType;
    ItemType itemType;
    int animationIndex;
    int numCoins;
    int health;
    
    bool hasKey;

    bool collidedTop;
    bool collidedBottom;
    bool collidedRight;
    bool collidedLeft;
};
#endif /* Entity_hpp */
