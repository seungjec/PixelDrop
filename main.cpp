#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>


int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow(
        "PixelDrop", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_W, WINDOW_H, false);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Event event;

    // Texture
    SDL_Surface* tmpSurface;

    SDL_Texture* texture[7]{};
    for (int i = 0; i < 7; i++)
    {
        tmpSurface = IMG_Load(img[i]);
        texture[i] = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    }
    SDL_Texture* blockTexture = texture[4];

    SDL_FreeSurface(tmpSurface);
    // End of Texture

    // Rectangle
    SDL_Rect bgRect{ 0, 0, WINDOW_W, WINDOW_H };

    // Main Loop
    while (isRunning)
    {
        // Get Ticks
        frameStart = (Uint32)SDL_GetTicks64();

        // Get Event
        SDL_PollEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            isRunning = false;
            break;

        /* KEYDOWN event */
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_SPACE:
                clearBlock();
                break;
            case SDLK_1:
                setBlockColor(blockTexture, texture[1]);
                break;
            case SDLK_2:
                setBlockColor(blockTexture, texture[2]);
                break;
            case SDLK_3:
                setBlockColor(blockTexture, texture[3]);
                break;
            case SDLK_4:
                setBlockColor(blockTexture, texture[4]);
                break;
            case SDLK_5:
                setBlockColor(blockTexture, texture[5]);
                break;
            case SDLK_6:
                setBlockColor(blockTexture, texture[6]);
                break;
            }
            break;

        /* Mouse event */
        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                putBlocks(event.button.x, event.button.y, Radius, blockTexture);
                break;
            }
            break;

        case SDL_MOUSEMOTION:
            switch (event.motion.state)
            {
            case SDL_BUTTON_LMASK:
                putBlocks(event.motion.x, event.motion.y, Radius, blockTexture);
                break;
            }
        }

        // Update
        if (frameCount % updateCount == 0)
        {
            for (int i = 0; i < curBlockIdx; i++)
            {
                checkStop(block[i]);
                if (block[i].updatable)
                {
                    dropBlock(block[i]);
                }
            }
        }

        // Render
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture[0], NULL, NULL);
        for (int i = 0; i < curBlockIdx; i++)
        {
            if (block[i].onScreen)
            {
                SDL_RenderCopy(renderer, block[i].texture, NULL, &block[i].rect);
            }
        }
        SDL_RenderPresent(renderer);


        // Control iteration time for Specific FPS
        frameTime = (Uint32)SDL_GetTicks64() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
        if (frameTime > 0)
        {
            //printf("FPS : %d\n", 1000/((Uint32)SDL_GetTicks64() - frameStart));
        }

        frameCount++;
    }

    // Close SDL
    for (int i = 0; i < 7; i++)
    {
        SDL_DestroyTexture(texture[i]);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}