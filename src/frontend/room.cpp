#include "../../include/frontend/room.h"

Room::~Room()
{
    delete[] rightMaze;
    delete[] leftMaze;
    delete mazeTile;
    delete runeSprite;
    delete rune;
    delete runeTileSprite;
    delete scroll;
    delete scrollDecal;
    delete scrollAnimSpritesheet;
    delete scrollAnimator;
}

Room::Room(int mWidth, int mHeight)
{
    mazeWidth = mWidth;
    mazeHeight = mHeight;
    tileWidth = 13;
    tileHeight = 13;

    roomEnriched = false;

    rightMaze = new CheckedMaze(mazeWidth, mazeHeight);
    rightMaze->checkMaze();
    leftMaze = new CheckedMaze(mazeWidth, mazeHeight);
    leftMaze->checkMaze();

    mazeTile = new olc::Sprite("public/tile.png");

    generateRoom();
    
}

void Room::setUpScrollAnimations()
{
    scrollAnimSpritesheet = new olc::Renderable();
    scrollAnimSpritesheet->Load("public/Scroll_Anim_Spritesheet.png");
    scrollAnimator = new olc::AnimatedSprite();
    scrollAnimator->mode = olc::AnimatedSprite::SPRITE_MODE::SINGLE;
    scrollAnimator->type = olc::AnimatedSprite::SPRITE_TYPE::DECAL;
    scrollAnimator->spriteSheet = scrollAnimSpritesheet;
    scrollAnimator->SetSpriteSize({25, 25});
    scrollAnimator->SetSpriteScale(0.50f);

    scrollAnimator->AddState("idle", 0.20f, olc::AnimatedSprite::PLAY_MODE::LOOP, std::vector<olc::vi2d>{ 
        {0, 0},
        {25, 0},
        {50, 0},
        {75, 0},
        {100, 0},
        {125, 0}
    });

    scrollAnimator->AddState("pickedUp", 0.20f, olc::AnimatedSprite::PLAY_MODE::LOOP, std::vector<olc::vi2d>{ 
        {150, 0}
    });

    scrollAnimator->SetState("idle");
}

void Room::generateRoom()
{

    rawMazeData.open("data/leftRawMazeData.maze", std::ofstream::out);

    for (int i = 0; i < mazeHeight; i++)
    {
        for (int j = 0; j < mazeWidth; j++)
        {
            if (leftMaze->checkedMaze[i * mazeWidth + j] == '#')
            {
                rawMazeData << "1";
            }
            else if (leftMaze->checkedMaze[i * mazeWidth + j] == ' ')
            {
                rawMazeData << "0";
            }
        }
        rawMazeData << '\n';
    }

    rawMazeData.close();

    leftMaze->checkedMaze[1 * mazeWidth + 1] = 'R';

    rawMazeData.open("data/rightRawMazeData.maze", std::ofstream::out);

    for (int i = 0; i < mazeHeight; i++)
    {
        for (int j = 0; j < mazeWidth; j++)
        {
            if (rightMaze->checkedMaze[i * mazeWidth + j] == '#')
            {
                rawMazeData << "1";
            }
            else if (rightMaze->checkedMaze[i * mazeWidth + j] == ' ')
            {
                rawMazeData << "0";
            }
        }
        rawMazeData << '\n';
    }

    rawMazeData.close();

    rightMaze->checkedMaze[(mazeHeight - 1) * mazeWidth + (mazeWidth - 1)] = 'R';

}

char Room::getTile(std::string mazeOrientation, int x, int y)
{
    return (mazeOrientation == "left") ? leftMaze->checkedMaze[y * mazeWidth + x] : rightMaze->checkedMaze[y * mazeWidth + x];
}

