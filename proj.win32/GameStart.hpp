#pragma once
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "Config.h"
#include "cocostudio/CocoStudio.h"
#include"LevelSelect.hpp"

USING_NS_CC;
using namespace cocostudio;
using namespace cocos2d::ui;

extern ChineseWord Words;
extern cocos2d::SpriteFrameCache* spritecache;
extern CocosDenshion::SimpleAudioEngine* audio;
class GameStart :public cocos2d::Scene
{
private:

	TransitionScene* transition;
	cocos2d::Scene* onlinechat;
	cocos2d::ui::Button* button_back;
	cocos2d::ui::Button* button_alonegame;
	cocos2d::ui::Button* button_onlinegame;

	cocos2d::Label* _label_adver_0;

	cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void initializeUIControls(Node* rootNode);
	void setupAnimations(Node* rootNode);
	void setupCloudAnimations(Node* rootNode);
	void setupBirdAnimation(Node* rootNode);
	void setupLeafAnimations(Node* rootNode);
	void btnClick(Button* btn, Widget::TouchEventType eventType);
	CREATE_FUNC(GameStart);
};