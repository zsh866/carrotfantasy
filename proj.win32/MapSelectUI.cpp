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

    // ���ض�Ӧ�� CSB �ļ�
    auto rootNode = CSLoader::createNode(_csbPath);
    if (rootNode == nullptr) {
        CCLOG("Failed to load %s", _csbPath.c_str());
        return false;
    }
    this->addChild(rootNode);

    initializeUI(rootNode);
    setupMapDisplay();

    auto listener = EventListenerCustom::create(EVENT_RETURN_TO_LEVEL, [this](EventCustom* event) {
        auto LevelType = SelectedMapInfo::getInstance().getMapLevel();
        std::string csbPath = "";
        switch (LevelType) {
        case LevelType::SKY:
            csbPath = "res/SkylineScene.csb";
            break;
        default:
            break;
        }
        auto scene = MapSelectUI::createScene(csbPath);
        Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
    });
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);

    return true;
}


void MapSelectUI::initializeUI(Node* rootNode)
{
    _btnBack = static_cast<Button*>(rootNode->getChildByName("btnBack"));
    _btnStart = static_cast<Button*>(rootNode->getChildByName("btnStart"));
    _mapPreview = static_cast<PageView*>(rootNode->getChildByName("mapPreview"));

    if (_mapPreview) {
        _mapPreview->addEventListener(CC_CALLBACK_2(MapSelectUI::onPageViewEvent, this));

        // ���ó�ʼѡ��ҳ��
        int currentIndex = SelectedMapInfo::getInstance().getMapIndex();
        if (currentIndex >= 0 && currentIndex < _mapPreview->getItems().size()) {
            _mapPreview->scrollToItem(currentIndex);
        }
    }

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
    // ���ݵ�ǰѡ��Ĺؿ�����������Ӧ����ʾ����
    LevelType currentLevel = SelectedMapInfo::getInstance().getMapLevel();

    // ������Ը��ݲ�ͬ�ؿ��������ò�ͬ��UIԪ��
    // ���米��ͼƬ����ͼԤ����
}

void MapSelectUI::onBackButtonClicked()
{
    Director::getInstance()->replaceScene(LevelSelect::createScene());
}

void MapSelectUI::onStartButtonClicked()
{
    // ���ݲ�ͬ�ؿ����ʹ�����Ӧ����Ϸ����
    LevelType currentLevel = SelectedMapInfo::getInstance().getMapLevel();
    int currentIndex = SelectedMapInfo::getInstance().getMapIndex();
    Scene* gameScene = nullptr;

    switch (currentLevel) {
    case LevelType::SKY:
        switch (currentIndex) {
        case 0:
            Director::getInstance()->replaceScene(LevelOneOne::createScene());
            break;
        case 1:
            Director::getInstance()->replaceScene(LevelOneTwo::createScene());
            break;
        default:
            CCLOG("δ֪�Ĺؿ�����");
            return;
        }
        break;
    case LevelType::DESERT:
        CCLOG("gameScene = DesertGameScene::createScene()");
        break;
    case LevelType::FOREST:
        CCLOG("gameScene = ForestGameScene::createScene()");
        break;
    default:
        CCLOG("δ֪�Ĺؿ�����");
        return;
    }

    if (gameScene) {
        // Director::getInstance()->replaceScene(gameScene);
    }
}

void MapSelectUI::onPageViewEvent(Ref* sender, PageView::EventType type)
{
    if (type == PageView::EventType::TURNING) {
        int currentIndex = _mapPreview->getCurrentPageIndex();
        SelectedMapInfo::getInstance().setMapIndex(currentIndex);
        CCLOG("currentIndex = %d", currentIndex);
        // ������������µ�ͼԤ��ͼ
        // updateMapPreview(currentIndex);
    }
}