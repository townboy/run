#ifndef __FOOTBALL_H__
#define __FOOTBALL_H__

#include "all.h"
#include "cocos2d.h"
#include "Athlete.h"
#include "TeamSprite.h"
USING_NS_CC;

class Football : public cocos2d::Sprite{
public:
	static float ballDist;
	Football();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void moveUpLeft();
	void moveUpRight();
	void moveDownLeft();
	void moveDownRight();
	void move(Vec2 vect);
	void jump();
	void update(float dt);
	void BeginAction(float x, float y, int action);
	void setBelong(Athlete *);
	void setLastBelong();
	bool withoutContronl();
	Athlete *getControner();
	Athlete *getLastControner();
	void afterBallOut();
	bool boundDetect();
	bool goalDetect();

private:
	int direction;
	Action *runingAction;
	Athlete* belong;
	Athlete* lastBelong;
	bool ballOut;
	TeamSprite *ballOutTeam;
};


#endif // __HELLOWORLD_SCENE_H__