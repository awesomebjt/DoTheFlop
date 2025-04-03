#include <stdio.h>
#include <math.h>
#include <SDL.h>
#include <SDL_image.h>
#include <stdbool.h>

#include "includes/rockets.h"

int WIN_WIDTH = 1280;
int WIN_HEIGHT = 720;

void destroy_and_quit(SDL_Window *window, SDL_Renderer *renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Texture* load_mem_texture(SDL_Renderer* renderer, const unsigned char* img, const unsigned int len) {
    // Load image
    SDL_RWops* rw = SDL_RWFromConstMem(img, (int) len);
    if (rw == NULL) {
        printf("SDL_RWFromConstMem failed: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_Surface* surface = IMG_Load_RW(rw, 1);
    if (surface == NULL) {
        printf("IMG_Load_RW failed: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (texture == NULL) {
        printf("SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
        return NULL;
    }

    return texture;
}

int main(void) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Do The Flop",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIN_WIDTH,
        WIN_HEIGHT,
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

    SDL_Texture* rocket_unlit = load_mem_texture(renderer, rocket, rocket_len);
    SDL_Texture* rocket_islit = load_mem_texture(renderer, rocket_lit, rocket_lit_len);

    SDL_Texture* rocket_sprite = rocket_unlit;

    int texWidth, texHeight;
    if (SDL_QueryTexture(rocket_unlit, NULL, NULL,
                        &texWidth, &texHeight) != 0) {
        printf("SDL_QueryTexture failed: %s\n", SDL_GetError());
        destroy_and_quit(window, renderer);
        return 1;
                        }

    SDL_Rect bgRect;
    bgRect.x = 0;
    bgRect.y = 0;
    bgRect.w = WIN_WIDTH;
    bgRect.h = WIN_HEIGHT;

    SDL_Rect rocketRect;
    rocketRect.x = (WIN_WIDTH - texWidth) / 2;
    rocketRect.y = (WIN_HEIGHT - texHeight) / 2;
    rocketRect.w = texWidth;
    rocketRect.h = texHeight;
    float g = 0.4f;
    float vy = 0;
    bool thruster_on = false;

    // Event Loop
    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_UP) {

                rocket_sprite = rocket_islit;
                thruster_on = true;
            }
            if (event.type == SDL_KEYUP && event.key.keysym.scancode == SDL_SCANCODE_UP) {
                rocket_sprite = rocket_unlit;
                thruster_on = false;
            }
        }

        if (thruster_on) {
            vy -= 1;
        }
        vy += g;

        SDL_RenderClear(renderer);
        rocketRect.y += (int) round(vy);
        if (rocketRect.y >= (WIN_HEIGHT - rocketRect.h)) {
            rocketRect.y = (WIN_HEIGHT - rocketRect.h);
            vy = 0;
        }

        SDL_SetRenderDrawColor(renderer, 64, 100, 128, 255);
        SDL_RenderFillRect(renderer, NULL);

        if (SDL_RenderCopy(renderer, rocket_sprite, NULL, &rocketRect) != 0) {
            printf("SDL_RenderCopy failed: %s\n", SDL_GetError());
            running = 0;
            break;
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(34);
    }

    SDL_DestroyTexture(rocket_sprite);
    SDL_DestroyTexture(rocket_islit);
    SDL_DestroyTexture(rocket_unlit);
    destroy_and_quit(window, renderer);
    return 0;
}
