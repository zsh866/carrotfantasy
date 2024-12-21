#pragma once
#include "Boss01.hpp"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

class MapSelectUI; // 前向声明

USING_NS_CC;
using namespace cocostudio;
using namespace cocos2d::ui;

#define EVENT_RETURN_TO_LEVEL "event_return_to_level"
#define EVENT_ENEMY_REACHED_END  "enemy_reached_end"

class BaseGameScene : public cocos2d::Scene {
public:
    virtual bool init() override;
    virtual ~BaseGameScene();
    // 虚拟工厂方法
    virtual BaseGameScene* createNewScene() = 0;
protected:
    TMXTiledMap* map;
    SpriteBatchNode* spriteSheet;
    TMXObjectGroup* objects;
    Vector<Node*> pointsVector;

    // 控制按钮
    Button* _pauseButton;
    Button* _backButton;
    Button* _restartButton;
    Button* _optionsButton;

    // 遮罩层和按钮
    LayerColor* _maskLayer;
    Sprite* _resultSprite;
    Button* _maskRestartButton;
    Button* _maskBackButton;

    EventListenerCustom* _enemyReachedEndListener;

    bool _isPaused;
    std::string _currentMapPath;
    // 初始化方法
    virtual void initMap(const std::string& mapPath);
    void initPointsVector(float offX);
    void initButtons();

    // 按钮回调
    void onPauseButtonClicked(Ref* sender, Widget::TouchEventType type);
    void onBackButtonClicked(Ref* sender, Widget::TouchEventType type);
    void onRestartButtonClicked(Ref* sender, Widget::TouchEventType type);
    void onOptionsButtonClicked(Ref* sender, Widget::TouchEventType type);

    // 遮罩按钮回调
    void onMaskRestartClicked(Ref* sender, Widget::TouchEventType type);
    void onMaskBackClicked(Ref* sender, Widget::TouchEventType type);

    void showWinMask();
    void showLoseMask();

    // 添加事件处理方法
    virtual void onEnemyReachedEnd(EventCustom* event);
    // 初始化事件监听
    void initEventListeners();
};