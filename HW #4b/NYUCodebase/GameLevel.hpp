//
//  GameLevel.hpp
//  NYUCodebase
//
//  Created by Mandy Kong on 11/8/18.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#ifndef GameLevel_hpp
#define GameLevel_hpp

#include <stdio.h>
#include "Entity.hpp"

class GameLevel {
public:
    void SetUp();
    void ProcessInput(SDL_Event event);
    void Update(float elapsed);
    void Render(ShaderProgram &program);
    
    void checkPlayerCollisions(float elapsed);
    void SetUpMushroomPlatform();
    void SetUpBottomPlatform();
    void SetUpLevelPlatforms();
    void UpdatePlatforms(float elapsed);
    void RenderPlatforms(ShaderProgram &program);

    Entity bottomFloor[100];
    Entity mushrooms[16];
    Entity platforms[10];
    Entity leftEdge;
    Entity rightEdge;
    Entity player;
    Entity bee;
    
    SheetSprite spritesheet;
};

#endif /* GameLevel_hpp */
