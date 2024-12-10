#pragma once
#include "Config.h"
#include "GameStart.hpp"
#include "SelectedMapInfo.hpp"
#include "MapSelectUI.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GameDataManager.h"

USING_NS_CC;
using namespace cocostudio;
using namespace cocos2d::ui;
extern ChineseWord Words;
extern cocos2d::SpriteFrameCache* spritecache;

class LevelSelect :public cocos2d::Scene {
public:
	CREATE_FUNC(LevelSelect);
	static cocos2d::Scene* createScene();
    virtual bool init();

private:
    Button* _btnLeft;
    Button* _btnRight;
    Button* _btnHome;
    PageView* _pageView;
    void handlePageTouch(Layout* layout, Widget::TouchEventType eventType);
    void changePage(int index);
    void initializeUIControls(Node* rootNode);
    void setupPageView(Node* rootNode);
};