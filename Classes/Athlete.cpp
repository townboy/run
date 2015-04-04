#include "Athlete.h"
#include "GameWorld.h"
#include <vector>
  
USING_NS_CC;
using namespace std;

void Athlete::action_pass() {
}

void Athlete::action_block() {
}

void Athlete::action_shoot() {
}

void Athlete::action_putDown() {
}

void Athlete::action_getBall()
{
	if(GameWorld::getInstance()->football->withoutContronl() == false) return ;
	Point posInGrass = this->getPosition() + this->getParent()->getPosition();
	Point footballPos = GameWorld::getInstance()->football->getPosition();
	posInGrass.y -= this->getContentSize().height / 2;
	Vec2 distVec = posInGrass - footballPos;
	double dist = distVec.x * distVec.x + distVec.y * distVec.y;
	if(dist < 500){
		GameWorld::getInstance()->football->setBelong(this);
		TeamSprite *team = (TeamSprite *)(this->getParent());
		int controlID = team->testMember(this);
		team->setControl(controlID);
		team->setAttackStatus();
	}
}

void Athlete::setShoot(float val)
{
	this->shoot = val;
}

int Athlete::getShoot()
{
	return (int)(shoot / 10);
}

void Athlete::setSpeed(float val)
{
	this->speed = val;
}

void Athlete::setPower(float val)
{
	this->power = val;
}

Athlete::Athlete(int id)
{
	skillSpeed = 0.0f;
	//init for the right 
	this->init();
	Vector<SpriteFrame *> frames;
	char buff[255];
	for(int i = 0;i <= 6;i ++)
	{
		sprintf(buff ,"right_%d.png" ,i);
		auto pic = Director::getInstance()->getTextureCache()->addImage(buff);
		auto frame = SpriteFrame::createWithTexture(pic,Rect(0,0,pic->getContentSize().width,pic->getContentSize().height));
		this->setPosition(Vec2(pic->getContentSize().width / 2 + 100,pic->getContentSize().height / 2));
		frames.pushBack(frame);
	}
	auto info = Animation::createWithSpriteFrames(frames ,1.0f / 10.f ,1000);
	right = Animate::create(info);

	//init for the down
	frames.clear();
	for(int i = 0;i <= 6;i ++)
	{
		sprintf(buff ,"down_%d.png" ,i);
		auto pic = Director::getInstance()->getTextureCache()->addImage(buff);
		auto frame = SpriteFrame::createWithTexture(pic,Rect(0,0,pic->getContentSize().width,pic->getContentSize().height));
		this->setPosition(Vec2(pic->getContentSize().width / 2 + 100,pic->getContentSize().height / 2));
		frames.pushBack(frame);
	}
	info = Animation::createWithSpriteFrames(frames ,1.0f / 10.f ,1000);
	down = Animate::create(info);

	
	//init for the down
	frames.clear();
	for(int i = 0;i <= 6;i ++)
	{
		sprintf(buff ,"up_%d.png" ,i);
		auto pic = Director::getInstance()->getTextureCache()->addImage(buff);
		auto frame = SpriteFrame::createWithTexture(pic,Rect(0,0,pic->getContentSize().width,pic->getContentSize().height));
		this->setPosition(Vec2(pic->getContentSize().width / 2 + 100,pic->getContentSize().height / 2));
		frames.pushBack(frame);
	}
	info = Animation::createWithSpriteFrames(frames ,1.0f / 10.f ,1000);
	up = Animate::create(info);
	this->runAction(down);
	this->scheduleUpdate();
	
	//avoid the action was released;
	this->up->retain();
	this->right->retain();
	this->down->retain();
	this->running = NULL;
	char fileName[20];
	if(id < 4) sprintf(fileName ,"id_%d.png" ,id + 1);
	else sprintf(fileName ,"ID2_%d.png" ,id - 3);
	id_sprite = Sprite::create(fileName);
	this->addChild(id_sprite);
	id_sprite->runAction(MoveBy::create(1.0f ,Vec2(15.0f ,90.0f)));
	this->id = id;
	this->speed = 15.0f;
	this->shoot = 10.0f;
	speedUP = 0.0f;
}

