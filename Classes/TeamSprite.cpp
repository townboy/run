#include "TeamSprite.h"
#include <cmath>
#define PI acos(-1.0)

TeamSprite::TeamSprite(int start ,int end) {
	init();
	int y = 200;
	for(int i = start; i < end; i++) {
		int y = i * 50;
		int x = 0;
		int id = i;
		if(i >= 4){
			x = 200;
			y = (i - 4) * 50;
			id -= 4;
		}
		Player[id] = new Athlete(i);
		Player[id]->setPosition(cocos2d::Vec2(x,y));
		Player[id]->direction = 0;
		this->addChild(Player[id] ,100);
		CCLOG("TeamSprite : id:%d\n:",id);
	}
	this->goal = 0;
	this->control = 0;
	this->scheduleUpdate();
}

void TeamSprite::reset(int start ,int end)
{
	int y = 200;
	for(int i = start; i < end; i++) {
		int y = i * 50;
		int x = 0;
		int id = i;
		if(i >= 4){
			x = 200;
			y = (i - 4) * 50;
			id -= 4;
		}
		Player[id]->setPosition(cocos2d::Vec2(x,y));
		Player[id]->direction = 0;
	}
}

void TeamSprite::addGoal()
{
	goal += 1;
}

void TeamSprite::contronlPlayer(int id ,int action)
{
	Athlete * &player = Player[id];
	switch(action)
	{
	case hello_LEFT:
		player->moveLeft();
		break;
	case hello_RIGHT:
		player->moveRight();
		break;
	case hello_UP:
		player->moveUp();
		break;
	case hello_DOWN:
		player->moveDown();
		break;
	case hello_UPLEFT:
		player->moveUpLeft();
		break;
	case hello_UPRIGHT:
		player->moveUpRight();
		break;
	case hello_DOWNRIGHT:
		player->moveDownRight();
		break;
	case hello_DOWNLEFT:
		player->moveDownLeft();
		break;
	default:
		break;
	}
}

void TeamSprite::contronlFromButton(int action)
{
	this->contronlPlayer(control ,action);
	//under the attack status
}

void TeamSprite::contronlFromAI(int id ,int action)
{
	this->contronlPlayer(id ,action);
}

void TeamSprite::contronlFromWeb(int id,int action)
{
	this->contronlPlayer(id ,action);
}

Vec2 TeamSprite::getMainPosition()
{
	return this->Player[control]->getPosition();
}

void TeamSprite::setAttackStatus()
{
	this->attackStatus = true;
}

void TeamSprite::setDenfenseStatus()
{
	this->attackStatus = false;
}

bool TeamSprite::attackIng()
{
	return this->attackStatus;
}

Athlete * TeamSprite::getControner()
{
	return this->Player[this->control];
}

int TeamSprite::getScore()
{
	return this->goal;
}


int TeamSprite::getControlID()
{
	return this->control;
}
std::vector<float> TeamSprite::degreeCal()
{
	std::vector<float> ret;
	Point mainPos = Player[this->control]->getPosition();
	for(int i = 0;i < 4;i ++)
	{
		if(i == this->control) ret.push_back(0.0f);
		else{
			Point nowPos = Player[i]->getPosition();
			Point calPos = nowPos - mainPos;
			float tanThta = calPos.y * 1.0 / calPos.x;
			float degree = atan(tanThta) * 180.0 / PI;
			if(calPos.x > 0 && calPos.y < 0) degree =  degree;
			else if(calPos.x < 0 && calPos.y > 0) degree = 180.0 + degree;
			else if(calPos.x < 0 && calPos.y < 0) degree = degree - 180.0f;
			ret.push_back(degree);
		}
	}
	return ret;
}

int TeamSprite::getMainDirection()
{
	return Player[this->control]->direction;
}

Vec2 TeamSprite::getIDPosition(int id)
{
	return Player[id]->getPosition();
}

int TeamSprite::testMember(Athlete *toTest)
{
	for(int i = 0;i < 4;i ++){
		if(Player[i] == toTest) return i;
	}
	return -1;
}

void TeamSprite::setControl(int id)
{
	this->control = id;
}


std::vector<Point> TeamSprite::calPos(){
	std::vector<Point> ret;
	Point parentPos = this->getPosition();
	for(int i = 0;i < 4;i ++){
		ret.push_back(Player[i]->getPosition() + parentPos);
	}
	return ret;
}

void TeamSprite::setAttaciDirection(int dirction)
{
	this->attackDirection = dirction;
}

int TeamSprite::getAttackDirection()
{
	return this->attackDirection;
}