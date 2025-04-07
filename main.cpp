#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* numbers[11];
SDL_Texture* background = NULL;

int board[4][4] = {0};

SDL_Texture* loadTexture(const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        cerr << "Error loading image: " << path << " " << IMG_GetError() << endl;
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void loadTextures() {
    background = loadTexture("background.png");
    const char* filenames[] = { "so2.png", "so4.png", "so8.png", "so16.png", "so32.png",
                                "so64.png", "so128.png", "so256.png", "so512.png",
                                "so1024.png", "so2048.png" };

    for (int i = 0; i < 11; i++) {
        numbers[i] = loadTexture(filenames[i]);
    }
}

void spawnNumber() {
    int emptyCells[16][2];
    int count = 0;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (board[i][j] == 0) {
                emptyCells[count][0] = i;
                emptyCells[count][1] = j;
                count++;
            }
        }
    }

    if (count > 0) {
        int r = rand() % count;
        board[emptyCells[r][0]][emptyCells[r][1]] = (rand() % 2 + 1) * 2;
    }
}

void moveLeft() {
    for (int i = 0; i < 4; i++) {
        int temp[4] = {0}, index = 0;

        for (int j = 0; j < 4; j++) {
            if (board[i][j] != 0) {
                if (index > 0 && temp[index - 1] == board[i][j]) {
                    temp[index - 1] *= 2;
                } else {
                    temp[index++] = board[i][j];
                }
            }
        }

        for (int j = 0; j < 4; j++) {
            board[i][j] = temp[j];
        }
    }
    spawnNumber();
}

void moveRight() {
    for (int i = 0; i < 4; i++) {
        int temp[4] = {0}, index = 3;

        for (int j = 3; j >= 0; j--) {
            if (board[i][j] != 0) {
                if (index < 3 && temp[index + 1] == board[i][j]) {
                    temp[index + 1] *= 2;
                } else {
                    temp[index--] = board[i][j];
                }
            }
        }

        for (int j = 0; j < 4; j++) {
            board[i][j] = temp[j];
        }
    }
    spawnNumber();
}

void moveUp() {
    for (int j = 0; j < 4; j++) {
        int temp[4] = {0}, index = 0;

        for (int i = 0; i < 4; i++) {
            if (board[i][j] != 0) {
                if (index > 0 && temp[index - 1] == board[i][j]) {
                    temp[index - 1] *= 2;
                } else {
                    temp[index++] = board[i][j];
                }
            }
        }

        for (int i = 0; i < 4; i++) {
            board[i][j] = temp[i];
        }
    }
    spawnNumber();
}

void moveDown() {
    for (int j = 0; j < 4; j++) {
        int temp[4] = {0}, index = 3;

        for (int i = 3; i >= 0; i--) {
            if (board[i][j] != 0) {
                if (index < 3 && temp[index + 1] == board[i][j]) {
                    temp[index + 1] *= 2;
                } else {
                    temp[index--] = board[i][j];
                }
            }
        }

        for (int i = 0; i < 4; i++) {
            board[i][j] = temp[i];
        }
    }
    spawnNumber();
}

void renderBoard() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, background, NULL, NULL);

    SDL_Rect dstRect;
    dstRect.w = 100;
    dstRect.h = 100;

    int posX = 50, posY = 50;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (board[i][j] != 0) {
                int index = -1;
                switch (board[i][j]) {
                    case 2: index = 0; break;
                    case 4: index = 1; break;
                    case 8: index = 2; break;
                    case 16: index = 3; break;
                    case 32: index = 4; break;
                    case 64: index = 5; break;
                    case 128: index = 6; break;
                    case 256: index = 7; break;
                    case 512: index = 8; break;
                    case 1024: index = 9; break;
                    case 2048: index = 10; break;
                }

                if (index != -1) {
                    dstRect.x = posX + j * 110;
                    dstRect.y = posY + i * 110;
                    SDL_RenderCopy(renderer, numbers[index], NULL, &dstRect);
                }
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void cleanUp() {
    for (int i = 0; i < 11; i++) {
        if (numbers[i]) SDL_DestroyTexture(numbers[i]);
    }
    if (background) SDL_DestroyTexture(background);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    srand(time(0));

    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("2048 Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    IMG_Init(IMG_INIT_PNG);
    loadTextures();

    spawnNumber();

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_LEFT: moveLeft(); break;
                    case SDLK_RIGHT: moveRight(); break;
                    case SDLK_UP: moveUp(); break;
                    case SDLK_DOWN: moveDown(); break;
                }
            }
        }
        renderBoard();
    }

    cleanUp();
    return 0;
}
