#pragma once
#include "Config.h"
#include "GameDataManager.h"
#include "GameStart.hpp"
#include "LevelSelect.hpp"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include"Property.hpp"
#include<array>
#include"AudioManager.hpp"

USING_NS_CC;
using namespace cocostudio;
using namespace cocos2d::ui;
extern cocos2d::SpriteFrameCache* spritecache;
class Setting : public cocos2d::Scene {
private:
    AudioManager& audioManager = AudioManager::getInstance();
    cocos2d::ui::Button* _homeButton;
    cocos2d::ui::Button* _soundButtonOff;
    cocos2d::ui::Button* _soundButtonOn;
    cocos2d::ui::Button* _musicButtonOff;
    cocos2d::ui::Button* _musicButtonOn;
    cocos2d::ui::Button* _resetButton;
    cocos2d::ui::Button* _confirmButton;
    cocos2d::ui::Button* _cancelButton;

    Property<bool> bgmEnabled{true};
    Property<bool> sfxEnabled{ true };
    void initializeUIControls(Node* rootNode);

public:
    void btnClick(Button* btn, Widget::TouchEventType eventType);
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(Setting);
};
