//
//  utilities.hpp
//  NYUCodebase
//
//  Created by Mandy Kong on 10/21/18.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#ifndef utilities_hpp
#define utilities_hpp

#include <stdio.h>

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>

#include "ShaderProgram.h"

GLuint LoadTexture(const char *filePath);
void DrawText(ShaderProgram &program, int fontTexture, std::string text, float size, float spacing);
enum GameMode { STATE_MAIN_MENU, STATE_GAME_LEVEL };

class SpriteDimensions {
public:
    float spriteWidth = 145.0f;
    float spriteHeight = 100.0f;
    float sheetWidth = 585.0f;
    float sheetHeight = 718.0f;
    
    void resetSpriteDimensions() {
        spriteWidth = 145.0f;
        spriteHeight = 100.0f;
    }
};

#endif /* utilities_hpp */
