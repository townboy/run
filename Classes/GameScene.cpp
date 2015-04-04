#include "GameScene.h"
#include "GameUI.h"
#include "Athlete.h"
#include <iostream>
#include <cmath>
#include <vector>

// This is a Scene

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

	auto joystick = Joystick::create();
	scene->addChild(joystick, 100);

    // return the scene

    return scene;
}

bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    //add the backGround
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


	//init
	greenSward = GreenSward::create("green_sward.jpg");
	greenSward->setTextureRect(cocos2d::Rect(0, 0, 4576.0, 2835.0));
	//greenSward = dynamic_cast<GreenSward*>new Sprite();
    greenSward->addObject();
	greenSward->setPosition(visibleSize.width / 2 ,visibleSize.height / 2);
	this->addChild(greenSward);
	joyStickDirection = 8;
	initJoyStick();
	initButtonA();
	initButtonB();
	//add the UI
	auto gameUI = GAMEUI::create();
	GameWorld::getInstance()->setGameUI(gameUI);
	this->addChild(gameUI);
	this->scheduleUpdate();
	return true;
}


void GameScene::initButtonA()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto closeItem = MenuItemImage::create(
                                           "buttonA.png",
                                           "buttonDown.png",
                                           CC_CALLBACK_1(GameScene::buttonACloseCallback, this));
    
	closeItem->setPosition(visibleSize.width - closeItem->getContentSize().width * 2 - 50,visibleSize.height / 8 + 50);

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 100);
}

void GameScene::initButtonB()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto closeItem = MenuItemImage::create(
                                           "buttonB.png",
                                           "buttonDown.png",
                                           CC_CALLBACK_1(GameScene::buttonBCloseCallback, this));
    
	closeItem->setPosition(visibleSize.width - closeItem->getContentSize().width * 2 + 250,visibleSize.height / 7 + 150);

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 100);
}

void GameScene::initJoyStick()
{
	//init for the joy stick
	auto _listener = EventListenerCustom::create(JoystickEvent::EVENT_JOYSTICK, [=](EventCustom* event){
        JoystickEvent* jsevent = static_cast<JoystickEvent*>(event->getUserData());
        log("!!!!!!!!!!!!!!!--------------got joystick event, %p,  angle=%f", jsevent, jsevent->mAnagle);
		joyStickDirection = calDirection(jsevent->mAnagle);
    });

    _eventDispatcher->addEventListenerWithFixedPriority(_listener, 1);
	GameWorld::getInstance()->setStickJoyDirection(&joyStickDirection);
	CCLOG("!QSFDSFDSFDSFDSDSFFFFFFFFFFFF");
}

