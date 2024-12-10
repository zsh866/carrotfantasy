#include "GameData.h"

GameData* GameData::getInstance() 
{
    static GameData instance;
    return &instance;
}

void GameData::changeMoney(int moneyChange)
{
    money += moneyChange;
}

int  GameData::getMoney()
{
    return money;
}