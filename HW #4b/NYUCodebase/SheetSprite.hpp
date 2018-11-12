//
//  SheetSprite.hpp
//  NYUCodebase
//
//  Created by Mandy Kong on 11/8/18.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#ifndef SheetSprite_hpp
#define SheetSprite_hpp

#include <stdio.h>

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include "ShaderProgram.h"

class SheetSprite {
public:
    SheetSprite();
    SheetSprite(int countX, int countY);
    GLuint LoadTexture (const char *filePath);
    void DrawSpriteSheetSprite (ShaderProgram &program, int index);
    
    float spriteWidth;
    float spriteHeight;
    int spriteCountX;
    int spriteCountY;
    GLuint textureID;
};

#endif /* SheetSprite_hpp */
