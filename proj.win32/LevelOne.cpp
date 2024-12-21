#include "LevelOne.h"

Scene* LevelOne::createScene()
{
    auto scene = Scene::create();
    assert(scene != nullptr);

    auto layer = LevelOne::create();
    scene->addChild(layer);
    return scene;
}

bool LevelOne::init()
{
    map = TMXTiledMap::create("/scene/level1/level1.tmx");
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
    return true;
}

void LevelOne::initPointsVector(float offX)
{
    Node* runOfPoint = nullptr;
    // 1
    int count = 0;
    ValueMap point;

    // 输出objects的长度
    CCLOG("objects的长度: %d", objects->getObjects().size());
    point = objects->getObject(std::to_string(count));
    // 2
    while (point.begin() != point.end()) {
        // 3
        const float x = point.at("x").asFloat();
        const float y = point.at("y").asFloat();
        // 4
        runOfPoint = Node::create();
        runOfPoint->setPosition(Point(x - offX, y));
        this->pointsVector.pushBack(runOfPoint);
        count++;
        point = objects->getObject(std::to_string(count));
    }
    runOfPoint = nullptr;
}
