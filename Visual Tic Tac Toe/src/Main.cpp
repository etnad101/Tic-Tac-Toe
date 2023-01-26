#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <vector>
#include <cmath>
#include "RenderWindow.h"
#include "Vector2f.h"
#include "Entity.h"
#include "Message.h"
#include "Game.h"
#include "Settings.h"

/**Delta time variables**/
Uint64 NOW = SDL_GetPerformanceCounter();
Uint64 LAST = 0;
double deltaTime = 0;

/**Create window**/
RenderWindow window(TITLE, WIDTH, HEIGHT);

SDL_Event event;
bool running = true;
bool end = false;

/**Creating Variables**/
char player = 'X';
bool mouseDown;
int mouseX, mouseY;
std::vector<Entity> pieces;
Game g = Game(window.loadTexture(X_PATH), window.loadTexture(O_PATH));
Entity Game_Board = Entity(Vector2f(0, 0), 600, 600, window.loadTexture(BOARD_PATH));

TTF_Font* font = window.loadFont(FONT_PATH, 52);
Message endMessage = Message(Vector2f(0, 0), "ERROR", font, window.getRenderer());

void init()
{
    if (SDL_Init(SDL_INIT_VIDEO) > 0)
        std::cout << "SDL_Init has failed, ERROR: " << SDL_GetError() << std::endl;

    if (!(IMG_Init(IMG_INIT_PNG)))
        std::cout << "IMG_init has failed, ERROR: " << SDL_GetError() << std::endl;

    if (TTF_Init() != 0)
        std::cout << "TTF_init has failed, ERROR: " << SDL_GetError() << std::endl;
}

void events()
{
    /**Event loop**/
    while (SDL_PollEvent(&event))
    {
        /**Events**/
        SDL_PumpEvents();  // make sure we have the latest mouse state.

        switch (event.type) {
        case SDL_MOUSEBUTTONDOWN:
            mouseDown = true;
            break;

        case SDL_MOUSEBUTTONUP:
            mouseDown = false;
            break;

        case SDL_QUIT:
            running = false;
            break;

        default:
            break;
        }
    }
}

void update()
{
    /**Calculates delta time**/
    LAST = NOW;
    NOW = SDL_GetPerformanceCounter();
    deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());

    if (mouseDown) {
        SDL_GetMouseState(&mouseX, &mouseY);
        int res = g.placePiece(mouseX, mouseY, player, pieces);
        switch (res)
        {
        case 0:
            if (player == 'X') {
                player = 'O';
            }
            else {
                player = 'X';
            }
            break;

        case 1:
            end = true;
            endMessage.setMessage("Player 1 has won");
            break;

        case 2:
            end = true;
            endMessage.setMessage("Player 2 has won");
            break;

        case 3:
            end = true;
            endMessage.setMessage("Tie");
            break;
        }
     
        mouseDown = false;
    }
}



void graphics()
{
    /**Clearing**/
    window.clear();

    /**Rendering**/
    window.render(Game_Board);
    for (Entity p : pieces) {
        window.render(p);
    }
    if (end) {
        window.render(endMessage);
    }

    window.display();
}

int main(int argc, char* argv[])
{
    while (running)
    {
        graphics();
        events();
        update();
    }

    window.cleanUp();
    SDL_Quit();

    return 0;
}