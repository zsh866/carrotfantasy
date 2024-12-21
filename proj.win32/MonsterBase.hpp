#pragma once

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace cocostudio;
using namespace cocos2d::ui;

class MonsterBase : public cocos2d::Sprite {
public:
    virtual bool init() override;
    CREATE_FUNC(MonsterBase);

    Animation* createAnimation(std::string prefixName, int framesNum, float delay);
    void changeDirection(float dt);
    Node* currPoint();
    Node* nextPoint();
    void runFllowPoint();
    void setPointsVector(Vector<Node*> points);
    virtual void playAnimation(const std::string& animName);

private:
    Vector<Node*> pointsVector;

protected:
    int pointCounter;
    Animation* animationRight;
    Animation* animationLeft;
    CC_SYNTHESIZE(float, runSpeed, RunSpeed);

    bool checkReachedEnd(); // 添加检查是否到达终点的方法
    void dispatchReachedEndEvent(); // 添加发送事件的方法
};