//
//  Entity.hpp
//  NYUCodebase
//
//  Created by Mandy Kong on 11/8/18.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "SheetSprite.hpp"

enum EntityType { ENTITY_PLAYER, ENTITY_ENEMY, ENTITY_PLATFORM, ENTITY_FRAME };
enum collided { COLLIDED_TOP, COLLIDED_BOTTOM, COLLIDED_LEFT, COLLIDED_RIGHT, COLLIDED_NONE };

class Entity {
public:
    Entity();
    Entity(EntityType type, bool isStatic);
    
    void ProcessInput(SDL_Event event);
    void Update(float elapsed);
    void Render(ShaderProgram &program, SheetSprite &sprite);

    bool CollidesWith(Entity &entity);
    int collisionY(Entity &entity);
    int collisionX(Entity &entity);
    void resetCollisions();
    
    void setAnimationIndex(float elapsed, int numFrames);
    
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 size;
    glm::vec3 velocity;
    glm::vec3 acceleration;

    bool isStatic;
    EntityType entityType;
    int index;
    int animationIndex;
    
    bool collidedTop;
    bool collidedBottom;
    bool collidedLeft;
    bool collidedRight;
};

#endif /* Entity_hpp */
