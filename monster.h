#pragma once
#ifndef _MONSTER_H_
#define _MONSTER_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <vector>
using namespace cocos2d;
using namespace cocos2d::ui;

class Monster:public Sprite{
public:
	int hitpoint; 
	int speed;
	//int attackrange;
	bool dead=false;

    LoadingBar* _hp;

public:

    //构造函数，初始化怪物
   /* Monster(int type);*/

    //初始化怪物对象
    bool init(int type);

    static Monster* createMonster(int type);

    //受到伤害
    void gethit(int damage);

    //按路径移动
    void movepath(const std::vector<cocos2d::Vec2>& path);

    //处理怪物死亡
    void isdead();

    void updateHealthBar();

    //void showexplosionScene();   爆炸特效

};

#endif
