#include "Tower.h"
#include "Bullet.h"
#include "Monster.h"
#include "GameData.h"

USING_NS_CC;

Tower* Tower::buildTower()
{
	auto tower = new Tower;
	if (tower && tower->initWithFile("Bottle1.png"))
	{
		tower->init();
		tower->autorelease();
		return tower;
	}
	CC_SAFE_DELETE(tower);
	return nullptr;
}

bool Tower::init()
{
	type = "Bottle";
	level = 1;
	buildMoney[0] = 100;
	buildMoney[1] = 180;
	buildMoney[2] = 260;
	sellMoney[0] = 80;
	sellMoney[1] = 224;
	sellMoney[2] = 432;
	attackNumber[0] = 100;
	attackNumber[1] = 120;
	attackNumber[2] = 140;
	attackRange = 300.0;
	attackSpeed = 1.0;
	bulletFlySpeed = 100;
	isChosed = false;
	return true;
}

void Tower::sellTower()
{
	EventCustom event("moneyChange");
	event.setUserData((void*)sellMoney[level - 1]);
	_eventDispatcher->dispatchEvent(&event);

	this->removeFromParentAndCleanup(true);
}

void Tower::levelUp()
{
	if (level < 3)
	{
		EventCustom event("moneyChange");
		event.setUserData((void*)buildMoney[level]);
		_eventDispatcher->dispatchEvent(&event);

		++level;
		this->setTexture(getTowerFilename(type, level));//升级，换贴图即可
	}
}

Bullet* Tower::aimAndShoot()
{
	Vec2 towerPosition = getPosition();
	Vec2 targetPosition = target->getPosition();
	Vec2 direction = targetPosition - towerPosition;
	float angle = CC_RADIANS_TO_DEGREES(direction.getAngle());//角度

	auto rotate = RotateTo::create(0.5, angle);
	this->runAction(rotate);

	//需要补充发射特效

	auto bullet = Bullet::create("Bullet1.png",this, target);
	bullet->setPosition(getPosition());
	return bullet;
}

std::string getTowerFilename(const std::string& type, int level)
{
	std::string filename = type;
	filename += char(level + '0');
	filename += ".png";
	return filename;
}