int GameScene::calDirection(float angle)
{
	if(angle > -22.5f && angle < 22.5f) return hello_RIGHT;
	else if(angle > 22.5f && angle < 67.5f) return hello_UPRIGHT;
	else if(angle > 67.5f && angle < 112.5f) return hello_UP;
	else if(angle > 112.5f && angle < 157.5f) return hello_UPLEFT;
	else if((angle > 157.5f && angle < 1000.0f )|| angle < -157.5f) return hello_LEFT;
	else if(angle > -157.5f && angle < -112.5f) return hello_DOWNLEFT;
	else if(angle > -112.5f && angle < -67.5f) return hello_DOWN;
	else if(angle > -67.5f && angle < -22.5f) return hello_DOWNRIGHT;
	else if(angle == 1000.0f) return STATIC;
}
void GameScene::ApassBall() {
	
	if(GameWorld::getInstance()->teamA->attackIng() == false) return  ;
	std::vector<float> degreeVec = GameWorld::getInstance()->teamA->degreeCal();
	for(int i = 0;i < 4;i ++)
	{
		int direction = calDirection(degreeVec[i]);
		if(i == GameWorld::getInstance()->teamA->getControlID()) continue;
		if(direction == GameWorld::getInstance()->teamA->getMainDirection())
		{
			Point targetPosition = GameWorld::getInstance()->teamA->getIDPosition(i) + GameWorld::getInstance()->teamA->getPosition();
			GameWorld::getInstance()->football->setLastBelong();
			hitBallA(targetPosition);
			return  ;
		}
	}
	int mainDirection = GameWorld::getInstance()->teamA->getMainDirection();
	Point ballPos = GameWorld::getInstance()->football->getPosition();
	GameWorld::getInstance()->football->setLastBelong();
	switch(mainDirection)
	{
	case hello_RIGHT:
		hitBallA(ballPos + Point(500.0f , 0.0f));
		break;
	case hello_LEFT:
		hitBallA(ballPos + Point(-500.0f ,0.0f));
		break;
	case hello_UP:
		hitBallA(ballPos + Point(0.0f ,500.0f));
		break;
	case hello_DOWN:
		hitBallA(ballPos + Point(0.0f ,-500.0f));
		break;
	case hello_UPRIGHT:
		hitBallA(ballPos + Point(300.0f ,300.0f));
		break;
	case hello_UPLEFT:
		hitBallA(ballPos + Point(-300.0f ,300.0f));
		break;
	case hello_DOWNLEFT:
		hitBallA(ballPos + Point(-300.0f ,-300.0));
		break;
	case hello_DOWNRIGHT:
		hitBallA(ballPos + Point(300.0f ,-300.0f));
		break;
	default:
		break;
	}
	return  ;
}

void GameScene::BpassBall() {
	if(GameWorld::getInstance()->teamB->attackIng() == false) return  ;
	std::vector<float> degreeVec = GameWorld::getInstance()->teamB->degreeCal();
	for(int i = 0;i < 4;i ++)
	{
		int direction = calDirection(degreeVec[i]);
		if(i == GameWorld::getInstance()->teamB->getControlID()) continue;
		if(direction == GameWorld::getInstance()->teamB->getMainDirection())
		{
			Point targetPosition = GameWorld::getInstance()->teamB->getIDPosition(i) + GameWorld::getInstance()->teamB->getPosition();
			GameWorld::getInstance()->football->setLastBelong();
			hitBallB(targetPosition);
			return  ;
		}
	}
	int mainDirection = GameWorld::getInstance()->teamB->getMainDirection();
	Point ballPos = GameWorld::getInstance()->football->getPosition();
	GameWorld::getInstance()->football->setLastBelong();
	switch(mainDirection)
	{
	case hello_RIGHT:
		hitBallB(ballPos + Point(500.0f , 0.0f));
		break;
	case hello_LEFT:
		hitBallB(ballPos + Point(-500.0f ,0.0f));
		break;
	case hello_UP:
		hitBallB(ballPos + Point(0.0f ,500.0f));
		break;
	case hello_DOWN:
		hitBallB(ballPos + Point(0.0f ,-500.0f));
		break;
	case hello_UPRIGHT:
		hitBallB(ballPos + Point(300.0f ,300.0f));
		break;
	case hello_UPLEFT:
		hitBallB(ballPos + Point(-300.0f ,300.0f));
		break;
	case hello_DOWNLEFT:
		hitBallB(ballPos + Point(-300.0f ,-300.0));
		break;
	case hello_DOWNRIGHT:
		hitBallB(ballPos + Point(300.0f ,-300.0f));
		break;
	default:
		break;
	}
	return  ;
}

void GameScene::buttonACloseCallback(Ref* pSender)
{
	//正在持球
	GameWorld::getInstance()->passBall = 1;
}

void GameScene::buttonBCloseCallback(Ref* pSender)
{
	GameWorld::getInstance()->shootBall = 1;
	if(GameWorld::getInstance()->teamA->getControner() == GameWorld::getInstance()->football->getControner()){
		AshootBall();
	}
}

