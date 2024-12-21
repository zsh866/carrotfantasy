#include "LevelOneOne.h"

Scene* LevelOneOne::createScene()
{
    auto scene = Scene::create();
    assert(scene != nullptr);

    auto layer = LevelOneOne::create();
    scene->addChild(layer);
    return scene;
}

bool LevelOneOne::init()
{
    if (!BaseGameScene::init()) {
        return false;
    }

    initMap("/Scene/level1/level1.tmx");
    return true;
}

BaseGameScene* LevelOneOne::createNewScene()
{
    auto newScene = LevelOneOne::create();
    if (newScene) {
        // 如果需要其他初始化，可以在这里添加
        return newScene;
    }
    return nullptr;
}
