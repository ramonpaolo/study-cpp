#include <iostream>
#include <SDL3/SDL.h>
#include <math.h>
#include <random>
#include <string>

#include "defs.h"

namespace apple
{
    bool spawn(u_int16_t windowWidth, u_int16_t windowHeight, SDL_Renderer *renderer, SDL_FRect &apple, SDL_FRect &player);
    std::vector<u_int16_t> choose_location(u_int16_t windowWidth, u_int16_t windowHeight);
};

std::vector<u_int16_t> apple::choose_location(u_int16_t windowWidth, u_int16_t windowHeight)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> disWidth(0, windowWidth - APPLE_SIZE);
    std::uniform_int_distribution<> disHeigth(0, windowHeight - APPLE_SIZE);

    u_int16_t randomWidth = disWidth(gen);
    u_int16_t randomHeigth = disHeigth(gen);

    std::string wString = std::to_string(randomWidth);
    std::string hString = std::to_string(randomHeigth);

    u_int8_t wLen = wString.length();
    u_int8_t hLen = hString.length();

    if (wString[wLen - 1] != '0')
    {
        wString[wLen - 1] = '0';

        randomWidth = std::stoi(wString);
    }

    if (hString[hLen - 1] != '0')
    {
        hString[hLen - 1] = '0';

        randomHeigth = std::stoi(hString);
    }

    return {randomWidth, randomHeigth};
}

bool apple::spawn(u_int16_t windowWidth, u_int16_t windowHeight, SDL_Renderer *renderer, SDL_FRect &apple, SDL_FRect &player)
{
    bool firstSpawn = apple.x == 0 && apple.y == 0;

    bool playerCaught = (player.x < apple.x + APPLE_SIZE) &&
                        (player.x + PLAYER_SIZE > apple.x) &&
                        (player.y < apple.y + APPLE_SIZE) &&
                        (player.y + PLAYER_SIZE > apple.y);

    if (playerCaught)
        std::cout << "Player caught the apple!\n";

    if (playerCaught || firstSpawn)
    {
        std::cout << "Apple will spawn in different location!\n";

        auto location = apple::choose_location(windowWidth, windowHeight);

        apple.x = location[0];
        apple.y = location[1];

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color for the apple
        SDL_RenderFillRect(renderer, &apple);

        std::cout << "Apple spawned!\n";

        std::cout << "Apple X: " << apple.x << std::endl;
        std::cout << "Apple Y: " << apple.y << std::endl;

        return playerCaught;
    }
    else
    {
        // std::cout << "Apple is still there!\n";

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color for the apple
        SDL_RenderFillRect(renderer, &apple);
    }

    return false;
}
