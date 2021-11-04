#define OLC_PGE_APPLICATION
#include "libs/olcPixelGameEngine.h"

#define OLC_PGEX_ANIMSPR
#include "libs/olcPGEX_AnimatedSprite.h"

#include "include/frontend/mainMenu.h"
#include "include/frontend/lobby.h"
#include "include/frontend/room.h"
#include "include/frontend/player.h"
#include "include/backend/collisions.h"
#include <iostream>

class GameManager : public olc::PixelGameEngine
{

public:
  MainMenu *mainMenu;
  Player *player;
  Lobby *lobby;
  Room *room;
  Collisions *collisions;

  olc::Sprite* cutsceneSprite;
  olc::Decal* cutscene;
  bool inCutscene;

public:
  GameManager()
  {
    mainMenu = new MainMenu;
    player = new Player;
    lobby = new Lobby;
    room = new Room(24, 12);
    collisions = new Collisions;
  }

private:
  bool OnUserCreate() override
  {
    cutsceneSprite = new olc::Sprite("public/Cutscene.png");
    cutscene = new olc::Decal(cutsceneSprite);
    inCutscene = false;

    player->innitPlayer();
    player->playerX = ScreenWidth() / 2;
    player->playerY = ScreenHeight() / 2;
    player->playerRadius = 5.0f;
    player->playerPos = {player->playerX, player->playerY};
    player->playerSpeed = 60.0f;
    player->playerVelX = 0.0f;
    player->playerVelY = 0.0f;

    mainMenu->mainMenuEnabled = true;
    mainMenu->pauseMenuEnabled = false;
    mainMenu->optionsMenuEnabled = false;
    mainMenu->menuOption = 0;
    mainMenu->gameStarted = false;
    mainMenu->anyKeyPressed = false;
    mainMenu->welcomeLogo = new olc::Sprite("public/VAVYLON_LOGO_BIG_NOBG.png");
    mainMenu->welcomeLogoDecal = new olc::Decal(mainMenu->welcomeLogo);

    lobby->inMaze = false;
    lobby->lobbyRoom = new olc::Sprite("public/lobby.png");
    lobby->lobbyForegroundSprite = new olc::Sprite("public/foreGroundLobby.png");
    lobby->lobbyForeground = new olc::Decal(lobby->lobbyForegroundSprite);

    room->runeSprite = new olc::Sprite("public/rune.png");
    room->rune = new olc::Decal(room->runeSprite);
    room->runeTileSprite = new olc::Sprite("public/rune_tile.png");
    room->runeTile = new olc::Decal(room->runeTileSprite);
    room->scroll = new olc::Sprite("public/scroll.png");
    room->scrollDecal = new olc::Decal(room->scroll);

    collisions->colliding = false;

    return true;
  }

  bool OnUserUpdate(float fElapsedTime) override
  {

    Clear(olc::BLACK);

    if (mainMenu->gameStarted)
    {
      if(!inCutscene)
      {
        Game(fElapsedTime);
      }
      else
      {
        Cutscene(fElapsedTime);
      }
    }

    if (mainMenu->anyKeyPressed)
    {
      if (mainMenu->mainMenuEnabled)
      {
        mainMenu->displayMainMenu(this);
      }

      if (mainMenu->pauseMenuEnabled)
      {
        mainMenu->displayPauseMenu(this);
      }

      if (mainMenu->optionsMenuEnabled && mainMenu->mainMenuEnabled == false)
      {
        mainMenu->displayOptionsMenu(this);
      }
    }
    else
    {
      mainMenu->pressAnyKey(this);
    }

    return true;
  }

  void Game(float fElapsedTime)
  {
    getInput(fElapsedTime);

    if (!mainMenu->pauseMenuEnabled && !mainMenu->mainMenuEnabled && !mainMenu->optionsMenuEnabled)
    {
      lobby->drawLobby(this, player, room);
      if (!player->firstPlayerMove)
      {
        collisions->checkCollisions(this, player, lobby, room);
      }
      player->playerPos = {player->playerX, player->playerY};
      player->drawPlayer(this, fElapsedTime);
      if(!lobby->inMaze)
      {
        lobby->drawLobbyForeground(this);
      }
    }
  }

