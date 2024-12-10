#ifndef _TOWERLAYER_H_
#define _TOWERLAYER_H_

#include "cocos2d.h"
#include "Tower.h"
#include "Bullet.h"
#include "Monster.h"

USING_NS_CC;

#define hitRange 10

class EntityLayer : public Layer
{
public:
	std::vector<Tower*> towers;
	std::vector<Bullet*> bullets;
	std::vector<Monster*> monsters;

	Size visibleSize;
public:
	static Layer* createLayer();

	virtual bool init();

	virtual void update(float dt) override;

	CREATE_FUNC(EntityLayer);
};

#endif