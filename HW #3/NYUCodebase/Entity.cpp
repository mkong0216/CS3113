//
//  Entity.cpp
//  NYUCodebase
//
//  Created by Mandy Kong on 10/20/18.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#include "Entity.hpp"

glm::mat4 entityMatrix = glm::mat4(1.0f);

void Entity::Draw(ShaderProgram &program) {
    glUseProgram(program.programID);

    entityMatrix = glm::mat4(1.0f);
    entityMatrix = glm::translate(entityMatrix, position);
    program.SetModelMatrix(entityMatrix);
    sprite.Draw(program);
}