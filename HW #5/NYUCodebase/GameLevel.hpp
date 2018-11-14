//
//  GameLevel.hpp
//  NYUCodebase
//
//  Created by Mandy Kong on 10/21/18.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#ifndef GameLevel_hpp
#define GameLevel_hpp

#include <stdio.h>
#include <SDL_mixer.h>
#include "GameState.hpp"
#include "utilities.hpp"
#include "ShaderProgram.h"

class GameLevel {
public:
    void Render();
    void Update(float elapsed);
    void ProcessInput(SDL_Event &event, float elapsed);
    
    GameState state;
    ShaderProgram texturedProgram;
    ShaderProgram untexturedProgram;
    
    Mix_Chunk *shooting;
    Mix_Chunk *spaceinvaderkilled;
    Mix_Music *music;
};

#endif /* GameLevel_hpp */