  void Cutscene(float fElapsedTime)
  {
    getInput(fElapsedTime);

    if (!mainMenu->pauseMenuEnabled && !mainMenu->mainMenuEnabled && !mainMenu->optionsMenuEnabled)
    {
      DrawDecal({0.0f - player->playerX, 0.0f - player->playerY}, cutscene);
      player->playerPos = {player->playerX, player->playerY};
      player->playerSpeed = 30.0f;
      // if (!player->firstPlayerMove)
      // {
      //   collisions->checkCollisions(player, lobby, room);
      // }
      player->drawPlayer(this, fElapsedTime);
    }
    
  }

  void getInput(float elapsedTime)
  {
    if (this->GetKey(olc::LEFT).bHeld && mainMenu->pauseMenuEnabled == false && mainMenu->mainMenuEnabled == false && collisions->lastCollisionDir != player->PLAYER_DIRS::LEFT)
    {
      player->movePlayer(this, olc::LEFT, elapsedTime);
    }

    if (this->GetKey(olc::LEFT).bReleased)
    {
      player->playerSpeed = 60;
      player->Animator->SetState("leftIdle");
    }

    if (this->GetKey(olc::RIGHT).bHeld && mainMenu->pauseMenuEnabled == false && mainMenu->mainMenuEnabled == false && collisions->lastCollisionDir != player->PLAYER_DIRS::RIGHT)
    {
      player->movePlayer(this, olc::RIGHT, elapsedTime);
    }
    if (GetKey(olc::RIGHT).bReleased)
    {
      player->playerSpeed = 60;
      player->Animator->SetState("rightIdle");
    }

    if (this->GetKey(olc::UP).bHeld && mainMenu->pauseMenuEnabled == false && mainMenu->mainMenuEnabled == false && collisions->lastCollisionDir != player->PLAYER_DIRS::UP)
    {
      player->movePlayer(this, olc::UP, elapsedTime);
    }
    if (GetKey(olc::UP).bReleased)
    {
      player->playerSpeed = 60;
      player->Animator->SetState("upIdle");
    }

    if (GetKey(olc::DOWN).bHeld && mainMenu->pauseMenuEnabled == false && mainMenu->mainMenuEnabled == false && collisions->lastCollisionDir != player->PLAYER_DIRS::DOWN)
    {
      player->movePlayer(this, olc::DOWN, elapsedTime);
    }
    if (GetKey(olc::DOWN).bReleased)
    {
      player->playerSpeed = 60;
      player->Animator->SetState("downIdle");
    }

    if (GetKey(olc::ESCAPE).bPressed && !mainMenu->mainMenuEnabled && !mainMenu->optionsMenuEnabled)
    {
      if (!mainMenu->pauseMenuEnabled)
      {
        mainMenu->pauseMenuEnabled = true;
      }
      else
      {
        mainMenu->pauseMenuEnabled = false;
      }
    }
  }
};

//GO BIG OR GO HOME

int main()
{
  olc::PixelGameEngine engine;
  GameManager gameManager;

  gameManager.mainMenu->saveFile.open("data/saveFile.save");
  if (gameManager.mainMenu->saveFile.is_open())
  {
    while (getline(gameManager.mainMenu->saveFile, gameManager.mainMenu->line))
    {
      if (gameManager.mainMenu->line == "true")
      {
        gameManager.mainMenu->fullScreen = true;
      }
      else
      {
        gameManager.mainMenu->fullScreen = false;
      }
    }
    gameManager.mainMenu->saveFile.close();
  }

  if (gameManager.Construct(engine.ScreenWidth() * 1.5f - 46, engine.ScreenHeight() - 46, 5, 5, gameManager.mainMenu->fullScreen))
    // if (gameManager.Construct(engine.ScreenWidth() * 1.875f - 46, engine.ScreenHeight() * 1.125 - 46, 4, 4, gameManager.mainMenu->fullScreen))
    gameManager.Start();
}