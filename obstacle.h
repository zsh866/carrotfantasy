#pragma once

#ifndef _OBSTACLE_H
#define _OBSTACLE_H
#include "cocos2d.h"

class Obstacle:public cocos2d::Sprite {
	std::string obstacleType;
	int rewardmoney;
	int hp;
	int fullHp;
public:
	static Obstacle* create();
	virtual bool init();
	void getAttack(int damage);
	void dead();
	std::string getObstacleName(const std::string name,int type);
};


#endif


