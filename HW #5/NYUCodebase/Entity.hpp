//
//  Entity.hpp
//  NYUCodebase
//
//  Created by Mandy Kong on 10/20/18.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>
#include "glm/gtc/matrix_transform.hpp"
#include "SheetSprite.hpp"
#include "ShaderProgram.h"

class Entity {
public:
    void Draw(ShaderProgram &program);
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 size;
    
    float rotation;
    SheetSprite sprite;
    
    int points;
};

#endif /* Entity_hpp */
