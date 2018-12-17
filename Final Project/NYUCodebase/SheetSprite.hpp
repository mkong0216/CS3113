//
//  SheetSprite.hpp
//  NYUCodebase
//
//  Created by Mandy Kong on 12/13/18.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#ifndef SheetSprite_hpp
#define SheetSprite_hpp

#include <stdio.h>
#include "ShaderProgram.h"

class SheetSprite {
public:
    SheetSprite();
    SheetSprite(int countX, int countY, GLuint texture);
    SheetSprite(float width, float height, float size, GLuint texture);

    void setSpritePosAndDimensions(float x, float y, float imageWidth, float imageHeight);
    GLuint getTextureID ();

    void Draw(ShaderProgram &program);
    void DrawSpriteSheetSprite(ShaderProgram &program, int index, float spriteSize);

private:
    GLuint textureID;

    // For non-uniform spritesheets
    float u;
    float v;
    float size;
    float sheetWidth;
    float sheetHeight;
    
    // For uniform spritesheets
    int spriteCountX;
    int spriteCountY;
    
    // For both
    float spriteWidth;
    float spriteHeight;

};

#endif /* SheetSprite_hpp */
