#include "UiLayer.h"
#include "GameData.h"

Layer* UiLayer::createLayer()
{
	return UiLayer::create();
}

bool UiLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	int money = GameData::getInstance()->getMoney();
	char moneyString[10] = { 0 };
	sprintf(moneyString, "%d", money);
	auto moneyLabel = Label::createWithTTF(moneyString, "fonts/Marker Felt.ttf", 24);
	//moneyLabel->setPosition();放在适当位置

	this->addChild(moneyLabel);

	auto eventListener = EventListenerCustom::create("moneyChange", [=](EventCustom* event) {
		int money = (int)(event->getUserData());
		char moneyString[10] = { 0 };
		sprintf(moneyString, "%d", money);
		moneyLabel->initWithTTF(moneyString, "fonts/Marker Felt.ttf", 24);//更新money的显示
		});

	_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);


	//还要添加退出，暂停等等按钮


	return true;
}
