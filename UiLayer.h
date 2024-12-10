#ifndef _UILAYER_H_
#define _UILAYER_H_

#include "cocos2d.h"

USING_NS_CC;

class UiLayer : public Layer
{
public:
	static Layer* createLayer();

	virtual bool init();

	CREATE_FUNC(UiLayer);
};

#endif
