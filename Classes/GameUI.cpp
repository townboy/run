#include "GameUI.h"
#include "GameWorld.h"
#include "EndScene.h"
#include "BlackScene.h"
#include "StartScene.h"
#include <vector>
#define SPEEDUPTIME 120

// This is a Scene

Scene* GAMEUI::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GAMEUI::create();

    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene

    return scene;
}

bool GAMEUI::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
	}
	point = Sprite::create("which.png");
	GameWorld::getInstance()->teamA->addChild(point , 1);
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	clock = Sprite::create("clock.png");
	this->addChild(clock);
	clock->setPosition(visibleSize.width / 2 - clock->getContentSize().width , visibleSize.height - clock->getContentSize().height) ;
	CCLOG("clock position:(%f ,%f)\n",clock->getPositionX() ,clock->getPositionY());
	num[0] = Sprite::create("time_0.png");
	num[0]->setPosition(910.0f ,952.0f);
	num[1] = Sprite::create("time_0.png");
	num[1]->setPosition(1000.0f ,952.0f);
	num[2] = Sprite::create("time_0.png");
	num[2]->setPosition(1040.0f ,952.0f);
	this->addChild(num[0]);
	this->addChild(num[1]);
	this->addChild(num[2]);
	auto split = Sprite::create("split_2.png");
	split->setPosition(960.0f ,960.f);
	this->addChild(split);
	getCurTime(startTime);
	CCLOG(startTime);
	this->scheduleUpdate();
	initMap();
	mistake = Sprite::create("mistake.png");
	mistake->retain();
	mistake->setPosition(Point(visibleSize.width / 2 ,visibleSize.height / 2));
	mistakeAdd = false;
	lastTime = 599;
	//lastTime = 10;
	initPower();
	AspeedUpCount = BspeedUpCount = 0;
	return true;
}

void GAMEUI::speedUp(TeamSprite * team)
{
	if(coldTime) return ;
	if(team== GameWorld::getInstance()->teamA){
		Vec2 size = restPower->getContentSize();
		powerSize -= 0.08f;
		if(powerSize <= 0)
		{
			coldTime = 300;
			getCold();
		}
		restPower->runAction(ScaleTo::create(0.1f ,1.0f ,powerSize ,1.0f));
		Point toPos = restPower->getPosition();
		toPos.y += 10;
		restPower->setPosition(toPos);
	}
	team->getControner()->addSpeed();
	if(team == GameWorld::getInstance()->teamA) {
		AspeedUpCount = SPEEDUPTIME;
	}
	else BspeedUpCount = SPEEDUPTIME;
}

void GAMEUI::getCold()
{
	cold->setPosition(power->getPosition());
}

void GAMEUI::resumePower()
{
	resumeCount += 1;
	if(resumeCount == 60){
		resumeCount = 0;
		if(powerSize >= 1.0f) return ;
		powerSize += 0.08f;
		restPower->runAction(ScaleTo::create(0.1f ,1.0f ,powerSize ,1.0f));
		Point toPos = restPower->getPosition();
		toPos.y -= 10;
		restPower->setPosition(toPos);
	}
}

void GAMEUI::initPower()
{
	resumeCount = 0;
	powerSize = 1.0f;
	coldTime = 0;
	power = Sprite::create("power.jpg");
	Point pos = smallMap->getPosition();
	pos.x += smallMap->getContentSize().width / 2 - 110;
	power->setPosition(pos);
	this->addChild(power);
	restPower = Sprite::create("rest_power.png");
	restPower->setPosition(pos);
	this->addChild(restPower);
	cold = Sprite::create("cold.jpg");
	this->addChild(cold);
	cold->setPosition(-1.0f ,-1.0f);
}

void GAMEUI::mistakeDisplay()
{
	if(mistakeAdd == false){
		this->addChild(mistake);
	}
	mistakeAdd = true;
}

void GAMEUI::mistakeRemove()
{
	auto removeAction = CallFunc::create(this ,callfunc_selector(GAMEUI::callbackC));
	auto waitAction = DelayTime::create(3.0f);
	this->runAction(Sequence::create(waitAction ,removeAction ,NULL));
}

void GAMEUI::callbackC()
{
	this->removeChild(mistake);
	mistakeAdd = false;
	auto tmp = BlackScene::createScene();
	Director::getInstance()->pushScene(tmp);
}

