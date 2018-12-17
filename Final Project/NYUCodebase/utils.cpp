//
//  utils.cpp
//  NYUCodebase
//
//  Created by Mandy Kong on 12/13/18.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#include "utils.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint LoadTexture (const char *filePath, bool isLinear) {
    int w, h, comp;
    unsigned char* image = stbi_load(filePath, &w, &h, &comp, STBI_rgb_alpha);
    
    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }
    
    GLuint retTexture;
    glGenTextures(1, &retTexture);
    glBindTexture(GL_TEXTURE_2D, retTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    if (isLinear) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    
    stbi_image_free(image);
    return retTexture;
}

void DrawArrow(ShaderProgram &program, int textureID) {
    float vertices[] = {-0.1f, -0.1f, 0.1f, 0.1f, -0.1f, 0.1f, 0.1f, 0.1f, -0.1f, -0.1f, 0.1f, -0.1f};
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);

    float texCoords[] = { 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void DrawQuitButton(ShaderProgram &program, int textureID) {
    float vertices[] = {-0.3f, -0.1f, 0.3f, 0.1f, -0.3f, 0.1f, 0.3f, 0.1f, -0.3f, -0.1f, 0.3f, -0.1f};
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    
    float texCoords[] = { 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void DrawLabel(ShaderProgram &program, int textureID) {
    float vertices[] = {-1.0f, -0.25f, 1.0f, 0.25f, -1.0f, 0.25f, 1.0f, 0.25f, -1.0f, -0.25f, 1.0f, -0.25f};
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    
    float texCoords[] = { 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void DrawText(ShaderProgram &program, int fontTexture, std::string text, float size, float spacing) {
    float character_size = 1.0/16.0f;
    
    std::vector<float> vertexData;
    std::vector<float> texCoordData;
    
    for (int i = 0; i < text.size(); i++) {
        int spriteIndex = (int)text[i];
        
        float texture_x = (float)(spriteIndex % 16) / 16.0f;
        float texture_y = (float)(spriteIndex / 16) / 16.0f;
        
        vertexData.insert(vertexData.end(), {
            ((size + spacing) * i) + (-0.5f * size), 0.5f * size,
            ((size + spacing) * i) + (-0.5f * size), -0.5f * size,
            ((size + spacing) * i) + (0.5f * size), 0.5f * size,
            ((size + spacing) * i) + (0.5f * size), -0.5f * size,
            ((size + spacing) * i) + (0.5f * size), 0.5f * size,
            ((size + spacing) * i) + (-0.5f * size), -0.5f * size
        });
        
        texCoordData.insert(texCoordData.end(), {
            texture_x, texture_y,
            texture_x, texture_y + character_size,
            texture_x + character_size, texture_y,
            texture_x + character_size, texture_y + character_size,
            texture_x + character_size, texture_y,
            texture_x, texture_y + character_size
        });
    }
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
    glEnableVertexAttribArray(program.positionAttribute);
    
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, vertexData.size() / 2.0f);
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
}

float lerp(float v0, float v1, float t) {
    return (1.0-t)*v0 + t*v1;
}

float easeIn(float from, float to, float time) {
    float tVal = time*time*time*time*time;
    return (1.0f-tVal)*from + tVal*to;
}

float easeOut(float from, float to, float time) {
    float oneMinusT = 1.0f-time;
    float tVal =  1.0f - (oneMinusT * oneMinusT * oneMinusT *
                          oneMinusT * oneMinusT);
    return (1.0f-tVal)*from + tVal*to;
}

float easeInOut(float from, float to, float time) {
    float tVal;
    if(time > 0.5) {
        float oneMinusT = 1.0f-((0.5f-time)*-2.0f);
        tVal =  1.0f - ((oneMinusT * oneMinusT * oneMinusT * oneMinusT *
                         oneMinusT) * 0.5f);
    } else {
        time *= 2.0;
        tVal = (time*time*time*time*time)/2.0;
    }
    return (1.0f-tVal)*from + tVal*to;
}

float mapValue(float value, float srcMin, float srcMax, float dstMin, float dstMax) {
    float retVal = dstMin + ((value - srcMin)/(srcMax-srcMin) * (dstMax-dstMin));
    if(retVal < dstMin) {
        retVal = dstMin;
    }
    if(retVal > dstMax) {
        retVal = dstMax;
    }
    return retVal;
}
