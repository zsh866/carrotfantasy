#include "obstacle.h"
Obstacle* Obstacle::create() {
	Obstacle* obstacle = new (std::nothrow)Obstacle;
	if (obstacle && obstacle->init()) {
		obstacle->autorelease();
		return obstacle;
	}
	CC_SAFE_DELETE(obstacle);
	return nullptr;
}
bool Obstacle::init() {
	if (!Sprite::init())
		return false;

	//暂时不支持多种障碍物
	auto obstacle = Sprite::create("obstacle.png");
	hp = fullHp = 50;
	rewardmoney = hp / 10;
	obstacleType = "obstacle";


	obstacle->setPosition(getPosition());
	this->addChild(obstacle);
	return true;
}
void Obstacle::getAttack(int damage) {
	hp -= damage;
	if (hp <= 0)
		dead();
	else
		this->setTexture(getObstacleName(obstacleType, fullHp - hp));

}
std::string Obstacle::getObstacleName(const std::string name, int type) {
	std::string filename = name;
	filename += type;
	filename += ".png";
	return filename;
}
void Obstacle::dead() {

	this->removeFromParentAndCleanup(true);
	//消失特效

	//掉落金币
}