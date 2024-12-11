#pragma once

#include "monster.h"
#include "Carrot.h"
USING_NS_CC;

Monster* Monster::createMonster(int type) {
    Monster* monster = new (std::nothrow)Monster;
    
    if (monster && monster->init(type)) {
        monster->autorelease();
        return monster;
    }
    CC_SAFE_DELETE(monster);
    return nullptr;
}

bool Monster::init(int type) {
    if (!Sprite::init()) {
        return false;
    }

    //暂时支持一种类型
    hitpoint = 100;
    speed = 10;

    //创建动画
    auto monster = Sprite::create("monster.png");
    this->addChild(monster);

    //需要添加选中的功能，和obstacle处理思路相似，暂未添加
    
    //血量条初始化
    _hp = LoadingBar::create("health_bar_background.png");  
    _hp->setDirection(LoadingBar::Direction::LEFT);  
    _hp->setPercent(100);  

    _hp->setAnchorPoint(Vec2(0.5f, 1.0f));  
    _hp->setPosition(monster->getPosition() + Vec2(0, 30));  
    _hp->setVisible(false);          
    this->addChild(_hp);  

    return true;
}

void Monster::gethit(int damage) {
    hitpoint -= damage;

    if (hitpoint <= 0)
        isdead();
    else {
        updateHealthBar();
        //showexplosionScene();
    }

}

void Monster::movepath(const std::vector<Vec2>& path) {
    if (path.empty()) return;  // 检查路径是否为空

    this->stopAllActions();                           
    Vector<FiniteTimeAction*> actions;                
    Vec2 currentposition = this->getPosition();       

    for (const Vec2&position:path) {
        auto distance=position-currentposition;
        currentposition=position;

        float duration=distance.length()/speed; 

        auto moveAction=MoveTo::create(duration,position);
        actions.pushBack(moveAction);
    }

    //执行移动动作序列
    this->runAction(Sequence::create(actions));

    //到达终点之后
    auto reachedEndCallback = CallFunc::create([this]() {
        if (this->hitpoint > 0) {
            //减少萝卜血量

        }
        this->removeFromParent();
        });
    actions.pushBack(reachedEndCallback);

    // 执行动作序列
    this->runAction(Sequence::create(actions));

}
void Monster::updateHealthBar() {
    //根据现在的血量更新显示，持续1-2s，随怪物移动

    _hp->setVisible(true);
    _hp->setPercent(hitpoint);

    this->runAction(Sequence::create(
        DelayTime::create(2.0f), 
        FadeOut::create(0.5f),   
        CallFunc::create([this]() {
            _hp->setVisible(false); 
            }),
        nullptr
    ));
}

void Monster::isdead() {
    //播放死亡动画
    auto deadAnim=Animation::create();
    for (int i=1;i<=3;++i) 
        deadAnim->addSpriteFrameWithFile("dead_"+std::to_string(i)+ ".png");
    deadAnim->setDelayPerUnit(0.1f);
    auto* deadaction = Animate::create(deadAnim);
    auto* func = CallFunc::create([this]() { this->removeFromParentAndCleanup(true);});
    this->runAction(Sequence::create(deadaction,func,nullptr));

}



