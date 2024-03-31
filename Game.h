#pragma once
#include <SDL.h>
#include <string.h>
#include <random>

constexpr int WINDOW_W = 800;
constexpr int WINDOW_H = 600;

constexpr int BLOCK_W = 5;
constexpr int BLOCK_H = 5;

constexpr int MAP_W = WINDOW_W / BLOCK_W;
constexpr int MAP_H = WINDOW_H / BLOCK_H;

constexpr int BLOCK_MAX = MAP_W * MAP_H;

const char img[7][30]{ "assets/background.png",
                       "assets/block_red_s.png",
                       "assets/block_orange_s.png",
                       "assets/block_yellow_s.png",
                       "assets/block_green_s.png",
                       "assets/block_blue_s.png",
                       "assets/block_purple_s.png" };

bool isRunning = true;

int map[MAP_W][MAP_H]{};
int Radius = 5;

constexpr int FPS = 60;
constexpr int frameDelay = 1000 / FPS;
constexpr int updateCount = 1;

Uint32 frameStart, frameTime, frameCount = 0;

struct Block
{
    int x, y;
    int blockIdx;
    bool onScreen;
    bool updatable;
    SDL_Rect rect;
    SDL_Texture* texture;

} block[BLOCK_MAX];

static int curBlockIdx = 0;

void putBlock(int mousePosX, int mousePosY, int i, int j, int Radius, SDL_Texture* blockTexture)
{
    int x = mousePosX / BLOCK_W + i;
    int y = mousePosY / BLOCK_H + j;

    if (curBlockIdx < BLOCK_MAX && map[x][y] == 0 && !(x < 0) && !(y < 0) && (x < MAP_W) && (y < MAP_H))
    {
        map[x][y] = 1;
        block[curBlockIdx].x = x;
        block[curBlockIdx].y = y;
        block[curBlockIdx].blockIdx = curBlockIdx;
        block[curBlockIdx].onScreen = true;
        block[curBlockIdx].updatable = true;
        block[curBlockIdx].rect.x = block[curBlockIdx].x * BLOCK_W;
        block[curBlockIdx].rect.y = block[curBlockIdx].y * BLOCK_H;
        block[curBlockIdx].rect.w = BLOCK_W;
        block[curBlockIdx].rect.h = BLOCK_H;
        block[curBlockIdx].texture = blockTexture;

        curBlockIdx++;
    }
}

void putBlocks(int mousePosX, int mousePosY, int Radius, SDL_Texture* blockTexture)
{
    // In window area
    if (!(mousePosX < 0) && !(mousePosY < 0) && (mousePosX < WINDOW_W) && (mousePosY < WINDOW_H))
    {
        for (int i = -Radius; i < Radius; i++)
        {
            int y = (int)sqrt(Radius * Radius - i * i);
            if (Radius > 1)
            {
                for (int j = -y; j < y; j++)
                {
                    putBlock(mousePosX, mousePosY, i, j, Radius, blockTexture);
                }
            }
            else
            {
                for (int j = 0; j < y; j++)
                {
                    putBlock(mousePosX, mousePosY, i, j, Radius, blockTexture);
                }
            }

        }
    }
}

void dropBlock(Block& block)
{
    map[block.x][block.y] = 0;
    block.y++;
    map[block.x][block.y] = 1;
    block.rect.y += BLOCK_H;
}

int checkTumbledDown(Block& block)
{
    if (block.y < MAP_H - 1)
    {
        int blockBelow = map[block.x][block.y + 1];
        if (blockBelow == 0)
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }

    int leftTumbledDown, rightTumbledDown;
    if (block.x > 0 && block.y < MAP_H - 2)
    {
        leftTumbledDown = map[block.x - 1][block.y + 0] + map[block.x - 1][block.y + 1] + map[block.x - 1][block.y + 2];
    }
    else
    {
        leftTumbledDown = 1;
    }

    if (block.x < MAP_W - 1 && block.y < MAP_H - 2)
    {
        rightTumbledDown = map[block.x + 1][block.y + 0] + map[block.x + 1][block.y + 1] + map[block.x + 1][block.y + 2];
    }
    else
    {
        rightTumbledDown = 1;
    }

    if (leftTumbledDown + rightTumbledDown == 0)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(-1, 1);

        return dis(gen);
    }
    else if (leftTumbledDown == 0 && block.x != 0)
    {
        return -1;
    }
    else if (rightTumbledDown == 0 && block.x != MAP_W - 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void checkStop(Block& block)
{
    // tumble down
    int x_shift = checkTumbledDown(block);
    if (x_shift != 0)
    {
        map[block.x][block.y] = 0;
        block.x += x_shift;
        map[block.x][block.y] = 1;
        block.rect.x += x_shift * BLOCK_W;
    }

    // on the block
    if (block.y < MAP_H - 1)
    {
        int blockBelow = map[block.x][block.y + 1];
        if (blockBelow == 1)
        {
            block.updatable = false;
        }
        else
        {
            block.updatable = true;
        }
    }

    // on the bottom
    if (block.y == MAP_H - 1)
    {
        block.updatable = false;
    }
}

void clearBlock()
{
    for (int i = 0; i < curBlockIdx; i++)
    {
        block[curBlockIdx].onScreen = false;
        block[curBlockIdx].updatable = false;
    }
    memset(map, 0, sizeof(int) * BLOCK_MAX);
    curBlockIdx = 0;
}

void setBlockColor(SDL_Texture* &des, SDL_Texture* &src)
{
    des = src;
}