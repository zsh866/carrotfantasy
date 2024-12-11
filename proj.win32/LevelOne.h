#pragma once
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos2d.h"
#include"Boss01.hpp"

USING_NS_CC;
using namespace cocostudio;
using namespace cocos2d::ui;

class LevelOne : public cocos2d::Scene {
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(LevelOne);
private:
	TMXTiledMap* map;
	SpriteBatchNode* spriteSheet;
	TMXObjectGroup* objects;
	Vector<Node*> pointsVector;

	void initPointsVector(float offX);
	// void addEnemy();
};