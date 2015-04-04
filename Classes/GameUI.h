#ifndef __GAMEUI_SCENE_H__
#define __GAMEUI_SCENE_H__

#include "cocos2d.h"
#include "TeamSprite.h"
USING_NS_CC;

class GAMEUI : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(GAMEUI);
	void getCurTime(char *);
	void update(float dt);
	int calTime();
	void drawMap();
	void initMap();
	void mistakeRemove();
	void callbackC();
	void callbackC2();
	void mistakeDisplay();
	void getGoal();
	int game_ui_update();
	void setPoint();
	void initPower();
	void speedUp(TeamSprite *);
	void resumePower();
	void getCold();
	int coldTime;
private:
	int resumeCount;
	float powerSize;
	int AspeedUpCount;
	int BspeedUpCount;
	Sprite *cold;
	Sprite *power;
	Sprite *restPower;
	Sprite *point;
	Sprite *mistake;
	bool mistakeAdd;
	Sprite *smallMap;
	Sprite *clock;
	Sprite *num[3];
	Sprite *playerPoint[8];
	int lastTime;
	char startTime[20];
	char curTime[20];
};

#endif // __HELLOWORLD_SCENE_H__
