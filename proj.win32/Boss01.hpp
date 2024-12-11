#pragma once
#include "MonsterBase.hpp"
class Monster : public MonsterBase {
public:
    CREATE_FUNC(Monster);
    bool init() override {
        if (!MonsterBase::init()) {
            return false;
        }

        // 加载精灵图集
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("/res/Item/Monsters01-hd.plist");

        // 使用第一帧初始化精灵
        initWithSpriteFrameName("boss_big01.png");

        // 初始化动画
        initAnimations();

        // 设置移动速度（根据需要调整）
        setRunSpeed(2.0f);

        // 添加方向变化的定时器
        schedule(CC_SCHEDULE_SELECTOR(MonsterBase::changeDirection), 0.1f);

        return true;
    }
    void playAnimation(const std::string& animName) {
        // 停止当前动画
        stopAllActions();

        // 播放新动画
        auto animation = AnimationCache::getInstance()->getAnimation(animName);
        if (animation) {
            runAction(RepeatForever::create(Animate::create(animation)));
        }
    }
private:
    void initAnimations() {
        // 根据plist中的命名创建动画
        // 假设boss_big01.png到boss_big08.png是一组动画
        auto bossAnim = createAnimation("boss_big", 8, 0.1f);
        AnimationCache::getInstance()->addAnimation(bossAnim, "boss_run");

        // 添加各个方向的动画
        auto rightAnim = createAnimation("boss_big", 8, 0.1f);
        AnimationCache::getInstance()->addAnimation(rightAnim, "runright");

        auto leftAnim = createAnimation("boss_big", 8, 0.1f);
        AnimationCache::getInstance()->addAnimation(leftAnim, "runleft");

        auto upAnim = createAnimation("boss_big", 8, 0.1f);
        AnimationCache::getInstance()->addAnimation(upAnim, "runup");

        auto downAnim = createAnimation("boss_big", 8, 0.1f);
        AnimationCache::getInstance()->addAnimation(downAnim, "rundown");
    }
};