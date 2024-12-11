#pragma once
#include "Carrot.h"
Carrot* Carrot::createCarrot(const cocos2d::Vec2& position) {
	Carrot* carrot = new (std::nothrow)Carrot;
	if (carrot && carrot->init(position)) {
		carrot->autorelease();
		return carrot;
	}
	CC_SAFE_DELETE(carrot);
	return nullptr;
}
bool Carrot::init(const cocos2d::Vec2& position) {
	if (!Sprite::init())
		return false;

	healthpoint = fullhealthpoint = 100;
	rewardmoney = 100;

	auto carrot = Sprite::create("Carrot_1.png");
	carrot->setPosition(position);
	this->getParent()->addChild(carrot);

	return true;
}
void Carrot::getAttack(int damage) {
	healthpoint -= damage;
	if (healthpoint <= 0) {
		healthpoint = 0;  //游戏结束
		return;
	}
	//奖励值减少

	updateAnim();
}
void Carrot::updateAnim() {
	this->setTexture("Carrot_" + std::to_string(fullhealthpoint - healthpoint) + ".png");
}
//添加萝卜的抖动效果
//void Carrot::shakeAnim() {
//	float shakeangle = 10.0f;
//	float duration = 0.1f;
//
//	auto leftshake = RotateBy::create(duration,shakeangle);
//	auto rightshake = RotateBy::create(duration*2, -shakeangle*2);
//	auto backshake = RotateBy::create(duration, shakeangle);
//
//	auto shakeaction = Sequence::create(leftshake, rightshake, backshake);
//
//	this->runAction(shakeaction);
//
//}