void Room::DrawRoom(olc::PixelGameEngine *engine, std::string mazeOrientation)
{

    if (mazeOrientation == "left")
    {
        for (int x = 0; x <= mazeWidth; x++)
        {
            for (int y = 0; y <= mazeHeight; y++)
            {
                char currentTile = getTile("left", x, y);

                if(currentTile == 'R')
                {
                    engine->DrawSprite({x * tileWidth, y * tileHeight}, runeTileSprite);
                    if(!leftRunePickedUp) { engine->DrawDecal({x * tileWidth + 1.5f, y * tileHeight}, rune, {0.55f, 0.55f}); }
                }

                if(currentTile == ' ')
                {
                    engine->DrawSprite(x * tileWidth, y * tileHeight, mazeTile);
                }

                if(currentTile == 'P')
                {
                    if(!roomEnriched)
                    {
                        items_vec.push_back({x, y, "path", false});
                    }
                    for(auto i : items_vec)
                    {
                        if(i.x == x && i.y == y && !i.pickedUp)
                        {
                            engine->DrawSprite(x * tileWidth, y * tileHeight, mazeTile);
                            scrollAnimator->Draw(engine->GetElapsedTime(), {x * tileWidth, y * tileHeight});
                        }
                    }
                }

                if(currentTile == 'D')
                {
                    if(!roomEnriched)
                    {
                        items_vec.push_back({x, y, "dash", false});
                    }
                    for(auto i : items_vec)
                    {
                        if(i.x == x && i.y == y && !i.pickedUp)
                        {
                            engine->DrawSprite(x * tileWidth, y * tileHeight, mazeTile);
                            scrollAnimator->Draw(engine->GetElapsedTime(), {x * tileWidth, y * tileHeight});
                        }
                    }
                }

                if(currentTile == 'S')
                {
                    if(!roomEnriched)
                    {
                        items_vec.push_back({x, y, "speed", false});
                    }
                    for(auto i : items_vec)
                    {
                        if(i.x == x && i.y == y && !i.pickedUp)
                        {
                            engine->DrawSprite(x * tileWidth, y * tileHeight, mazeTile);
                            scrollAnimator->Draw(engine->GetElapsedTime(), {x * tileWidth, y * tileHeight});
                        }
                    }
                }
            }
        }
    }
    else
    {
        for (int x = 0; x < mazeWidth; x++)
        {
            for (int y = 0; y < mazeHeight; y++)
            {
                char currentTile = getTile("right", x, y);

                if(currentTile == 'R')
                {
                    engine->DrawSprite({x * tileWidth, y * tileHeight}, runeTileSprite);
                    
                    if(!rightRunePickedUp) { engine->DrawDecal({x * tileWidth + 1.5f, y * tileHeight}, rune, {0.55f, 0.55f}); }
                }

                if(currentTile == ' ')
                {
                    engine->DrawSprite(x * tileWidth, y * tileHeight, mazeTile);
                }

                if(currentTile == 'P')
                {
                    if(!roomEnriched)
                    {
                        items_vec.push_back({x, y, "path", false});
                    }
                    for(auto i : items_vec)
                    {
                        if(i.x == x && i.y == y && !i.pickedUp)
                        {
                            engine->DrawSprite(x * tileWidth, y * tileHeight, mazeTile);
                            scrollAnimator->Draw(engine->GetElapsedTime(), {x * tileWidth, y * tileHeight});
                        }
                    }
                }

                if(currentTile == 'D')
                {
                    if(!roomEnriched)
                    {
                        items_vec.push_back({x, y, "dash", false});
                    }
                    for(auto i : items_vec)
                    {
                        if(i.x == x && i.y == y && !i.pickedUp)
                        {
                            engine->DrawSprite(x * tileWidth, y * tileHeight, mazeTile);
                            scrollAnimator->Draw(engine->GetElapsedTime(), {x * tileWidth, y * tileHeight});
                        }
                    }
                }

                if(currentTile == 'S')
                {
                    if(!roomEnriched)
                    {
                        items_vec.push_back({x, y, "speed", false});
                    }
                    for(auto i : items_vec)
                    {
                        if(i.x == x && i.y == y && !i.pickedUp)
                        {
                            engine->DrawSprite(x * tileWidth, y * tileHeight, mazeTile);
                            scrollAnimator->Draw(engine->GetElapsedTime(), {x * tileWidth, y * tileHeight});
                        }
                    }
                }
            }
        }
    }

    roomEnriched = true;
}