void GameScene::hitBallA(Point targetPosition)
{
	GameWorld::getInstance()->football->setBelong(NULL);
	GameWorld::getInstance()->teamA->setDenfenseStatus();
	Point footballPos = GameWorld::getInstance()->football->getPosition();
	Point moveVec = footballPos - targetPosition;
	float dist = sqrt(moveVec.x * moveVec.x + moveVec.y * moveVec.y);
	GameWorld::getInstance()->football->runAction(MoveTo::create(dist / 400.0 ,targetPosition));
}

void GameScene::hitBallB(Point targetPosition)
{
	GameWorld::getInstance()->football->setBelong(NULL);
	GameWorld::getInstance()->teamB->setDenfenseStatus();
	Point footballPos = GameWorld::getInstance()->football->getPosition();
	Point moveVec = footballPos - targetPosition;
	float dist = sqrt(moveVec.x * moveVec.x + moveVec.y * moveVec.y);
	GameWorld::getInstance()->football->runAction(MoveTo::create(dist / 400.0 ,targetPosition));
}

void GameScene::update(float e)
{
}

void GameScene::AshootBall()
{
	shootBall(GameWorld::getInstance()->teamA);
}


void GameScene::shootBall(TeamSprite *team)
{
	int direction = switchDirection(team->getMainDirection());
	Point footballPos = GameWorld::getInstance()->football->getPosition();
	float xDis = -1;
	if(direction == hello_LEFT)
	{
		xDis = footballPos.x - 510.0f;
	}
	else{
		xDis = 4075.0f - footballPos.x; 
	}
	//CCLOG("football:(%f ,%f) xDis:%f\n",footballPos.x ,footballPos.y ,xDis);
	float probability = 1.0f;
	if(xDis > 1700.0f) probability = 0.0f;
	else probability = (1700.0f - xDis) / 1700.0f;
	int pro = (int)(probability * 100);
	//增加运动员的天赋
	pro += team->getControner()->getShoot();
	pro = std::min(100 ,pro);
	srand(time(NULL));
	int val = random(0 ,100);
	CCLOG("val:%d  pro:%d  xDis:%f   football:(%f ,%f)\n",val ,pro ,xDis ,footballPos.x ,footballPos.y);
	if(val < pro){
		int lowerBound = 800.0f;
		int upperBound = 1700.0f;
		int offset = (int)(20.0f * (10.0f - team->getControner()->getShoot()));
		lowerBound -= offset;
		upperBound += offset;
		int val = random(lowerBound ,upperBound);
		shootBallSuccess(val ,direction);
	}
	else {
		//第一步就失败了
		shootBallFail(direction);
	}
}

void GameScene::shootBallSuccess(int y ,int direction)
{
	Point targetPosition = Point(509.0f ,y * 1.0f);
	if(direction == hello_RIGHT) targetPosition.x = 4076.0f;
	GameWorld::getInstance()->footballTarget = targetPosition;
}

void GameScene::shootBallFail(int direction)
{
	srand(time(NULL));
	int val = cocos2d::random() % 2;
	Point targetPosition = Point(560.0f ,900.0f);

	if(val == 1) targetPosition.y = 1560.0f;
	if(direction == hello_RIGHT) targetPosition.x = 4000;
	GameWorld::getInstance()->footballTarget = targetPosition;
}

void GameScene::catchBall(TeamSprite *team)
{
	CCLOG("catch the footBall\n");
	Point posInGrass = team->getControner()->getPosition() + team->getPosition();
	Point footballPos = GameWorld::getInstance()->football->getPosition();
	posInGrass.y -= team->getControner()->getContentSize().height / 2;
	Vec2 distVec = posInGrass - footballPos;
	double dist = distVec.x * distVec.x + distVec.y * distVec.y;
	if(dist < 500){
		GameWorld::getInstance()->football->setBelong(team->getControner());
		GameWorld::getInstance()->football->setLastBelong();
	}
}

int GameScene::switchDirection(int direction)
{
	switch(direction)
	{
	case hello_LEFT:
		return hello_LEFT;
	case hello_UPLEFT:
		return hello_LEFT;
	case hello_DOWNLEFT:
		return hello_LEFT;
	default:
		return hello_RIGHT;
	}
}
