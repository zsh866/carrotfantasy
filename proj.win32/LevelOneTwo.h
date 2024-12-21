#pragma once
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos2d.h"
#include"Boss01.hpp"
#include "BaseGameScene.h"

USING_NS_CC;
using namespace cocostudio;
using namespace cocos2d::ui;

class LevelOneTwo : public BaseGameScene {
public:
	static Scene* createScene();
	virtual bool init() override;
	virtual BaseGameScene* createNewScene() override;
	CREATE_FUNC(LevelOneTwo);
};