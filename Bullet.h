#ifndef _BULLET_H_
#define _BULLET_H_

#include "cocos2d.h"

class Tower;
class Monster;

USING_NS_CC;

class Bullet : public Sprite
{
public:
	std::string type;
	int attackNumber;
	float flySpeed;

	Sprite* target;
public:
	static Bullet* create(const std::string& filename, Tower* tower, Sprite* _target);

	virtual bool init(Tower* tower, Sprite* _target);

	void update(float dt);
};

#endif