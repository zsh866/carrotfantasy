#pragma once
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "Config.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace cocostudio;
using namespace cocos2d::ui;
extern ChineseWord Words;
extern cocos2d::SpriteFrameCache* spritecache;
extern CocosDenshion::SimpleAudioEngine* audio;

class LevelSelect :public cocos2d::Scene {
public:
	CREATE_FUNC(LevelSelect);
	static cocos2d::Scene* createScene();
	virtual bool init();
private:

};