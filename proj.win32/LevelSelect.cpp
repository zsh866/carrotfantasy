#include "LevelSelect.hpp"
cocos2d::Scene* LevelSelect::createScene()
{
    auto scene = Scene::create();
    assert(scene != nullptr);

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
    // 初始化UI控件
    initializeUIControls(rootNode);

    // 设置页面视图
    setupPageView(rootNode);

    changePage(0);
    return true;
}

void LevelSelect::initializeUIControls(Node* rootNode)
{
    assert(rootNode != nullptr);

    _btnLeft = static_cast<Button*>(rootNode->getChildByName("btnLeft"));
    _btnRight = static_cast<Button*>(rootNode->getChildByName("btnRight"));
    _btnHome = static_cast<Button*>(rootNode->getChildByName("btnHome"));

    // 使用 lambda 统一处理按钮点击
    auto buttonCallback = [this](Ref* sender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED)
            return;

        auto btn = static_cast<Button*>(sender);
        if (!btn)
            return;

        const int currentPage = _pageView->getCurPageIndex();
        if (btn->getName() == "btnLeft") {
            changePage(currentPage - 1);
        } else if (btn->getName() == "btnRight") {
            changePage(currentPage + 1);
        } else if (btn->getName() == "btnHome") {
            Director::getInstance()->replaceScene(GameStart::createScene());
        }
    };

    _btnLeft->addTouchEventListener(buttonCallback);
    _btnRight->addTouchEventListener(buttonCallback);
    _btnHome->addTouchEventListener(buttonCallback);
}

void LevelSelect::setupPageView(Node* rootNode)
{
    assert(rootNode != nullptr);

    _pageView = static_cast<PageView*>(rootNode->getChildByName("PageView"));
    _pageView->addEventListener([this](Ref* sender, PageView::EventType type) {
        changePage(_pageView->getCurPageIndex());
    });

    // 设置页面触摸事件
    for (int i = _pageView->getPages().size() - 1; i >= 0; --i) {
        if (auto page = static_cast<Layout*>(_pageView->getPage(i))) {
            page->setUserData(reinterpret_cast<void*>(static_cast<std::intptr_t>(i)));
            page->addTouchEventListener([this](Ref* sender, Widget::TouchEventType type) {
                if (auto layout = static_cast<Layout*>(sender)) {
                    handlePageTouch(layout, type);
                }
            });
        }
    }
}

void LevelSelect::changePage(int index)
{
    const auto totalPages = _pageView->getPages().size();
    if (index < 0 || index >= totalPages) {
        return;
    }

    _btnLeft->setVisible(index > 0);
    _btnRight->setVisible(index < totalPages - 1);

    if (index != _pageView->getCurPageIndex()) {
        _pageView->scrollToPage(index);
    }
}

void LevelSelect::handlePageTouch(Layout* layout, Widget::TouchEventType eventType)
{
    assert(layout != nullptr);

    if (eventType != Widget::TouchEventType::ENDED) {
        return;
    }

    // 在这里处理页面点击逻辑
    CCLOG("页面被点击：%s", layout->getName().c_str());

    const auto level = LevelType((int)layout->getUserData());
    SelectedMapInfo::getInstance().setmapLevel(level);

    // 根据不同关卡加载对应的地图选择界面
    std::string csbPath;
    switch (level) {
    case LevelType::SKY:
        csbPath = "res/SkylineScene.csb";
        break;
    case LevelType::DESERT:
        csbPath = "res/DesertMapSelect.csb";
        break;
    case LevelType::FOREST:
        csbPath = "res/ForestMapSelect.csb";
        break;
    default:

        return;
    }

    Director::getInstance()->replaceScene(MapSelectUI::createScene(csbPath));
}