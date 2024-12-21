#include "BaseGameScene.h"
bool BaseGameScene::init()
{
    if (!Scene::init()) {
        return false;
    }

    _isPaused = false;
    initButtons();
    initEventListeners();
    return true;
}
BaseGameScene::~BaseGameScene()
{
    if (_enemyReachedEndListener) {
        _eventDispatcher->removeEventListener(_enemyReachedEndListener);
        _enemyReachedEndListener = nullptr;
    }
}
void BaseGameScene::initMap(const std::string& mapPath)
{
    map = TMXTiledMap::create(mapPath);
    _currentMapPath = mapPath;
    CCLOG("get map");
    auto const winSize = Director::getInstance()->getWinSize();
    auto bgLayer = map->getLayer("bg");
    assert(bgLayer != nullptr, "bgLayer is nullptr");

    bgLayer->setAnchorPoint(Point(0.5f, 0.5f));
    bgLayer->setPosition(Point(winSize.width / 2, winSize.height / 2));
    objects = map->getObjectGroup("ava_path");
    assert(objects != nullptr, "objects is nullptr");
    initPointsVector(0);
    auto monster = Monster::create();
    monster->setPointsVector(pointsVector);
    this->addChild(monster);
    this->addChild(map, -1);
}

void BaseGameScene::initPointsVector(float offX)
{
    Node* runOfPoint = nullptr;
    int count = 0;
    ValueMap point;
    CCLOG("objects的长度: %d", objects->getObjects().size());
    point = objects->getObject(std::to_string(count));

    while (point.begin() != point.end()) {
        const float x = point.at("x").asFloat();
        const float y = point.at("y").asFloat();

        runOfPoint = Node::create();
        runOfPoint->setPosition(Point(x - offX, y));
        this->pointsVector.pushBack(runOfPoint);
        count++;
        point = objects->getObject(std::to_string(count));
    }
    runOfPoint = nullptr;
}

void BaseGameScene::initButtons()
{
    auto const visibleSize = Director::getInstance()->getVisibleSize();
    // 暂停按钮
    _pauseButton = Button::create("/res/UI/touming12.png", // 正常状态
        "/res/UI/touming25.png", // 按下状态
        "/res/UI/touming11.png"); // 禁用状态
    _pauseButton->setPosition(Vec2(visibleSize.width - 80, visibleSize.height - 50));
    _pauseButton->addTouchEventListener(CC_CALLBACK_2(BaseGameScene::onPauseButtonClicked, this));
    this->addChild(_pauseButton, 10);
    // 返回按钮
    _backButton = Button::create("/res/UI/return_normal.png",
        "/res/UI/return_selected.png",
        "/res/UI/return_normal.png");
    _backButton->setPosition(Vec2(150, visibleSize.height - 50));
    _backButton->addTouchEventListener(CC_CALLBACK_2(BaseGameScene::onBackButtonClicked, this));
    this->addChild(_backButton, 10);
    // 重新开始按钮
    _restartButton = Button::create("/res/UI/restart_normal.png",
        "/res/UI/restart_selected.png",
        "/res/UI/restart_normal.png");
    _restartButton->setPosition(Vec2(400, visibleSize.height - 50));
    _restartButton->addTouchEventListener(CC_CALLBACK_2(BaseGameScene::onRestartButtonClicked, this));
    this->addChild(_restartButton, 10);
    // 选项按钮
    /*_optionsButton = Button::create("options_bg.png",
        "options_bg.png",
        "options_bg.png");
    _optionsButton->setPosition(Vec2(visibleSize.width - 250, visibleSize.height - 50));
    _optionsButton->addTouchEventListener(CC_CALLBACK_2(BaseGameScene::onOptionsButtonClicked, this));
    this->addChild(_optionsButton, 10);*/

    /*_pauseButton = Button::create("/res/UI/pause_normal.png",
        "/res/UI/pause_pressed.png",
        "/res/UI/pause_disabled.png");
    _pauseButton->setPosition(Vec2(visibleSize.width - 50, visibleSize.height - 50));
    _pauseButton->addTouchEventListener(CC_CALLBACK_2(BaseGameScene::onPauseButtonClicked, this));
    this->addChild(_pauseButton, 10);
    _backButton = Button::create("/res/UI/back_normal.png",
        "/res/UI/back_pressed.png",
        "/res/UI/back_disabled.png");
    _backButton->setPosition(Vec2(50, visibleSize.height - 50));
    _backButton->addTouchEventListener(CC_CALLBACK_2(BaseGameScene::onBackButtonClicked, this));
    this->addChild(_backButton, 10);*/
}

void BaseGameScene::onPauseButtonClicked(Ref* sender, Widget::TouchEventType type)
{

    if (type != Widget::TouchEventType::ENDED)
        return;

    _isPaused = !_isPaused;
    if (_isPaused) {
        Director::getInstance()->pause();
        // 将正常图片与按下图片互换
        _pauseButton->loadTextures("/res/UI/touming25.png", "/res/UI/touming12.png", "/res/UI/touming11.png");
    } else {
        Director::getInstance()->resume();
        // 将正常图片与按下图片互换
        _pauseButton->loadTextures("/res/UI/touming12.png", "/res/UI/touming25.png", "/res/UI/touming11.png");
    }
}

void BaseGameScene::onBackButtonClicked(Ref* sender, Widget::TouchEventType type)
{

    if (type != Widget::TouchEventType::ENDED)
        return;

    // 发送返回地图选择的事件
    EventCustom event(EVENT_RETURN_TO_LEVEL);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    // auto scene = MapSelectUI::createScene("res/SkylineScene.csb");
    // Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
}

