#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "TeamSprite.h"
#include "GreenSward.h"
#include "Football.h"
#include "ControlLayer.h"
#include "Athlete.h"
#include "GameWorld.h"
#include "algorithm"
USING_NS_CC;

class GameScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void buttonACloseCallback(cocos2d::Ref* pSender);
    void buttonBCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

	void initJoyStick();
	void initButtonA();
	void initButtonB();
	void update(float dt);
	void initGreenSward();
	int calDirection(float degree);
	//void hitBall(Point target);
	void ApassBall();
	void BpassBall();
	void AshootBall();
	void hitBallA(Point targetPosition);
	void hitBallB(Point targetPosition);
	int switchDirection(int direction);
	void shootBall(TeamSprite *);
	void shootBallFail(int direction);
	void shootBallSuccess(int y ,int direction);
	void catchBall(TeamSprite * );

private:
	GreenSward * greenSward;
	int joyStickDirection;
};

#endif // __HELLOWORLD_SCENE_H__