int Athlete::getID(){
	return this->id;
}

void Athlete::update(float e) {
	//每帧去检测是否可以拿到球
	if(GameWorld::getInstance()->football->getControner() != this) action_getBall();
}

bool Athlete::move(Vec2 vect)
{
	running = MoveTo::create(1.0f ,this->getPosition() + vect);
	this->running->retain();
	this->runAction(running);
	return true;
}

void Athlete::moveUp()
{
	if(this->direction != hello_UP){
		this->stopAllActions();
		auto tmp = ScaleTo::create(0.0f ,1.0f * SCALE,1.0f * SCALE);
		this->runAction(Spawn::create(tmp ,up ,NULL));
	}
	this->move(Vec2(0.0f ,(speed + speedUP) ));
	this->direction = hello_UP;
	
}

void Athlete::moveDown()
{
	if(this->direction != hello_DOWN){
		this->stopAllActions();
		auto tmp = ScaleTo::create(0.0f ,1.0f * SCALE,1.0f * SCALE);
		this->runAction(Spawn::create(tmp ,down ,NULL));
	}
	this->move(Vec2(0.0f ,-1.0f * (speed + speedUP) ));
	this->direction = hello_DOWN;
}

void Athlete::moveRight()
{
	if(this->direction != hello_RIGHT){
		this->stopAllActions();
		auto tmp = ScaleTo::create(0.0f ,1.0f * SCALE ,1.0f * SCALE);
		this->runAction(Spawn::create(tmp ,right ,NULL));
	}
	this->move(Vec2((speed + speedUP)  ,0.0f));
	this->direction = hello_RIGHT;
}

void Athlete::moveLeft()
{
	if(this->direction != hello_LEFT){
		this->stopAllActions();
		auto tmp = ScaleTo::create(0.0f ,-1.0f * SCALE ,1.0f * SCALE);
		this->runAction(Spawn::create(tmp ,right ,NULL));
	}
	this->move(Vec2(-1.0f * (speed + speedUP)  ,0.0f));
	this->direction = hello_LEFT;
}

void Athlete::moveUpLeft()
{
	if(this->direction != hello_UPLEFT){
		this->stopAllActions();
		auto tmp = ScaleTo::create(0.0f ,1.0f * SCALE ,1.0f * SCALE);
		this->runAction(Spawn::create(tmp ,up ,NULL));
	}
	this->move(Vec2(-1.0 * (speed + speedUP)  ,(speed + speedUP) ));
	this->direction = hello_UPLEFT;
}

void Athlete::moveUpRight()
{
	if(this->direction != hello_UPRIGHT){
		this->stopAllActions();
		auto tmp = ScaleTo::create(0.0f ,-1.0f * SCALE ,1.0f * SCALE);
		this->runAction(Spawn::create(tmp ,up ,NULL));
	}
	this->move(Vec2((speed + speedUP)  ,(speed + speedUP) ));
	this->direction = hello_UPRIGHT;
}

void Athlete::moveDownLeft()
{
	if(this->direction != hello_DOWNLEFT){
		this->stopAllActions();
		auto tmp = ScaleTo::create(0.0f ,-1.0f * SCALE ,1.0f * SCALE);
		this->runAction(Spawn::create(tmp ,down ,NULL));
	}
	this->direction = hello_DOWNLEFT;
	this->move(Vec2(-1.0f * (speed + speedUP)  ,-1.0 * (speed + speedUP) ));
}

void Athlete::moveDownRight()
{
	if(this->direction != hello_DOWNRIGHT){
		this->stopAllActions();
		auto tmp = ScaleTo::create(0.0f ,1.0f * SCALE ,1.0f * SCALE);
		this->runAction(Spawn::create(tmp ,down ,NULL));
	}
	this->direction = hello_DOWNRIGHT;
	this->move(Vec2((speed + speedUP)  ,-1.0 * (speed + speedUP) ));
}

void Athlete::toInit()
{
	this->speedUP = 0.0f;
}

void Athlete::addSpeed()
{
	this->speedUP = 10.0f;
}