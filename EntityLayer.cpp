#include "EntityLayer.h"
#include "GameData.h"

Layer* EntityLayer::createLayer()
{
	return EntityLayer::create();
}

bool EntityLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();

	auto mouseListener = EventListenerMouse::create();

	mouseListener->onMouseDown = [this](EventMouse* event) {

		auto locationX = event->getCursorX();
		auto locationY = event->getCursorY();
		Vec2 location(locationX, locationY);//鼠标点击位置

		//添加鼠标交互逻辑

		/*
			如果点击的是塔
			弹出升级和出售的图标（创建两个按钮）
			获得money值，如果够，升级有色
			如果不够，升级灰色
			点击按钮执行操作
		*/
		Tower* tower;
		int i;
		for (i = 0; i < towers.size(); ++i)
		{
			tower = towers[i];
			Rect boundingBox = tower->getBoundingBox();
			if (boundingBox.containsPoint(location))
			{
				break;
			}
		}
		if (i < towers.size())//找到了点击的tower
		{
			//补充
		}
		


		/*
			如果点击的是空白
			弹出建造图标（按钮）
			获得money,如果不够，灰色
			如果够，有色
			点击建造，调用auto tower = Tower::buildTower(),并加入towers
			点击出售，调用tower->sellTower(),并移除towers
		*/


		

		/*
			如果点击的是障碍物
			如果点击的是怪物
			选中（遍历一遍towers，如果在攻击范围内，把其设为目标）
		*/

		};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, this);

	EventCustom event("moneyChange");//自定义事件 金钱改变量
	/*
	以下在合适地方加入
	//击败敌人时，分发事件
	{
		event.setUserData((void*)100);//改为击败的monster获得的金钱
		_eventDispatcher->dispatchEvent(&event);
	}
	*/

	scheduleUpdate();//启用帧更新

	return true;
}

void EntityLayer::update(float dt)//帧更新
{
	for (auto& monster : monsters)//处理monster移动
	{
		//monster->movePath();
	}

	for (auto& tower : towers)//处理tower的攻击
	{
		if (tower->getNumberOfRunningActions() == 0)//如果tower不在行动
		{
			if (!tower->target)//如果tower没有目标
			{
				for (auto& monster : monsters)
				{
					Vec2 towerPostion = tower->getPosition();
					Vec2 monsterPostion = monster->getPosition();

					if (towerPostion.distance(monsterPostion) <= tower->attackRange)//如果距离在攻击范围内
					{
						tower->target = monster;
						auto bullet = tower->aimAndShoot();
						bullets.push_back(bullet);
						this->addChild(bullet, 3);
					}
				}
			}
			else//如果tower有目标
			{
				Vec2 towerPostion = tower->getPosition();
				Vec2 targetPostion = tower->target->getPosition();

				if (towerPostion.distance(targetPostion) <= tower->attackRange)//如果目标在攻击范围内
				{
					auto bullet = tower->aimAndShoot();
					bullets.push_back(bullet);
					this->addChild(bullet, 3);
				}
				else //如果tower和target超出范围
				{
					tower->target = nullptr;
				}
			}
		}
	}

	for (auto& bullet : bullets)//处理bullet的移动
	{
		bullet->update(dt);

		Vec2 bulletPosition = bullet->getPosition();
		
		if (bulletPosition.x < 0 || bulletPosition.x > visibleSize.width || bulletPosition.y < 0 || bulletPosition.y > visibleSize.height)//子弹出界
		{
			bullet->removeFromParentAndCleanup(true);
			bullets.erase(std::find(bullets.begin(), bullets.end(), bullet));
			continue;
		}

		auto target = bullet->target;
		Vec2 targetPosition = target->getPosition();

		if (bulletPosition.distance(targetPosition) <= hitRange)//如果距离小于阈值，认为子弹命中
		{
			//加入子弹命中特效
			bullet->removeFromParentAndCleanup(true);
			bullets.erase(std::find(bullets.begin(), bullets.end(), bullet));
			//处理target,如果死了，要清理精灵vector
		}
	}
	
}