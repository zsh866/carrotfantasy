#include "GameStart.hpp"

cocos2d::Scene* GameStart::createScene()
{
    auto scene = Scene::create();
    auto layer = GameStart::create();
    scene->addChild(layer);
    return scene;
}

bool GameStart::init()
{
    if (!Scene::init()) {
        return false;
    }

    auto rootNode = CSLoader::createNode("/res/MainScene.csb");
    if (!rootNode) {
        CCLOG("Failed to load MainScene.csb");
        return false;
    }
    addChild(rootNode);

    // 初始化UI控件
    initializeUIControls(rootNode);

    // 设置动画效果
    setupAnimations(rootNode);

    // 设置音频音效
    auto& audioManager = AudioManager::getInstance();
    audioManager.initialize("/res/music/bgm.mp3");
    audioManager.initializeWithConfig();

    return true;
}

void GameStart::initializeUIControls(Node* rootNode)
{
    assert(rootNode != nullptr);
    // 初始化按钮
    const std::vector<std::string> buttonNames = {
        "Btn_NormalModle",
        "Btn_Boss",
        "Btn_MonsterNest",
        "Btn_Setting"
    };

    for (const auto& name : buttonNames) {
        auto button = static_cast<Button*>(rootNode->getChildByName(name));
        if (button) {
            button->addTouchEventListener([this](Ref* sender, Widget::TouchEventType type) {
                btnClick(static_cast<Button*>(sender), type);
            });
        }
    }
}

void GameStart::setupAnimations(Node* rootNode)
{
    setupCloudAnimations(rootNode);
    setupBirdAnimation(rootNode);
    setupLeafAnimations(rootNode);
}

void GameStart::setupCloudAnimations(Node* rootNode)
{
    auto cloud1 = static_cast<ImageView*>(rootNode->getChildByName("Cloud1"));
    auto cloud2 = static_cast<ImageView*>(rootNode->getChildByName("Cloud2"));

    const Vec2 size = Director::getInstance()->getVisibleSize();

    if (cloud2) {
        cloud2->runAction(RepeatForever::create(Sequence::create(
            MoveBy::create(15, Vec2(size.x + cloud2->getCustomSize().width, 0)),
            Place::create(Vec2(0, cloud2->getPositionY())),
            nullptr)));
    }

    if (cloud1) {
        cloud1->runAction(RepeatForever::create(Sequence::create(
            MoveBy::create(25, Vec2(size.x + cloud1->getCustomSize().width, 0)),
            Place::create(Vec2(0, cloud1->getPositionY())),
            nullptr)));
    }
}

void GameStart::setupBirdAnimation(Node* rootNode)
{
    if (auto bird = static_cast<ImageView*>(rootNode->getChildByName("Bird"))) {
        // 获取屏幕尺寸
        const Vec2 screenSize = Director::getInstance()->getVisibleSize();

        // 创建一个复合动作序列
        auto flyUpAction = Spawn::create(
            MoveBy::create(1.2f, Vec2(100, 80)), // 向右上方飞行
            ScaleTo::create(1.2f, 1.1f), // 轻微放大
            RotateTo::create(1.2f, 15.0f), // 向右倾斜
            nullptr);

        auto flyDownAction = Spawn::create(
            MoveBy::create(1.2f, Vec2(-100, -80)), // 向左下方飞行
            ScaleTo::create(1.2f, 0.9f), // 轻微缩小
            RotateTo::create(1.2f, -15.0f), // 向左倾斜
            nullptr);

        // 添加缓动效果，使动作更自然
        auto easeUpAction = EaseInOut::create(flyUpAction->clone(), 2.0f);
        auto easeDownAction = EaseInOut::create(flyDownAction->clone(), 2.0f);

        // 组合所有动作
        bird->runAction(RepeatForever::create(
            Sequence::create(
                easeUpAction,
                DelayTime::create(0.2f), // 短暂停顿
                easeDownAction,
                DelayTime::create(0.2f), // 短暂停顿
                nullptr)));
    }
}

void GameStart::setupLeafAnimations(Node* rootNode)
{
    auto leaf2 = static_cast<ImageView*>(rootNode->getChildByName("Leaf2"));
    auto leaf3 = static_cast<ImageView*>(rootNode->getChildByName("Leaf3"));

    // 创建叶子摆动的基础动作
    auto createLeafWaveAction = [](float duration, float rotateAngle, float moveDistance) {
        // 组合旋转和轻微位移
        auto waveUp = Spawn::create(
            EaseInOut::create(RotateTo::create(duration, rotateAngle), 2.0f),
            EaseInOut::create(MoveBy::create(duration, Vec2(moveDistance, moveDistance / 2)), 2.0f),
            ScaleTo::create(duration, 1.05f),
            nullptr);

        auto waveDown = Spawn::create(
            EaseInOut::create(RotateTo::create(duration, -rotateAngle / 2), 2.0f),
            EaseInOut::create(MoveBy::create(duration, Vec2(-moveDistance, -moveDistance / 2)), 2.0f),
            ScaleTo::create(duration, 0.95f),
            nullptr);

        return Sequence::create(waveUp, waveDown, nullptr);
    };

    if (leaf2) {
        // 叶子2的动画：较快的摆动
        auto waveAction = createLeafWaveAction(0.8f, 15.0f, 10.0f);
        auto sequence = Sequence::create(
            Repeat::create(waveAction, 3),
            DelayTime::create(2.0f),
            nullptr);
        leaf2->runAction(RepeatForever::create(sequence));

        // 添加永久性的轻微飘动
        auto floatAction = Sequence::create(
            MoveBy::create(1.5f, Vec2(0, 3)),
            MoveBy::create(1.5f, Vec2(0, -3)),
            nullptr);
        leaf2->runAction(RepeatForever::create(floatAction));
    }

    if (leaf3) {
        // 叶子3的动画：较慢的摆动，与叶子2形成错落感
        auto waveAction = createLeafWaveAction(1.2f, 20.0f, 15.0f);
        auto sequence = Sequence::create(
            DelayTime::create(1.0f), // 错开时间
            Repeat::create(waveAction, 2),
            DelayTime::create(3.0f),
            nullptr);
        leaf3->runAction(RepeatForever::create(sequence));

        // 添加永久性的轻微飘动，方向与叶子2相反
        auto floatAction = Sequence::create(
            MoveBy::create(2.0f, Vec2(0, -4)),
            MoveBy::create(2.0f, Vec2(0, 4)),
            nullptr);
        leaf3->runAction(RepeatForever::create(floatAction));
    }
}

void GameStart::btnClick(Button* btn, Widget::TouchEventType eventType)
{
    if (!btn)
        return;
    CCLOG("btnClick: %s", btn->getName().c_str());
    switch (eventType) {
    case Widget::TouchEventType::BEGAN:
        btn->setVisible(false);
        break;

    case Widget::TouchEventType::ENDED:
        if (btn->getName() == "Btn_NormalModle") {
            auto scene = LevelSelect::createScene();
            Director::getInstance()->replaceScene(scene);
        } else if (btn->getName() == "Btn_Setting") {
            auto scene = Setting::createScene();
            Director::getInstance()->replaceScene(scene);
        }
        break;

    case Widget::TouchEventType::CANCELED:
        btn->setVisible(true);
        break;

    default:
        break;
    }
}