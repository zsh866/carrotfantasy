#include "Setting.h"

void Setting::initializeUIControls(Node* rootNode)
{
    // 初始化音效和音乐按钮
    struct ButtonPair {
        Button*& buttonOff;
        Button*& buttonOn;
        const char* nameOff;
        const char* nameOn;
    };

    const std::array<ButtonPair, 2> buttons = { { { _soundButtonOff, _soundButtonOn, "sfxOff", "sfxOn" },
        { _musicButtonOff, _musicButtonOn, "bgmOff", "bgmOn" } } };

    // 初始化按钮对
    for (const auto& pair : buttons) {
        pair.buttonOff = static_cast<Button*>(rootNode->getChildByName(pair.nameOff));
        pair.buttonOn = static_cast<Button*>(rootNode->getChildByName(pair.nameOn));
        if (!pair.buttonOff || !pair.buttonOn) {
            CCLOG("Failed to initialize buttons: %s, %s", pair.nameOff, pair.nameOn);
            return;
        }
    }

    // 设置观察者
    bgmEnabled.addObserver([this](bool enabled) {
                  _musicButtonOn->setVisible(enabled);
                  _musicButtonOff->setVisible(!enabled);
              })
        .addObserver([this](bool enabled) {
            auto& gameData = GameDataManager::getInstance();
            gameData.saveSoundSettings(
                enabled,
                gameData.getSfxEnabled(),
                gameData.getBgmVolume(),
                gameData.getSfxVolume());
        },
            false);
    sfxEnabled.addObserver([this](bool enabled) {
                  _soundButtonOn->setVisible(enabled);
                  _soundButtonOff->setVisible(!enabled);
              })
        .addObserver([this](bool enabled) {
            auto& gameData = GameDataManager::getInstance();
            gameData.saveSoundSettings(
                gameData.getBgmEnabled(),
                enabled,
                gameData.getBgmVolume(),
                gameData.getSfxVolume());
        },
            false);

    // 初始化状态
    auto& gameData = GameDataManager::getInstance();
    bgmEnabled.set(gameData.getBgmEnabled());
    sfxEnabled.set(gameData.getSfxEnabled());

    // 添加按钮点击事件
    constexpr std::array<const char*, 8> buttonNames = {
        "retHome", "sfxOff", "sfxOn", "bgmOff", "bgmOn",
        "resetBtn", "conBtn", "calBtn"
    };

    for (const auto& name : buttonNames) {
        if (auto button = static_cast<Button*>(rootNode->getChildByName(name))) {
            button->addTouchEventListener([this](Ref* sender, Widget::TouchEventType type) {
                btnClick(static_cast<Button*>(sender), type);
            });
        } else {
            CCLOG("Failed to find button: %s", name);
        }
    }
}

Scene* Setting::createScene()
{
    auto scene = Scene::create();
    assert(scene != nullptr);

    auto layer = Setting::create();
    scene->addChild(layer);
    return scene;
}

bool Setting::init()
{
    if (!Scene::init()) {
        return false;
    }
    auto rootNode = CSLoader::createNode("/res/SettingScene.csb");
    if (rootNode == nullptr) {
        CCLOG("Failed to load SettingScene.csb");
        return false;
    }
    this->addChild(rootNode);
    initializeUIControls(rootNode);
    return true;
}

void Setting::btnClick(Button* btn, Widget::TouchEventType eventType)
{
    if (!btn)
        return;
    CCLOG("btnClick: %s", btn->getName().c_str());

    switch (eventType) {
    case Widget::TouchEventType::BEGAN:
        break;
    case Widget::TouchEventType::ENDED:
        if (btn->getName() == "retHome") {
            auto scene = GameStart::createScene();
            Director::getInstance()->replaceScene(scene);
        } else if (btn->getName() == "sfxOn") {
            sfxEnabled.set(false);
            
        } else if (btn->getName() == "sfxOff") {
            sfxEnabled.set(true);
            audioManager.applySoundEffectSettings();
        } else if (btn->getName() == "bgmOn") {
            bgmEnabled.set(false);
            audioManager.applySoundEffectSettings();
            audioManager.applyBackgroundMusicSettings();
        } else if (btn->getName() == "bgmOff") {
            bgmEnabled.set(true);
            audioManager.applyBackgroundMusicSettings();
        } else if (btn->getName() == "resetBtn") {
            // 重置游戏
            // GameDataManager::getInstance().reset();
        } else if (btn->getName() == "conBtn") {
            // 确认
            // GameDataManager::getInstance().setBgmEnabled(bgmEnabled.get());
            // GameDataManager::getInstance().setSfxEnabled(sfxEnabled.get());
            // GameDataManager::getInstance().save();
            // auto scene = GameStart::createScene();
            // Director::getInstance()->replaceScene(scene);
        } else if (btn->getName() == "calBtn") {
            // 取消
            // auto scene = GameStart::createScene();
            // Director::getInstance()->replaceScene(scene);
        }

        break;
    case Widget::TouchEventType::CANCELED:
        break;
    default:
        break;
    }
}