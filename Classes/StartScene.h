#ifndef __START_SCENE_H__
#define __START_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

class StartScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    CREATE_FUNC(StartScene);
	bool onTouchBegan(cocos2d::Touch*touch, cocos2d::Event*event);
	void addLogin();
	cocos2d::TextFieldTTF * id;
	void dengluCallback(cocos2d::Ref* pSender);
	
private:
	Sprite* backGround;
	Sprite* frog;
};

#endif // __HELLOWORLD_SCENE_H__
