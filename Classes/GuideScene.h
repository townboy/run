#ifndef __GUIDESCENE_H__
#define __GUIDESCENE_H__

#include "cocos2d.h"
#include "string"

class GuideScene : public cocos2d::Layer {
public:
	virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone

    // implement the "static create()" method manually
    CREATE_FUNC(GuideScene);
	int now;
	int endPage;
	cocos2d::LabelTTF * ttf;
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* e);
	void changeNow();
};

#endif