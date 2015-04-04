#include "GreenSward.h"
#include "Athlete.h"
#include "TeamSprite.h"
#include "GameWorld.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "client_sdk/network_tool_win.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "client_sdk/network_tool.h"
#endif

#include "client_sdk\data_def.h"

GreenSward * GreenSward::create(const char *fileName)
{
	float x = 455;
	float y = 2405;
	GreenSward * sprite = new GreenSward();
	if(sprite && sprite->init())
	//if(sprite && sprite->initWithFile(fileName))
	{
		for(int i = 0 ;i < 15;i ++) {
			char aimfile[100];
			sprintf(aimfile, "images/greenSward_%02d.png", i + 1);
			auto temp = cocos2d::Sprite::create(aimfile);
			int heng = i % 5;
			int lie = i / 5;
			float aimX = x + 915 * heng;
			float aimY = y - 945 * lie;
			temp->setPosition(aimX, aimY);
			sprite->addChild(temp);
		}
		SimpleAudioEngine::getInstance()->playBackgroundMusic("music/OPILKO_MR.mp3" ,true);
		SimpleAudioEngine::getInstance()->playEffect("music/SpectatorExcite.mp3" ,true);
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void GreenSward::update(float e)
{
	setViewPointCenter();
}

void GreenSward::setViewPointCenter() {
	Point nowPos = football->getPosition();
	Point moveVec = lastPos - nowPos;
	if(nowPos.x < 1335 || nowPos.x > 3405) moveVec.x = 0.0f;
	if(nowPos.y < 563 || nowPos.y > 2173) moveVec.y = 0.0f;
	this->runAction(MoveBy::create(2.0f , moveVec));
	lastPos = football->getPosition();
}

void GreenSward::addObject()
{
	if(strcmp(GameWorld::getInstance()->getInitPostion().c_str(),"left") == 0) {
		teamA = new TeamSprite(0 ,4); 
		teamB = new TeamSprite(4 ,8);
		teamA->setAttaciDirection(hello_RIGHT);
		teamB->setAttaciDirection(hello_LEFT);
	} else {
		teamA = new TeamSprite(4 ,8); 
		teamB = new TeamSprite(0 ,4);
		teamA->setAttaciDirection(hello_LEFT);
		teamB->setAttaciDirection(hello_RIGHT);
	}
	football = new Football();
	GameWorld::getInstance()->setTeamAPoint(teamA);
	GameWorld::getInstance()->setTeamBPoint(teamB);
	GameWorld::getInstance()->setFootballPoint(football);
	this->addChild(football);
	this->addChild(teamA);
	this->addChild(teamB);
	teamB->setPosition(2200 ,1200);
	teamA->setPosition(2200 , 1200);

	for(int i = 0 ;i < 4; i++) {
		GameWorld::getInstance()->teamA->Player[i]->setPower(GameWorld::getInstance()->A.player->strength);
		GameWorld::getInstance()->teamA->Player[i]->setSpeed(calLog(GameWorld::getInstance()->A.player->speed)+ BASESPEED);
		GameWorld::getInstance()->teamA->Player[i]->setShoot(GameWorld::getInstance()->A.player->accuracy);
	}

	for(int i = 0 ;i < 4; i++) {
		GameWorld::getInstance()->teamB->Player[i]->setPower(GameWorld::getInstance()->B.player->strength);
		GameWorld::getInstance()->teamB->Player[i]->setSpeed(calLog(GameWorld::getInstance()->B.player->speed) + BASESPEED);
		GameWorld::getInstance()->teamB->Player[i]->setShoot(GameWorld::getInstance()->B.player->accuracy);
	}

	football->setPosition(this->getContentSize().width  / 2 ,this->getContentSize().height / 2 - 150);
	this->scheduleUpdate();
	lastPos = football->getPosition();
}

float GreenSward::calLog(float val)
{
	val *= 10;
	return log(val) / log(2);
}
