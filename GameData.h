#ifndef _GAMEDATA_H_
#define _GAMEDATA_H_

#include "cocos2d.h"

USING_NS_CC;

class GameData
{
public:
    static GameData* getInstance();

    void changeMoney(int moneyChange);

    int getMoney();
    

private:
    int money;
};

#endif