void GAMEUI::initMap()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	smallMap = Sprite::create("smallMap.png");
	this->addChild(smallMap);
	smallMap->setPosition(visibleSize.width / 2 ,smallMap->getContentSize().height / 3);
	char tmp[20];
	for(int i = 0;i < 4;i ++){
		sprintf(tmp ,"teamA_%d.png",i + 1);
		CCLOG("NAME:%s\n" ,tmp);
		playerPoint[i] = Sprite::create(tmp);
		playerPoint[i]->setPosition(smallMap->getContentSize().width / 2 ,smallMap->getContentSize().height / 2 );
		smallMap->addChild(playerPoint[i]);
	}
	for(int i = 4;i < 8;i ++){
		sprintf(tmp ,"ID2_%d.png" ,i - 3);
		playerPoint[i] = Sprite::create(tmp);
		playerPoint[i]->setPosition(smallMap->getContentSize().width / 2 ,smallMap->getContentSize().height / 2 );
		smallMap->addChild(playerPoint[i]);
	}
	CCLOG("point(%f ,%f)\n",playerPoint[0]->getParent()->getContentSize().width / 2 ,playerPoint[0]->getParent()->getContentSize().height / 2 );
}

void GAMEUI::getCurTime(char *timeSrc)
{
	time_t t = time(0);
	strftime( timeSrc, 20, "%X",localtime(&t));
}

void GAMEUI::getGoal()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
	auto goal = Sprite::create("goal_1.png");
	goal->setPosition(visibleSize.width / 2 ,visibleSize.height / 2);
	auto actionFade = FadeOut::create(3.0f);
	auto actionRemove = RemoveSelf::create();
	goal->runAction(Sequence::create(actionFade , actionRemove ,NULL));
	this->addChild(goal);
}
int GAMEUI::game_ui_update() {
	drawMap();
	getCurTime(curTime);
	int intval = calTime();
	if(intval < 0){
		CCLOG("OK");
		return -1;
	}
	int first = intval / 60;
	int second = intval % 60 / 10;
	int third = intval % 60 % 10;
	char fileName[20];
	sprintf(fileName ,"time_%d.png",first);
	CCTexture2D* texture0 = CCTextureCache::sharedTextureCache()->addImage(fileName);  
    num[0]->setTexture(texture0);  

	
	sprintf(fileName ,"time_%d.png",second);
	CCTexture2D* texture1 = CCTextureCache::sharedTextureCache()->addImage(fileName);  
    num[1]->setTexture(texture1);  

	
	sprintf(fileName ,"time_%d.png",third);
	CCTexture2D* texture2 = CCTextureCache::sharedTextureCache()->addImage(fileName);  
    num[2]->setTexture(texture2);  
	return 0;
}

void GAMEUI::update(float e)
{
	if(coldTime) coldTime --;
	else{
		cold->setPosition(-1.0f ,-1.0f);
	}
	setPoint();
	resumePower();
	if(AspeedUpCount){
		AspeedUpCount --;
		if(AspeedUpCount == 0){
			for(int i = 0;i < 4;i ++)
				GameWorld::getInstance()->teamA->Player[i]->toInit();
		}
	}
	if(BspeedUpCount){
		BspeedUpCount --;
		if(BspeedUpCount == 0){
			for(int i = 0;i < 4;i ++)
				GameWorld::getInstance()->teamB->Player[i]->toInit();
		}
	}
}

int GAMEUI::calTime()
{
	int a = ((startTime[0] - '0') * 10 + (startTime[1] - '0')) * 3600;
	int b = ((startTime[3] - '0') * 10 + (startTime[4] - '0')) * 60;
	int c = ((startTime[6] - '0') * 10 + (startTime[7] - '0'));
	int first = a + b + c;

	a = ((curTime[0] - '0') * 10 + (curTime[1] - '0')) * 3600;
	b = ((curTime[3] - '0') * 10 + (curTime[4] - '0')) * 60;
    c = ((curTime[6] - '0') * 10 + (curTime[7] - '0'));
	int second = a + b + c;
	return lastTime - (second - first);

}

void GAMEUI::drawMap()
{
	std::vector<Point> playerAPosVec = GameWorld::getInstance()->teamA->calPos();
	for(int i = 0;i < 4;i ++){
		playerAPosVec[i].x /= 7.5f;
		playerAPosVec[i].y /= 5.5f;
		playerAPosVec[i].x += 85;
		playerPoint[i]->setPosition(playerAPosVec[i]);
	}

	std::vector<Point> playerBPosVec = GameWorld::getInstance()->teamB->calPos();
	for(int i = 0;i < 4;i ++){
		playerBPosVec[i].x /= 7.5f;
		playerBPosVec[i].y /= 5.5f;
		playerBPosVec[i].x += 85;
		playerPoint[i + 4]->setPosition(playerBPosVec[i]);
	}
}

void GAMEUI::callbackC2()
{
}

void GAMEUI::setPoint()
{
	auto player = GameWorld::getInstance()->teamA->getControner();
	point->setPosition(player->getPosition() - player->getContentSize() / 2 + Point(27 ,10) );
}