#include "Football.h"
#include "Athlete.h"
#include "GameWorld.h"

float Football::ballDist = 5.0f;

Football::Football()
{
	init();
	Vector<SpriteFrame *> frames;
	char buff[255];
	for(int i = 1;i <= 3;i ++)
	{
		sprintf(buff ,"football_%d.png" ,i);
		auto pic = Director::getInstance()->getTextureCache()->addImage(buff);
		auto frame = SpriteFrame::createWithTexture(pic,Rect(0,0,pic->getContentSize().width,pic->getContentSize().height));
		this->setPosition(Vec2(pic->getContentSize().width / 2 + 100,pic->getContentSize().height / 2));
		frames.pushBack(frame);
	}
	auto info = Animation::createWithSpriteFrames(frames ,1.0f / 10.f ,int(1e9));
	runingAction = Animate::create(info);
	this->initWithFile("football.png");
	this->runAction(runingAction);
	this->scheduleUpdate();
	runingAction->retain();
	belong = NULL;
	lastBelong = NULL;
	ballOut = false;
	ballOutTeam = NULL;
}

void Football::moveLeft()
{
	if(direction == hello_LEFT) return;
	this->move(Vec2(-1.0 * ballDist ,0.0f));
	direction = hello_LEFT;
}

void Football::moveRight()
{
	if(direction == hello_RIGHT) return;
	this->move(Vec2(1.0f * ballDist ,0.0f));
	direction = hello_RIGHT;
}

void Football::moveUp()
{
	if(direction == hello_UP) return ;
	this->move(Vec2(0.0f ,1.0 * ballDist));
	direction = hello_UP;
}

void Football::moveDown()
{
	if(direction == hello_DOWN) return ;
	this->move(Vec2(0.0f ,-1.0 * ballDist));
	direction = hello_DOWN;
}

void Football::moveUpLeft()
{
	this->move(Vec2(-1.0f * ballDist ,1.0f * ballDist));
}

void Football::moveUpRight()
{
	this->move(Vec2(1.0f * ballDist ,1.0f * ballDist));
}

void Football::moveDownLeft()
{
	this->move(Vec2(-1.0f * ballDist ,-1.0f * ballDist));
}

void Football::moveDownRight()
{
	this->move(Vec2(1.0f * ballDist ,-1.0f * ballDist));
}

void Football::move(Vec2 vect)
{
	auto action = JumpTo::create(2.0f ,this->getPosition() + vect ,10 ,2);
	this->runAction(action);
}

void Football::jump() {
	//
}

void Football::update(float dt) {
	
	 
	//如果得分了
	if(goalDetect())
	{
		auto teamA = GameWorld::getInstance()->teamA;
		auto teamB = GameWorld::getInstance()->teamB;
		//进左边的门
		if(getPositionX() < 510){
			if(teamA->getAttackDirection() == hello_LEFT){
				teamA->addGoal();
			}
			else teamB->addGoal();
			GameWorld::getInstance()->gameUI->getGoal();
			GameWorld::getInstance()->resetWorld();
			return ;
		}
		else{
			if(teamA->getAttackDirection() == hello_RIGHT){
				teamA->addGoal();
			}
			else teamB->addGoal();
			GameWorld::getInstance()->gameUI->getGoal();
			GameWorld::getInstance()->resetWorld();
			return ;
		}
	}

	//如果球出界了
	if(boundDetect())
	{
		CCLOG("OUT");
		auto who = this->getControner();
		if(who == NULL) who = this->getLastControner();
		GameWorld::getInstance()->gameUI->mistakeDisplay();
		GameWorld::getInstance()->gameUI->mistakeRemove();
		Point targetPos = this->getPosition();
		targetPos.x = std::max(targetPos.x , 550.0f);
		targetPos.x = std::min(targetPos.x , 4000.0f);
		targetPos.y = std::max(targetPos.y ,200.0f);
		targetPos.y = std::min(targetPos.y , 2320.0f);
		this->belong = NULL;
		this->stopAllActions();
		this->runAction(runingAction);
		GameWorld::getInstance()->football->setPosition(targetPos);
		//己方犯规
		TeamSprite * team =  GameWorld::getInstance()->teamA;
		if(team->testMember(who) != -1) team = GameWorld::getInstance()->teamB;
		Point fromPos = team->getControner()->getPosition() + team->getPosition();
		Point distVec = targetPos - fromPos;
		auto belongChange = CallFunc::create(this ,callfunc_selector(Football::afterBallOut));
		ballOutTeam = team;
		team->getControner()->runAction(Sequence::create(MoveBy::create(1.0f ,distVec) , belongChange ,NULL));
	}
	

	//刷新球的位置
	if(belong == NULL) return ;
	int athleteDirection = belong->direction;
	switch(athleteDirection)
	{
	case hello_LEFT:
		this->setPosition(belong->getPosition() + belong->getParent()->getPosition() + Point(-20 ,-23));
		break;
	case hello_RIGHT:
		this->setPosition(belong->getPosition() + belong->getParent()->getPosition() + Point(21 ,-24));
		break;
	case hello_UP:
		this->setPosition(belong->getPosition() + belong->getParent()->getPosition() + Point(1 ,-17));
		break;
	case hello_DOWN:
		this->setPosition(belong->getPosition() + belong->getParent()->getPosition() + Point(23 , -36));
		break;
	case hello_UPLEFT:
		this->setPosition(belong->getPosition() + belong->getParent()->getPosition() + Point(-10 ,-13));
		break;
	case hello_UPRIGHT:
		this->setPosition(belong->getPosition() + belong->getParent()->getPosition() + Point(20 ,-16));
		break;
	case hello_DOWNLEFT:
		this->setPosition(belong->getPosition() + belong->getParent()->getPosition() + Point(-25 ,-36));
		break;
	case hello_DOWNRIGHT:
		this->setPosition(belong->getPosition() + belong->getParent()->getPosition() + Point(21 ,-36));
		break;
	default:
		//CCLOG("OH ,HERE!");
		break;
	}
	
}

void Football::afterBallOut()
{
	this->belong = ballOutTeam->getControner();
}

void Football::setBelong(Athlete *player)
{
	this->belong = player;
}

void Football::setLastBelong()
{
	this->lastBelong = this->belong;
}

bool Football::withoutContronl()
{
	if(this->belong == NULL) return true;
	return false;
}


Athlete * Football::getControner()
{
	return this->belong;
}

Athlete * Football::getLastControner()
{
	return this->lastBelong;
}

bool Football::boundDetect()
{
	Point pos = this->getPosition();
	if(pos.x < 504 || pos.x > 4080) return true;
	if(pos.y < 119 || pos.y > 2349) return true;
	return false;
}

bool Football::goalDetect()
{
	Point pos = this->getPosition();
	if(pos.x < 510 || pos.x > 4075){
		if(pos.y > 929.0f && pos.y < 1525.7f) return true;
	}
	return false;
}
