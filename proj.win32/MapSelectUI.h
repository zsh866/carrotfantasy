#pragma once

#include "SelectedMapInfo.hpp"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "LevelOneOne.h"
#include "LevelOneTwo.h"

class MapSelectUI : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene(const std::string& csbPath);
    virtual bool init(const std::string& csbPath);

    // �޸� CREATE_FUNC Ϊ�Զ��崴������
    static MapSelectUI* create(const std::string& csbPath)
    {
        MapSelectUI* pRet = new (std::nothrow) MapSelectUI();
        if (pRet && pRet->init(csbPath)) {
            pRet->autorelease();
            return pRet;
        } else {
            delete pRet;
            return nullptr;
        }
    }

private:
    void initializeUI(Node* rootNode);
    void setupMapDisplay();
    void onBackButtonClicked();
    void onStartButtonClicked();
    void onPageViewEvent(Ref* sender, PageView::EventType type);

    std::string _csbPath;
    cocos2d::ui::Button* _btnBack;
    cocos2d::ui::Button* _btnStart;
    cocos2d::ui::PageView* _mapPreview;
    int _mapIndex;
};
