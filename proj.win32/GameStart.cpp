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

    auto btnNormalModle = static_cast<Button*>(rootNode->getChildByName("Btn_NormalModle"));
    auto btnBoss = static_cast<Button*>(rootNode->getChildByName("Btn_Boss"));
    auto btnMonsterNest = static_cast<Button*>(rootNode->getChildByName("Btn_MonsterNest"));

    auto touchEventListener = [this](Ref* sender, Widget::TouchEventType type) {
        this->btnClick(static_cast<Button*>(sender), type);
    };
    btnNormalModle->addTouchEventListener(touchEventListener);
    btnBoss->addTouchEventListener(touchEventListener);
    btnMonsterNest->addTouchEventListener(touchEventListener);

    auto cloud1 = static_cast<ImageView*>(rootNode->getChildByName("Cloud1"));
    auto cloud2 = static_cast<ImageView*>(rootNode->getChildByName("Cloud2"));

    Vec2 size = Director::getInstance()->getVisibleSize();

    cloud2->runAction(RepeatForever::create(Sequence::create(
        MoveBy::create(15, Vec2(size.x + cloud2->getCustomSize().width, 0)),
        Place::create(Vec2(0, cloud2->getPositionY())), nullptr)));
    cloud1->runAction(RepeatForever::create(Sequence::create(
        MoveBy::create(25, Vec2(size.x + cloud1->getCustomSize().width, 0)),
        Place::create(Vec2(0, cloud1->getPositionY())), nullptr)));

    auto bird = static_cast<ImageView*>(rootNode->getChildByName("Bird"));
    bird->runAction(RepeatForever::create(
        Sequence::create(
            MoveBy::create(0.8, Vec2(0, 60)),
            MoveBy::create(0.8, Vec2(0, -60)), nullptr)));

    auto leaf2 = static_cast<ImageView*>(rootNode->getChildByName("Leaf2"));
    auto leaf3 = static_cast<ImageView*>(rootNode->getChildByName("Leaf3"));
    auto rotateTo = RotateTo::create(0.1f, 20);
    auto rotateTo2 = RotateTo::create(0.1f, 0);
    leaf2->runAction(RepeatForever::create(
        Sequence::create(
            Repeat::create(Sequence::create(rotateTo, rotateTo2, nullptr), 2),
            DelayTime::create(4), nullptr)));
    leaf3->runAction(RepeatForever::create(
        Sequence::create(
            DelayTime::create(6),
            Repeat::create(Sequence::create(rotateTo, rotateTo2, nullptr), 2),
            DelayTime::create(3), nullptr)));
    return true;
}

void GameStart::btnClick(Button* btn, Widget::TouchEventType eventType)
{
    switch (eventType) {
    case Widget::TouchEventType::BEGAN:
        btn->setVisible(false);
        break;

    case Widget::TouchEventType::ENDED:
        if (btn->getName() == "Btn_NormalModle") {
            auto scene = LevelSelect::createScene();
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