void BaseGameScene::onRestartButtonClicked(Ref* sender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED)
        return;
    // 重新加载当前场景

    // 如果游戏处于暂停状态，先恢复游戏
    if (_isPaused) {
        Director::getInstance()->resume();
        _isPaused = false;
    }

    // 使用虚拟工厂方法创建具体的场景实例
    auto newScene = createNewScene();
    if (newScene) {
        newScene->initMap(_currentMapPath);
        Director::getInstance()->replaceScene(
            TransitionFade::create(0.5f, newScene));
    }
    else {
        CCLOG("Failed to create new scene for restart");
    }
}

void BaseGameScene::onOptionsButtonClicked(Ref* sender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED)
        return;
    // TODO: 显示选项菜单
    // 可以创建一个新的弹出层来显示游戏选项
}

void BaseGameScene::showWinMask()
{
    // 创建半透明黑色遮罩
    _maskLayer = LayerColor::create(Color4B(0, 0, 0, 180));
    this->addChild(_maskLayer, 100);

    // 显示胜利图片
    _resultSprite = Sprite::create("/res/UI/win.png");
    _resultSprite->setPosition(Director::getInstance()->getVisibleSize() / 2);
    _maskLayer->addChild(_resultSprite);

    // 添加按钮
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 重新开始按钮
    _maskRestartButton = Button::create(
        "/res/UI/restart_normal.png",
        "/res/UI/restart_selected.png",
        "/res/UI/restart_normal.png");
    _maskRestartButton->setPosition(Vec2(visibleSize.width / 2 - 150, visibleSize.height / 2 - 100));
    _maskRestartButton->addTouchEventListener(CC_CALLBACK_2(BaseGameScene::onMaskRestartClicked, this));
    _maskLayer->addChild(_maskRestartButton);

    // 返回按钮
    _maskBackButton = Button::create(
        "/res/UI/return_normal.png",
        "/res/UI/return_selected.png",
        "/res/UI/return_normal.png");
    _maskBackButton->setPosition(Vec2(visibleSize.width / 2 + 150, visibleSize.height / 2 - 100));
    _maskBackButton->addTouchEventListener(CC_CALLBACK_2(BaseGameScene::onMaskBackClicked, this));
    _maskLayer->addChild(_maskBackButton);
}

void BaseGameScene::showLoseMask()
{
    // 创建半透明黑色遮罩
    _maskLayer = LayerColor::create(Color4B(0, 0, 0, 180));
    this->addChild(_maskLayer, 100);

    // 显示失败图片
    _resultSprite = Sprite::create("/res/UI/lose.png");
    _resultSprite->setPosition(Director::getInstance()->getVisibleSize() / 2);
    _maskLayer->addChild(_resultSprite);

    // 添加按钮 (与胜利界面使用相同的布局)
    auto visibleSize = Director::getInstance()->getVisibleSize();

    _maskRestartButton = Button::create(
        "/res/UI/restart_normal.png",
        "/res/UI/restart_selected.png",
        "/res/UI/restart_normal.png");
    _maskRestartButton->setPosition(Vec2(visibleSize.width / 2 - 150, visibleSize.height / 2 - 100));
    _maskRestartButton->addTouchEventListener(CC_CALLBACK_2(BaseGameScene::onMaskRestartClicked, this));
    _maskLayer->addChild(_maskRestartButton);

    _maskBackButton = Button::create(
        "/res/UI/return_normal.png",
        "/res/UI/return_selected.png",
        "/res/UI/return_normal.png");
    _maskBackButton->setPosition(Vec2(visibleSize.width / 2 + 150, visibleSize.height / 2 - 100));
    _maskBackButton->addTouchEventListener(CC_CALLBACK_2(BaseGameScene::onMaskBackClicked, this));
    _maskLayer->addChild(_maskBackButton);
}

void BaseGameScene::initEventListeners()
{
    if (_enemyReachedEndListener) {
        _eventDispatcher->removeEventListener(_enemyReachedEndListener);
        _enemyReachedEndListener = nullptr;
    }

    _enemyReachedEndListener = EventListenerCustom::create(
        EVENT_ENEMY_REACHED_END,
        CC_CALLBACK_1(BaseGameScene::onEnemyReachedEnd, this));
    _eventDispatcher->addEventListenerWithFixedPriority(_enemyReachedEndListener, 1);
}

void BaseGameScene::onMaskRestartClicked(Ref* sender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED)
        return;

    // 移除遮罩层
    if (_maskLayer) {
        _maskLayer->removeFromParent();
        _maskLayer = nullptr;
    }

    // 如果游戏处于暂停状态，恢复游戏
    if (_isPaused) {
        Director::getInstance()->resume();
        _isPaused = false;
    }

    // 使用之前实现的重新开始逻辑
    auto newScene = createNewScene();
    if (newScene) {
        newScene->retain();
        newScene->initMap(_currentMapPath);
        newScene->autorelease();

        Director::getInstance()->replaceScene(
            TransitionFade::create(0.5f, newScene));
    }
}
void BaseGameScene::onMaskBackClicked(Ref* sender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED)
        return;

    // 移除遮罩层
    if (_maskLayer) {
        _maskLayer->removeFromParent();
        _maskLayer = nullptr;
    }

    // 如果游戏处于暂停状态，恢复游戏
    if (_isPaused) {
        Director::getInstance()->resume();
        _isPaused = false;
    }
    // 发送返回地图选择的事件
    EventCustom event(EVENT_RETURN_TO_LEVEL);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void BaseGameScene::onEnemyReachedEnd(EventCustom* event)
{
    // 显示失败遮罩
    showLoseMask();

    // 暂停游戏
    if (!_isPaused) {
        Director::getInstance()->pause();
        _isPaused = true;
    }
}