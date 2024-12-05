#include "GameStart.hpp"
CocosDenshion::SimpleAudioEngine* audio = CocosDenshion::SimpleAudioEngine::getInstance();
cocos2d::Scene* GameStart::createScene()
{
    auto scene = Scene::create();
    auto layer = GameStart::create();
    scene->addChild(layer);

    return scene;
}
bool GameStart::init()
{
    if (!Scene::init())
        return false;

    auto rootNode = CSLoader::createNode("/res/MainScene.csb");
    if (rootNode == nullptr) {
        CCLOG("Failed to load WelcomeScene.csb");
        return false;
    }
    this->addChild(rootNode);

    Button* btnNormalModle = (Button*)rootNode->getChildByName("Btn_NormalModle");
    Button* btnBoss = (Button*)rootNode->getChildByName("Btn_Boss");
    Button* btnMonsterNest = (Button*)rootNode->getChildByName("Btn_MonsterNest");

    if (btnNormalModle == nullptr) {
        CCLOG("Failed to load Btn_NormalModle");
        return false;
    }
    btnNormalModle->addTouchEventListener(this, toucheventselector(GameStart::btnClick));
    btnBoss->addTouchEventListener(this, toucheventselector(GameStart::btnClick));
    btnMonsterNest->addTouchEventListener(this, toucheventselector(GameStart::btnClick));

    ImageView* cloud1 = (ImageView*)rootNode->getChildByName("Cloud1");
    ImageView* cloud2 = (ImageView*)rootNode->getChildByName("Cloud2");
    Vec2 size = Director::getInstance()->getVisibleSize();

    cloud2->runAction(RepeatForever::create(Sequence::create(MoveBy::create(10,
                                                                 Vec2(size.x + cloud2->getCustomSize().width, 0)),
        Place::create(Vec2(0, cloud2->getPositionY())), nullptr)));
    cloud1->runAction(RepeatForever::create(Sequence::create(MoveBy::create(20,
                                                                 Vec2(size.x + cloud1->getCustomSize().width, 0)),
        Place::create(Vec2(0, cloud1->getPositionY())), nullptr)));

    ImageView* bird = (ImageView*)rootNode->getChildByName("Bird");
    bird->runAction(RepeatForever::create(
        Sequence::create(
            MoveBy::create(1, Vec2(0, 50)),
            MoveBy::create(1, Vec2(0, -50)), nullptr)));

    ImageView* leaf2 = (ImageView*)rootNode->getChildByName("Leaf2");
    ImageView* leaf3 = (ImageView*)rootNode->getChildByName("Leaf3");
    RotateTo* rotateTo = RotateTo::create(0.1f, 20);
    RotateTo* rotateTo2 = RotateTo::create(0.1f, 0);
    leaf2->runAction(RepeatForever::create(
        Sequence::create(
            Repeat::create(Sequence::create(rotateTo, rotateTo2, nullptr), 2),
            DelayTime::create(5), nullptr)));
    leaf3->runAction(RepeatForever::create(
        Sequence::create(
            DelayTime::create(8),
            Repeat::create(Sequence::create(rotateTo, rotateTo2, nullptr), 2),
            DelayTime::create(4), nullptr)));
    return true;
}

void GameStart::btnClick(Button* btn, TouchEventType eventType)
{
    if (eventType == TOUCH_EVENT_ENDED) {
        btn->setVisible(true);
        // 如果是btnNormalModle被按下，切换scene
        if (strcmp(btn->getName().c_str(), "Btn_NormalModle") == 0) {
            auto scene = LevelSelect::createScene();
            Director::getInstance()->replaceScene(scene);
        }

    } else if (eventType == TOUCH_EVENT_BEGAN) {
        btn->setVisible(false);
    }
}