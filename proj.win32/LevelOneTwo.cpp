#include "LevelOneTwo.h"

Scene* LevelOneTwo::createScene()
{
    auto scene = Scene::create();
    assert(scene != nullptr);

    auto layer = LevelOneTwo::create();
    scene->addChild(layer);
    return scene;
}

bool LevelOneTwo::init()
{
    if (!BaseGameScene::init()) {
        return false;
    }

    initMap("/Scene/level2/level2.tmx");
    return true;
}

BaseGameScene* LevelOneTwo::createNewScene()
{
    auto newScene = LevelOneTwo::create();
    if (newScene) {
        // 如果需要其他初始化，可以在这里添加
        return newScene;
    }
    return nullptr;
}
