//
//  MainMenu.cpp
//  NYUCodebase
//
//  Created by Mandy Kong on 10/21/18.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#include "MainMenu.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "utilities.hpp"
#include "ShaderProgram.h"
#include "Entity.hpp"
#include "SheetSprite.hpp"

glm::mat4 mainMenuMatrix = glm::mat4(1.0f);

// Position of "Click to Play" in { left, right, top, bottom }
float playPos[] = { -0.7f, 0.6f, -0.65f, -0.8f };
float playFontSize = 0.15f;
float playFontSpacing = -0.05f;

// Rendering Main Menu Enemy list showing space invader types and their corresponding points
void RenderEnemyList (ShaderProgram &program, int fontTexture, int spritesheet) {
    SpriteDimensions spaceInvaders;

    float yPos = 0.25f;
    int points = 10;

    for (size_t i = 0; i < 3; i++) {
        Entity enemy;
        
        // Last enemy sprite is smaller than other enemy sprites,
        // adjust spriteWidth and spriteHeight accordingly.
        if (i == 2) {
            spaceInvaders.spriteWidth = 100.0f;
            spaceInvaders.spriteHeight = 110.0f;
        }

        // Setting sprite and points for each sprite
        enemy.sprite = SheetSprite(spritesheet, i / spaceInvaders.sheetWidth, i * spaceInvaders.spriteHeight / spaceInvaders.sheetHeight, spaceInvaders.spriteWidth / spaceInvaders.sheetWidth, spaceInvaders.spriteHeight / spaceInvaders.sheetHeight, 0.2f);
        enemy.points = points;

        // Drawing enemy sprite
        mainMenuMatrix = glm::mat4(1.0f);
        mainMenuMatrix = glm::translate(mainMenuMatrix, glm::vec3(-0.5f, yPos, 0.0f));
        program.SetModelMatrix(mainMenuMatrix);
        enemy.sprite.Draw(program);

        // Drawing label for enemy sprite
        mainMenuMatrix = glm::mat4(1.0f);
        mainMenuMatrix = glm::translate(mainMenuMatrix,glm::vec3(-0.2f, yPos, 0.0f));
        program.SetModelMatrix(mainMenuMatrix);
        std::string label = "= " + std::to_string(enemy.points) + " pts";
        DrawText(program, fontTexture, label, 0.2f, -0.1f);

        // Updating next sprite position and sprite's label position
        // Updating next sprite's points
        yPos -= (spaceInvaders.spriteHeight / spaceInvaders.sheetHeight) + 0.1f;
        points += 10.0f;
    }
    
    spaceInvaders.resetSpriteDimensions();
}

void MainMenu::Render() {
    // Space Invaders
    mainMenuMatrix = glm::mat4(1.0f);
    mainMenuMatrix = glm::translate(mainMenuMatrix, glm::vec3(-1.0f, 0.6f, 0.0f));
    program.SetModelMatrix(mainMenuMatrix);
    DrawText(program, fontTexture, "SPACE INVADERS", 0.3f, -0.15f);
    
    // Three types of enemy sprites
    RenderEnemyList(program, fontTexture, spritesheet);
    
    // Click to Play
    mainMenuMatrix = glm::mat4(1.0f);
    mainMenuMatrix = glm::translate(mainMenuMatrix, glm::vec3(-0.65f, -0.7f, 0.0f));
    program.SetModelMatrix(mainMenuMatrix);
    DrawText(program, fontTexture, "Click to Play", playFontSize, playFontSpacing);
}

// If hovering over "Click to Play", change font size.
// If "Click to Play" is clicked, change mode to STATE_GAME_LEVEL
GameMode MainMenu::ProcessInput(SDL_Event &event) {
    if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN) {
        float mouseX = (event.type == SDL_MOUSEBUTTONDOWN) ? event.button.x : event.motion.x;
        float mouseY = (event.type == SDL_MOUSEBUTTONDOWN) ? event.button.y : event.motion.y;
        
        float unitX = (((float)mouseX / 640.0f) * 3.554f ) - 1.777f;
        float unitY = (((float)(360-mouseY) / 360.0f) * 2.0f ) - 1.0f;
        
        // Increase font size of "Click to Play" text when hovered over text
        if (unitX >= playPos[0] && unitX <= playPos[1] && unitY <= playPos[2] && unitY >= playPos[3]) {
            playFontSize = 0.20f;
            playFontSpacing = -0.1f;
            
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                // If "Click to Play" is clicked, set new game mode to STATE_GAME_LEVEL.
                return GameMode::STATE_GAME_LEVEL;
            }
        } else {
            playFontSize = 0.15f;
            playFontSpacing = -0.05f;

            // "Click to Play" was not selected, remain on game mode STATE_MAIN_MENU.
            return GameMode::STATE_MAIN_MENU;
        }
    }
    
    return GameMode::STATE_MAIN_MENU;
}
