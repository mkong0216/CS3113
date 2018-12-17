//
//  MainMenu.cpp
//  NYUCodebase
//
//  Created by Mandy Kong on 12/13/18.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#include "MainMenu.hpp"

// left, right, top, bottom
float startButtonPos[] = { -0.64f, 0.64f, -0.41f, -0.54f };

void MainMenu::SetUp(GLuint fontTexture, GLuint buttonTexture) {
    alphabet = fontTexture;
    SheetSprite buttonSprite(290, 509, 0.2f, buttonTexture);
    buttonSprite.setSpritePosAndDimensions(0, 331, 190, 49);
    startButton = buttonSprite;
    multiplayer = false;
}

bool MainMenu::ProcessInput(SDL_Event event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_UP:
                if (multiplayer) {
                    multiplayer = false;
                }
                break;
            case SDL_SCANCODE_DOWN:
                if (!multiplayer) {
                    multiplayer = true;
                }
                break;
            default:
                break;
        }
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        float mouseX = (event.type == SDL_MOUSEBUTTONDOWN) ? event.button.x : event.motion.x;
        float mouseY = (event.type == SDL_MOUSEBUTTONDOWN) ? event.button.y : event.motion.y;
        
        float unitX = (((float)mouseX / 640.0f) * 3.554f ) - 1.777f;
        float unitY = (((float)(360-mouseY) / 360.0f) * 2.0f ) - 1.0f;
        
        if (unitX >= startButtonPos[0] && unitX <= startButtonPos[1] && unitY <= startButtonPos[2] && unitY >= startButtonPos[3]) {
            return true;
        }
    }
    
    return false;
}

bool MainMenu::Update () {
    return multiplayer;
}

void MainMenu::Render(ShaderProgram &program) {
    // Render game title
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.0f, 0.6f, 0.0f));
    program.SetModelMatrix(modelMatrix);
    std::string gameTitle = "ALIEN ESCAPE";
    DrawText(program, alphabet, gameTitle, 0.3f, -0.12f);
    
    // Render player options
    std::string selection = "> ";
    std::string playerOne = "Single player";
    std::string playerTwo = "Multiplayer";
    
    std::string selected = (multiplayer) ? playerOne : selection + playerOne;
    float positionX = (multiplayer) ? -0.55f : -0.7f;

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(positionX, 0.2f, 0.0f));
    program.SetModelMatrix(modelMatrix);
    DrawText(program, alphabet, selected, 0.2f, -0.1f);
    
    selected = (multiplayer) ? selection + playerTwo : playerTwo;
    positionX = (multiplayer) ? -0.15f : 0.22f;

    modelMatrix = glm::translate(modelMatrix, glm::vec3(positionX, -0.3f, 0.0f));
    program.SetModelMatrix(modelMatrix);
    DrawText(program, alphabet, selected, 0.2f, -0.1f);
    
    // Render start button
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -0.5f, 0.0f));
    program.SetModelMatrix(modelMatrix);
    startButton.Draw(program);

    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.35f, 0.0f, 0.0f));
    program.SetModelMatrix(modelMatrix);
    DrawText(program, alphabet, "Start Game", 0.125f, -0.05f);
}
