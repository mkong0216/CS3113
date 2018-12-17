//
//  SheetSprite.cpp
//  NYUCodebase
//
//  Created by Mandy Kong on 12/13/18.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#include "SheetSprite.hpp"

SheetSprite::SheetSprite() {};

GLuint SheetSprite::getTextureID() {
    return textureID;
}

// Use for only uniform spritesheets
SheetSprite::SheetSprite(int countX, int countY, GLuint texture) {
    spriteCountX = countX;
    spriteCountY = countY;
    spriteWidth = 1.0 / (float) spriteCountX;
    spriteHeight = 1.0 / (float) spriteCountY;
    textureID = texture;
}

// Use for only non-uniform spritesheets
SheetSprite::SheetSprite(float width, float height, float size, GLuint texture) : sheetWidth(width), sheetHeight(height), size(size), textureID(texture) {}

void SheetSprite::setSpritePosAndDimensions(float x, float y, float imageWidth, float imageHeight) {
    u = x / imageWidth;
    v = y / imageHeight;
    spriteWidth = imageWidth / sheetWidth;
    spriteHeight = imageHeight / sheetHeight;
}

void SheetSprite::DrawSpriteSheetSprite (ShaderProgram &program, int index, float spriteSize) {
    float u = (float)(((int)index) % spriteCountX) / (float) spriteCountX;
    float v = (float)(((int)index) / spriteCountX) / (float) spriteCountY;
    
    float texCoords[] = {
        u, v+spriteHeight,
        u+spriteWidth, v,
        u, v,
        u+spriteWidth, v,
        u, v+spriteHeight,
        u+spriteWidth, v+spriteHeight
    };

    float vertices[] = {
        -spriteSize / 2, -spriteSize / 2,
        spriteSize / 2, spriteSize / 2,
        -spriteSize / 2, spriteSize / 2,
        spriteSize / 2, spriteSize / 2,
        -spriteSize / 2, -spriteSize / 2,
        spriteSize / 2, -spriteSize / 2
    };

    // Draw data
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
}


void SheetSprite::Draw(ShaderProgram &program) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    GLfloat texCoords[] = {
        u, v + spriteHeight,
        u + spriteWidth, v,
        u, v,
        u + spriteWidth, v,
        u, v + spriteHeight,
        u + spriteWidth, v + spriteHeight
    };
    
    float aspect = spriteWidth / spriteHeight;
    float vertices[] = {
        -0.5f * size * aspect, -0.5f * size,
        0.5f * size * aspect, 0.5f * size,
        -0.5f * size * aspect, 0.5f * size,
        0.5f * size * aspect, 0.5f * size,
        -0.5f * size * aspect, -0.5f * size,
        0.5f * size * aspect, -0.5f * size,
    };
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
}