#include "../../include/frontend/mainMenu.h"

bool MainMenu::displayPauseMenu(olc::PixelGameEngine* engine)
    {
      std::string menuOptionsArr[2] = {"Continue", "Quit To Menu"};
      engine->Clear(olc::BLACK);
      int vSpacing = -16;

      for(int i = 0; i < 2; i++)
      {
        if(i == menuOption)
        {
          engine->DrawString(engine->ScreenWidth() / 2 - 56, engine->ScreenHeight() / 2 + vSpacing, "-> " + menuOptionsArr[i], olc::DARK_GREY);
        } else {
          engine->DrawString(engine->ScreenWidth() / 2 - 32, engine->ScreenHeight() / 2 + vSpacing, menuOptionsArr[i], olc::WHITE);
        }

        vSpacing += 32;
      }

      getMenuInput(engine, "pause");
      vSpacing = -16;
      return true;
    }

bool MainMenu::displayMainMenu(olc::PixelGameEngine* engine)
  {
    std::string menuOptionsArr[3] = {"Start Game", "Options", "Quit Game"};
    engine->Clear(olc::BLACK);
    int spacing = -32;

    for(int i = 0; i < 3; i++)
    {
      if(i == menuOption)
      {
        engine->DrawString(engine->ScreenWidth() / 2 - 64, engine->ScreenHeight() / 2 + spacing, "-> " + menuOptionsArr[i], olc::DARK_GREY);
      } else {
        engine->DrawString(engine->ScreenWidth() / 2 - 40, engine->ScreenHeight() / 2 + spacing, menuOptionsArr[i], olc::WHITE);
      }

      spacing += 32;
    }
    getMenuInput(engine, "main");
    mainMenuEdit = true;
    spacing = -32;
    return true;
  }

bool MainMenu::displayOptionsMenu(olc::PixelGameEngine* engine)
  {
    std::string menuOptionsArr[4] = {"Full Screen", "Sound", "Controls", "Back"};
    engine->Clear(olc::BLACK);
    int spacing = -32;

    for(int i = 0; i < 4; i++)
    {
      if(i == menuOption)
      {
        if(menuOptionsArr[i] == "Full Screen")
        {
          engine->DrawString(engine->ScreenWidth() / 2 - 64, engine->ScreenHeight() / 2 + spacing, "-> " + menuOptionsArr[i] + ": " + (fullScreen == true ? "on" : "off"), olc::DARK_GREY);
        } else {
          engine->DrawString(engine->ScreenWidth() / 2 - 64, engine->ScreenHeight() / 2 + spacing, "-> " + menuOptionsArr[i], olc::DARK_GREY);
        }
        
      } else if(menuOptionsArr[i] == "Full Screen"){
        engine->DrawString(engine->ScreenWidth() / 2 - 40, engine->ScreenHeight() / 2 + spacing, menuOptionsArr[i] + ": " + (fullScreen == true ? "on" : "off"), olc::WHITE);
      } else {
        engine->DrawString(engine->ScreenWidth() / 2 - 40, engine->ScreenHeight() / 2 + spacing, menuOptionsArr[i], olc::WHITE);
      }

      spacing += 32;
    }
    getMenuInput(engine, "options");
    optionsMenuEdit = true;
    spacing = -32;
    return true;
  }

void MainMenu::getMenuInput(olc::PixelGameEngine* engine, std::string menuType)
{
    if(menuType == "main")
    {
        optionsMenuEdit = false;

        if(engine->GetKey(olc::DOWN).bPressed)
        {
            std::cout<<"down";
          if(menuOption == 2)
          {
            menuOption = 0;
          } else {
            menuOption+= 1;
          }
        }

        if(engine->GetKey(olc::UP).bPressed)
        {
          if(menuOption == 0)
          {
            menuOption = 2;
          } else {
            menuOption -= 1;
          }
        }

        if(engine->GetKey(olc::ENTER).bPressed)
        {
          if(menuOption == 0 && mainMenuEdit){
            mainMenuEnabled = false;
            gameStarted = true;
            menuOption = 0; // reset the selected option after quitting the menu
          }
          if(menuOption == 1){
            menuOption = 0;
            optionsMenuEnabled = true;
            mainMenuEnabled = false;
          }
          if(menuOption == 2){
            exit(0);
          }
        }
    }

    if(menuType == "options")
    {
        if(engine->GetKey(olc::DOWN).bPressed)
        {
          if(menuOption == 3)
          {
            menuOption = 0;
          } else {
            menuOption+= 1;
          }
        }

        if(engine->GetKey(olc::UP).bPressed)
        {
          if(menuOption == 0)
          {
            menuOption = 3;
          } else {
            menuOption -= 1;
          }
        }

        if(engine->GetKey(olc::ENTER).bPressed && optionsMenuEdit)
        {
          if(menuOption == 0){
            if(fullScreen == false){
              saveFile.open ("data/saveFile.save", std::ofstream::out | std::ofstream::trunc);
              saveFile << "true";
              saveFile.close();
              std::cout<<"true";
              fullScreen = true;
              exit(0);
            } else{
              saveFile.open ("data/saveFile.save", std::ofstream::out | std::ofstream::trunc);
              saveFile << "false";
              saveFile.close();
              std::cout<<"false";
              fullScreen = false;
              exit(0);
            }
          }

          if(menuOption == 1){
            // sound
            std::cout<<"sound";
          }

          if(menuOption == 2){
            // controls
            std::cout<<"controls";
          }

          if(menuOption == 3){
            optionsMenuEnabled = false;
            mainMenuEnabled = true;
            menuOption = 0;
          }
        }
    }

    if(menuType == "pause")
    {
        mainMenuEdit = false;
        
        if(engine->GetKey(olc::DOWN).bPressed)
        {
          if(menuOption == 1)
          {
            menuOption = 0;
          } else {
          menuOption+= 1;
          }
        }

        if(engine->GetKey(olc::UP).bPressed)
        {
          if(menuOption == 0)
          {
            menuOption = 1;
          } else {
            menuOption -= 1;
          }
        }

        if(engine->GetKey(olc::ENTER).bPressed)
        {
          if(menuOption == 0){
            pauseMenuEnabled = false;
            menuOption = 0; // reset the selected option after quitting the menu
          }
          if(menuOption == 1){
            optionsMenuEnabled = false;
            mainMenuEnabled = true;
            pauseMenuEnabled = false;
            menuOption = 0;
          }
        }
    }
}