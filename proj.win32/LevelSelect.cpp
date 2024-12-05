#include "LevelSelect.hpp"
cocos2d::Scene* LevelSelect::createScene()
{
    auto scene = Scene::create();
    auto layer = LevelSelect::create();
    scene->addChild(layer);


    return scene;
}

bool LevelSelect::init()
{
    if (!Scene::init())
        return false;

    auto rootNode = CSLoader::createNode("/res/LevelScene.csb");
    if (rootNode == nullptr)
    {
        CCLOG("Failed to load WelcomeScene.csb");
        return false;
    }
    this->addChild(rootNode);
    return true;
}