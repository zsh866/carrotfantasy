#include "MapSelectUI.h"
#include "LevelSelect.hpp"

USING_NS_CC;
using namespace cocos2d::ui;

Scene* MapSelectUI::createScene(const std::string& csbPath)
{
    auto scene = Scene::create();
    auto layer = MapSelectUI::create(csbPath);
    scene->addChild(layer);
    return scene;
}

bool MapSelectUI::init(const std::string& csbPath) {
    if (!Scene::init()) {
        return false;
    }

    _csbPath = csbPath;

    // 加载对应的 CSB 文件
    auto rootNode = CSLoader::createNode(_csbPath);
    if (rootNode == nullptr) {
        CCLOG("Failed to load %s", _csbPath.c_str());
        return false;
    }
    this->addChild(rootNode);

    initializeUI(rootNode);
    setupMapDisplay();

    return true;
}


void MapSelectUI::initializeUI(Node* rootNode)
{
    _btnBack = static_cast<Button*>(rootNode->getChildByName("btnBack"));
    _btnStart = static_cast<Button*>(rootNode->getChildByName("btnStart"));
    _mapPreview = static_cast<Sprite*>(rootNode->getChildByName("mapPreview"));

    _btnBack->addTouchEventListener([this](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            onBackButtonClicked();
        }
        });

    _btnStart->addTouchEventListener([this](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            onStartButtonClicked();
        }
        });
}

void MapSelectUI::setupMapDisplay()
{
    // 根据当前选择的关卡类型设置相应的显示内容
    LevelType currentLevel = SelectedMapInfo::getInstance().getMapLevel();

    // 这里可以根据不同关卡类型设置不同的UI元素
    // 比如背景图片、地图预览等
}

void MapSelectUI::onBackButtonClicked()
{
    Director::getInstance()->replaceScene(LevelSelect::createScene());
}

void MapSelectUI::onStartButtonClicked()
{
    // 根据不同关卡类型创建对应的游戏场景
    LevelType currentLevel = SelectedMapInfo::getInstance().getMapLevel();
    Scene* gameScene = nullptr;

    switch (currentLevel) {
    case LevelType::SKY:
        CCLOG("gameScene = SkyGameScene::createScene()");
        Director::getInstance()->replaceScene(LevelOne::createScene());
        break;
    case LevelType::DESERT:
        CCLOG("gameScene = DesertGameScene::createScene()");
        break;
    case LevelType::FOREST:
        CCLOG("gameScene = ForestGameScene::createScene()");
        break;
    default:
        CCLOG("未知的关卡类型");
        return;
    }

    if (gameScene) {
        // Director::getInstance()->replaceScene(gameScene);
    }
}