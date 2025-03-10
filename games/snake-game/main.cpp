// STL
#include <iostream>
#include <vector>

// SDL3
#include <SDL3/SDL.h>

// Local
#include "./src/defs.h"
#include "./src/apple.cc"

using namespace std;

int main()
{
    SDL_Window *window;
    SDL_Renderer *renderer;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        cout << "Error when try to init SDL!\n";
        cout << SDL_GetError() << "\n";
    }

    window = SDL_CreateWindow(
        NAME_PROJECT,
        1920,
        1080,
        SDL_WINDOW_FULLSCREEN);

    if (!window)
    {
        cout << "Error on the window!\n";
        cout << SDL_GetError() << "\n";
        return 1;
    }

    SDL_SetWindowFullscreen(window, true);
    SDL_SyncWindow(window);

    renderer = SDL_CreateRenderer(window, NULL);
    SDL_SetRenderVSync(renderer, 1);

    bool running = true;

    int windowHeight{0};
    int windowWidth{0};

    if (!SDL_GetWindowSizeInPixels(window, &windowWidth, &windowHeight))
    {
        cout << "Error to get window height!\n";
        cout << SDL_GetError() << "\n";
        return 1;
    }

    SDL_FRect player{windowWidth / 2, windowHeight / 2, PLAYER_SIZE, PLAYER_SIZE};
    vector<uint8_t> colorPlayer{255, 255, 255};

    SDL_FRect apple{0, 0, APPLE_SIZE, APPLE_SIZE};

    u_int16_t score{0};
    std::vector<SDL_FRect> historyLocations{};

    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {

            // cout << "Player X: " << player.x << "\n";
            // cout << "Player Y: " << player.y << "\n";

            if (event.type == SDL_EVENT_QUIT || (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE))
                running = false;

            else if (event.type == SDL_EVENT_KEY_DOWN)
            {
                bool moved{false};
                if (event.key.scancode == SDL_SCANCODE_UP)
                {
                    if (player.y >= 20)
                    {
                        player.y -= 20;
                        moved = true;
                    }
                }
                else if (event.key.scancode == SDL_SCANCODE_DOWN)
                {
                    if (player.y + PLAYER_MOVE_OFFSET + player.h <= windowHeight)
                    {
                        player.y += PLAYER_MOVE_OFFSET;
                        moved = true;
                    }
                }
                else if (event.key.scancode == SDL_SCANCODE_LEFT)
                {
                    if (player.x - PLAYER_MOVE_OFFSET >= 0)
                    {
                        player.x -= PLAYER_MOVE_OFFSET;
                        moved = true;
                    }
                }
                else if (event.key.scancode == SDL_SCANCODE_RIGHT)
                {
                    if (player.x + PLAYER_MOVE_OFFSET + player.w <= windowWidth)
                    {
                        player.x += PLAYER_MOVE_OFFSET;
                        moved = true;
                    }
                }

                if (moved)
                    historyLocations.push_back(player);
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        bool playerCaugth = apple::spawn(windowWidth, windowHeight, renderer, apple, player);

        if (playerCaugth)
            score++;

        if (historyLocations.size() > score + 1)
        {
            cout << "Score: " << score << "\n";
            cout << "History Locations Size: " << historyLocations.size() - 1 << "\n";
            historyLocations.erase(historyLocations.begin());
        }

        SDL_SetRenderDrawColor(renderer, colorPlayer[0], colorPlayer[1], colorPlayer[2], 255);
        SDL_RenderFillRect(renderer, &player);

        std::vector<u_int8_t> copyColorPlayer{colorPlayer};

        int index{0};
        for (auto location : historyLocations)
        {
            if (index == historyLocations.size() - 1)
                break;

            copyColorPlayer[0] -= 10;
            copyColorPlayer[1] -= 10;
            copyColorPlayer[2] -= 10;

            SDL_SetRenderDrawColor(renderer, copyColorPlayer[0], copyColorPlayer[1], copyColorPlayer[2], 255);
            SDL_RenderFillRect(renderer, &location);

            index++;
        }

        // Atualiza a janela
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    cout << "Exit program\n";

    return 0;
}
