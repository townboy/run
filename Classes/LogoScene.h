#ifndef __LOGO_SCENE_H__
#define __LOGO_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

class LogoScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    CREATE_FUNC(LogoScene);
	void update(float e);
	void to_next();
private:
	Sprite* rock_sprite;
};

#endif // __HELLOWORLD_SCENE_H__
