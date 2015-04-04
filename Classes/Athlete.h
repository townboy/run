#ifndef __ATHLETE_H__
#define __ATHLETE_H__

#include "cocos2d.h"
#define hello_RIGHT 0
#define hello_UPRIGHT 1
#define hello_UP 2
#define hello_UPLEFT 3
#define hello_LEFT 4
#define hello_DOWNLEFT 5
#define hello_DOWN 6
#define hello_DOWNRIGHT 7
#define STATIC 8
#define SCALE (1.0f)
#define BASESPEED 10.0f
USING_NS_CC;

class Athlete : public cocos2d::Sprite{
private:
	float speed;
	float power;
	float shoot;
	float skillSpeed;
	float speedUP;
	int id;

	Action * right;
	Action * up;
	Action * down;
	Action * running;
	Sprite * id_sprite;
public:
	int direction;
	Athlete(int id);
	bool score();
	int getID();
	void update(float e);
	
	bool move(Vec2 vect);
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
	void moveUpLeft();
	void moveUpRight();
	void moveDownLeft();
	void moveDownRight();
	void action_shoot();
	void action_pass();
	void action_block();
	void action_putDown();
	void action_getBall();
	bool action_detectBall();
	void setSpeed(float );
	void setPower(float );
	void setShoot(float );
	int getShoot();
	void toInit();
	void addSpeed();
};

#endif // __HELLOWORLD_SCENE_H__
