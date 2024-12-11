#include "MonsterBase.hpp"

bool MonsterBase::init()
{
    if (!Sprite::init())
    {
        return false;
    }
    return true;
}

Animation* MonsterBase::createAnimation(std::string prefixName, int framesNum, float delay)
{
    Vector<SpriteFrame*> animFrames;
    for (int i = 1; i <= framesNum; i++) {
        char buffer[20] = { 0 };
        sprintf(buffer, "%02d.png", i);
        std::string str = prefixName + buffer;

        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        if (frame != nullptr) {
            animFrames.pushBack(frame);
        }
    }
    return Animation::createWithSpriteFrames(animFrames, delay);
}

void MonsterBase::changeDirection(float dt)
{
    const auto curr = currPoint();

    if (curr->getPositionX() > this->getPosition().x) {
        runAction(Animate::create(AnimationCache::getInstance()->getAnimation("runright")));
    } else {
        runAction(Animate::create(AnimationCache::getInstance()->getAnimation("runleft")));
    }

    if (curr->getPositionY() > this->getPosition().y) {
        runAction(Animate::create(AnimationCache::getInstance()->getAnimation("runup")));
    } else {
        runAction(Animate::create(AnimationCache::getInstance()->getAnimation("rundown")));
    }
}

Node* MonsterBase::currPoint()
{
    return this->pointsVector.at(pointCounter);
}

Node* MonsterBase::nextPoint()
{
    const int maxCount = this->pointsVector.size();
    pointCounter++;
    if (pointCounter < maxCount) {
        auto node = this->pointsVector.at(pointCounter);
        return node;
    } else {
        pointCounter = maxCount - 1;
    }
    return nullptr;
}

void MonsterBase::runFllowPoint()
{
    auto point = currPoint();
    setPosition(point->getPosition());
    point = nextPoint();
    if (point != NULL) {
        runAction(CCSequence::create(MoveTo::create(getRunSpeed(), point->getPosition()), CallFuncN::create(CC_CALLBACK_0(MonsterBase::runFllowPoint, this)), NULL));
    }
}

void MonsterBase::setPointsVector(Vector<Node*> points)
{
    // 清空现有路径点
    pointsVector.clear();

    // 重置路径点计数器
    pointCounter = 0;

    // 复制新的路径点
    for (auto point : points) {
        pointsVector.pushBack(point);
    }

    // 如果有路径点,开始移动
    if (!pointsVector.empty()) {
        runFllowPoint();
    }
}

void MonsterBase::playAnimation(const std::string& animName)
{
    // 停止当前动画
    stopAllActions();

    // 从动画缓存获取动画
    auto animation = AnimationCache::getInstance()->getAnimation(animName);
    if (animation) {
        // 创建动画动作并运行
        auto animate = Animate::create(animation);
        runAction(animate);
    }
}
