#pragma once
#include <vector>
#include "TeamSprite.h"
#include "football.h"
#include "cocos2d.h"
#include "Athlete.h"
USING_NS_CC;

class AI : public cocos2d::Sprite{
public:
	AI();
	void getBall(int *t_id, int *t_dir);
	float calDegree(Vec2 vec);
	int calDirection(float degree);
	void calOperation(int id, int *t_dir);
	void run(int *array_dir ,int getBallID);
	bool distTest(int id);
	bool specialTest(int id);
	int contronl(int id);
	int go(int id ,int direction);
	int come(int id ,int direction);
	int whereBall(float angle);
	void cut();
	bool ONOFF;
private:
	int counts;
	int dir[4];
	int rest[4];
	int howLong[4];
	int last[4];
	bool player[4];
	int lastTime;
};