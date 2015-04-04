#ifndef __TEAMSPRITE_H__
#define __TEAMSPRITE_H__

#define KEY_DOWN(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 1 : 0)  
#define KEY_UP(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 0 : 1)  

#include <cocos2d.h>
#include <Athlete.h>
#include <vector>

USING_NS_CC;

class TeamSprite : public cocos2d::Sprite {
public:
	TeamSprite(int start ,int end);
	void contronlPlayer(int id ,int action);
	void contronlFromButton(int action);
	void contronlFromAI(int id ,int action);
	void contronlFromWeb(int id ,int action);
	void setAttackStatus();
	void setDenfenseStatus();
	bool attackIng();
	std::vector<float> degreeCal();
	Athlete *getControner();
	std::vector<Point> calPos();

	Vec2 getMainPosition();
	int getMainDirection();
	int getControlID();
	Vec2 getIDPosition(int id);
	int testMember(Athlete *);
	void setControl(int id);
	Athlete * Player[4];
	void setAttaciDirection(int );
	int getAttackDirection();
	void addGoal();
	int getScore();
	void reset(int start ,int end);
private:
	int control;
	int attackDirection;
	bool attackStatus;
	int goal;
};

#endif