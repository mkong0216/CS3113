//
//  MainMenu.hpp
//  NYUCodebase
//
//  Created by Mandy Kong on 12/13/18.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#ifndef MainMenu_hpp
#define MainMenu_hpp

#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "ShaderProgram.h"
#include "utils.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "SheetSprite.hpp"

class MainMenu {
public:
    void SetUp(GLuint fontTexture, GLuint buttonTexture);
    void Render(ShaderProgram &program);
    bool Update();
    bool ProcessInput(SDL_Event event);

private:
    GLuint alphabet;
    SheetSprite startButton;
    bool multiplayer;
};

#endif /* MainMenu_hpp */
