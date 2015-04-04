#ifndef __GREENSWARD_SCENE_H__
#define __GREENSWARD_SCENE_H__

#include "cocos2d.h"
#include "TeamSprite.h"
#include "Football.h"

USING_NS_CC;

class GreenSward : public cocos2d::Sprite
{
public:
	static GreenSward *create(const char *fileName);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    
    // implement the "static create()" method manually
    CREATE_FUNC(GreenSward);

	void update(float dt);
	void setViewPointCenter();
	void addObject();
	float calLog(float val);
private:
	Sprite * backGround;
	TeamSprite * teamA;
	TeamSprite * teamB;
	Football * football;
	Point lastPos;

};

#endif // __HELLOWORLD_SCENE_H__
