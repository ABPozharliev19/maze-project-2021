#define OLC_PGE_APPLICATION
#define OLC_PGEX_TRANSFORMEDVIEW
#include "../../include/backend/gameManager.h"

bool GameManager::OnUserCreate()
{
    sAppName = "VAVYLON";
    return true;
}

bool GameManager::OnUserUpdate(float fElapsedTime)
{
    return true;
}