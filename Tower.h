#ifndef _TOWER_H_
#define _TOWER_H_

#include "cocos2d.h"

USING_NS_CC;

class Bullet;
class Monster;

class Tower : public Sprite
{
public:
	std::string type;//塔的种类
	int level;//等级
	int buildMoney[3];//建造和升级金钱
	int sellMoney[3];//出售金钱
	int attackNumber[3];//攻击力
	float attackRange;//攻击范围
	float attackSpeed;//攻击速度
	float bulletFlySpeed;//子弹飞行速度
	bool isChosed;//是否被选中

	Sprite* target;
public:
	static Tower* buildTower();

	virtual bool init();//初始化

	void sellTower();//出售

	void levelUp();//升级

	Bullet* aimAndShoot();//瞄准并射击
};

std::string getTowerFilename(const std::string& type, int level);

#endif
