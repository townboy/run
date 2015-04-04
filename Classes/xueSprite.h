#ifndef __XUESPRITE_H__
#define __XUESPRITE_H__

#include "cocos2d.h"

class xueSprite : public cocos2d::Sprite {
public:
	void updateXue(int abi);
	virtual bool init();
    CREATE_FUNC(xueSprite);
	void fuzhi(int people, int skill, int abi);
	cocos2d::Sprite * xue;
	cocos2d::LabelTTF * ttf;
	cocos2d::MenuItemLabel * reduce;
	cocos2d::MenuItemLabel * map;

	int abi;
private:
	int people;
	int skill;
};

#endif