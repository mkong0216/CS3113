//
//  utils.hpp
//  NYUCodebase
//
//  Created by Mandy Kong on 12/13/18.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#ifndef utils_hpp
#define utils_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

GLuint LoadTexture(const char *filePath, bool isLinear);
float mapValue(float value, float srcMin, float srcMax, float dstMin, float dstMax);
void DrawText(ShaderProgram &program, int fontTexture, std::string text, float size, float spacing);
void DrawArrow(ShaderProgram &program, int textureID);
void DrawLabel(ShaderProgram &program, int textureID);
void DrawQuitButton(ShaderProgram &program, int textureID);
float lerp(float v0, float v1, float t);
float easeIn(float from, float to, float time);
float easeOut(float from, float to, float time);

#endif /* utils_hpp */
