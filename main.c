#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include "includes/rockets.h"

int main(void) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Do The Flop",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        SDL_WINDOW_SHOWN);

    if (window == NULL) {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL) {
        printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Load image
    SDL_RWops* rw = SDL_RWFromConstMem(rocket, rocket_len);
    if (rw == NULL) {
        printf("SDL_RWFromConstMem failed: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Surface* surface = IMG_Load_RW(rw, 1);
    if (surface == NULL) {
        printf("IMG_Load_RW failed: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (texture == NULL) {
        printf("SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int texWidth, texHeight;
    if (SDL_QueryTexture(texture, NULL, NULL,
                        &texWidth, &texHeight) != 0) {
        printf("SDL_QueryTexture failed: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
                        }

    SDL_Rect rocketRect;
    rocketRect.x = (800 - texWidth) / 2;
    rocketRect.y = (600 - texHeight) / 2;
    rocketRect.w = texWidth;
    rocketRect.h = texHeight;

    int vy = 0;

    // Event Loop
    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_UP) {
                vy -= 1;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
                vy += 1;
            }
        }

        SDL_RenderClear(renderer);

        rocketRect.y += vy;

        if (SDL_RenderCopy(renderer, texture, NULL, &rocketRect) != 0) {
            printf("SDL_RenderCopy failed: %s\n", SDL_GetError());
            running = 0;
            break;
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(34);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
