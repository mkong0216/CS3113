//
//  MainMenu.hpp
//  NYUCodebase
//
//  Created by Mandy Kong on 10/21/18.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#ifndef MainMenu_hpp
#define MainMenu_hpp

#include <stdio.h>
#include "ShaderProgram.h"
#include "utilities.hpp"

class MainMenu {
public:
    GLuint fontTexture;
    GLuint spritesheet;
    ShaderProgram program;

    void Render();
    void Update();

    GameMode ProcessInput(SDL_Event &event);
};

#endif /* MainMenu_hpp */
