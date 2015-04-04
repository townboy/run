#include "AI.h"
#include "GameWorld.h"
#include <vector>	
#define EPS 8.0f

AI::AI()
{
	this->init();
	counts = 0;
	for(int i = 0;i < 4;i ++){
		rest[i] = dir[i] = howLong[i] = 0;
		player[i] = false;
	}
	this->scheduleUpdate();
	lastTime = 15;
	ONOFF = true;
}

void AI::getBall(int *t_id, int *t_dir)
{
	if(GameWorld::getInstance()->football->getControner() != NULL) return ;
	Point footballPos = GameWorld::getInstance()->football->getPosition();
	float minDist = 1e10;
	int id = -1;
	TeamSprite *team = GameWorld::getInstance()->teamA;
	for(int i = 0;i < 4;i ++)
	{
		if(team->Player[i] == team->getControner()) continue;
		Point now = team->Player[i]->getPosition() + team->getPosition();
		Vec2 vec = now - footballPos;
		float dist = vec.x * vec.x + vec.y * vec.y;
		if(dist < minDist)
		{    
			minDist = dist;
			id = i;
		}
	}
	if(id == -1){
		CCLOG("ERROR!!!!!");
		return ;
	}
	*t_id = id;
	calOperation(id, t_dir);
}
void AI::calOperation(int id, int *t_dir)
{
	Point now = GameWorld::getInstance()->teamA->Player[id]->getPosition() + GameWorld::getInstance()->teamA->getPosition();
	now.y -= GameWorld::getInstance()->teamA->Player[id]->getContentSize().height / 2;
	Point to = GameWorld::getInstance()->football->getPosition();
	Vec2 vec = to - now;
	float degree = calDegree(vec);
	int direction = calDirection(degree);
	counts += 1;
	if(fabs(vec.x) > EPS){
		if(vec.x < 0) direction = hello_LEFT;
		else direction = hello_RIGHT;
	}
	else{
		if(vec.y > 0) direction = hello_UP;
		else direction = hello_DOWN;
	}
	CCLOG("vec:(%f ,%f)   degree:%f direction:%d\n",vec.x , vec.y ,degree ,direction);
	*t_dir = direction;
	//GameWorld::getInstance()->teamA->contronlFromAI(id ,direction);
}

float AI::calDegree(Vec2 vec)
{
	float tanThta = vec.y * 1.0 / vec.x;
	float degree = atan(tanThta) * 180.0 / 3.1415f;
	if(vec.x > 0 && vec.y < 0) degree =  degree;
	else if(vec.x < 0 && vec.y > 0) degree = 180.0 + degree;
	else if(vec.x < 0 && vec.y < 0) degree = degree - 180.0f;
	return degree;
}

int AI::calDirection(float angle)
{
	if(angle > 45.0 && angle < 135.0f) return hello_UP;
	else if(angle > -45.0f && angle < 45.0f) return hello_RIGHT;
	else if(angle < -45.0f && angle > -135.0f) return hello_DOWN;
	else return hello_LEFT;
}

void AI::run(int *array_dir ,int getBallID)
{
	Football * football = GameWorld::getInstance()->football;
	TeamSprite * team = GameWorld::getInstance()->teamA;
	int left ,right;
	left = right = -1;
	for(int i = 0;i < 4;i ++)
	{
		if(i == getBallID) continue;
		if(i == team->getControlID()) continue;
		if(left == -1) left = i;
		if(right == -1) right = i;
		int leftX = team->Player[left]->getPositionX() + team->getPositionX();
		int rightX = team->Player[right]->getPositionX() + team->getPositionX();
		int nowX = team->Player[i]->getPositionX() + team->getPositionX();
		if(nowX < leftX) left = i;
		if(nowX > rightX) right = i;
	}
	int mid = -1;
	for(int i = 0;i < 4 ;i ++){
		if(i == getBallID) continue;
		if(i == team->getControlID()) continue;
		if(i == left || i == right) continue;
		mid = i;
	}
	Point footballPos = GameWorld::getInstance()->football->getPosition();
	//left
	if(distTest(left)) array_dir[left] = hello_LEFT;
	else{
		if(rest[left] == -1){
			dir[left] = hello_LEFT;
			rest[left] = lastTime;
		}
		if(rest[left] == 0){
			if(dir[left] == hello_UPRIGHT) dir[left] = hello_DOWNRIGHT;
			else dir[left] = hello_UPRIGHT;
			rest[left] = lastTime;
		}
	//	CCLOG("rest:%d \n",rest[left]);
		array_dir[left] = dir[left];
		rest[left] --;
	}

	//right 
	if(distTest(right)) {
		array_dir[right] = hello_RIGHT;
	}
	else{
		if(rest[right] == -1){
			dir[right] = hello_RIGHT;
			rest[right] = lastTime;
		}
		if(rest[right] == 0){
			if(dir[right] == hello_UPLEFT) dir[right] = hello_DOWNLEFT;
			else dir[right] = hello_UPLEFT;
			rest[right] = lastTime;
		}
		rest[right] --;
		array_dir[right] = dir[right];
	}

	//mid
	if(mid != -1 && distTest(mid) ) array_dir[mid] = hello_RIGHT;
	else if(mid != -1)
	{
		if(rest[mid] == -1){
			dir[mid] = hello_RIGHT;
			rest[mid] = lastTime;
		}
		if(rest[mid] == 0){
			if(dir[mid] == hello_DOWNLEFT) dir[mid] = hello_UPLEFT;
			else dir[mid] = hello_DOWNLEFT;
			rest[mid] = lastTime;
		}
		rest[mid] --;
		array_dir[mid] = dir[mid];
	}
}

void AI::cut()
{
	CCLOG("I'm here !!!!!!!");
	for(int i = 0;i < 4;i ++)
	{
		if(howLong[i]) howLong[i] --;
	}
}

bool AI::distTest(int id)
{
	Point footballPos = GameWorld::getInstance()->football->getPosition();
	Point playerPos = GameWorld::getInstance()->teamA->Player[id]->getPosition() + GameWorld::getInstance()->teamA->getPosition();
	if(playerPos.x < 1000) return false;
	if(playerPos.x > 3500) return false;
	if(fabs(playerPos.x - footballPos.x) < 500) {
		rest[id] = -1;
		return true;
	}
	else return false;
}

int AI::contronl(int id )
{
	CCLOG("howlong:%d\n",howLong[id]);
	if(howLong[id] != 0)
	{
		return last[id];
	}
	Point playerPos = GameWorld::getInstance()->teamA->Player[id]->getPosition() + GameWorld::getInstance()->teamA->getPosition();
	Point ballPos = GameWorld::getInstance()->football->getPosition();
	Vec2 distVec = playerPos - ballPos;
	float dist = sqrt(distVec.x * distVec.x + distVec.y * distVec.y);
	//计算球在人的方向
	int degree = calDegree(distVec);
	int direction = whereBall(degree);
	int bound = RandomHelper::random_int(0 ,200);
	if(dist > 700 + bound || playerPos.x < 504 || playerPos.x > 4080 || playerPos.y < 119 || playerPos.y > 2349) player[id] = false;
	if(dist < 200) player[id] = true;
	//远离
	if(player[id]){
		int ret = go(id  ,direction);
		return ret;
	}
	//靠近
	else{
		int ret = come(id  ,direction);
		return ret;
	}
}

int AI::go(int id  ,int direction)
{
	int a[3];
	for(int i = -1;i <= 1;i ++)
		a[i + 1] = ((direction + i) % 8);
	int val = RandomHelper::random_int(0 ,2);
	last[id] = a[val];
	howLong[id] = 30;
	CCLOG("go: start:%d  end:%d\n",direction ,a[val]);
	return a[val];
}

int AI::come(int id  ,int direction)
{
	int a[3];
	for(int i = 3;i <= 5;i ++)
		a[i - 3] = ((direction + i) % 8);
	int val = RandomHelper::random_int(0 ,2);
	last[id] = a[val];
	howLong[id] = 30;
	CCLOG("come: start:%d  end:%d\n",direction ,a[val]);
	return a[val];
}

int AI::whereBall(float angle){
	if(angle > -22.5f && angle < 22.5f) return hello_RIGHT;
	else if(angle > 22.5f && angle < 67.5f) return hello_UPRIGHT;
	else if(angle > 67.5f && angle < 112.5f) return hello_UP;
	else if(angle > 112.5f && angle < 157.5f) return hello_UPLEFT;
	else if((angle > 157.5f && angle < 1000.0f )|| angle < -157.5f) return hello_LEFT;
	else if(angle > -157.5f && angle < -112.5f) return hello_DOWNLEFT;
	else if(angle > -112.5f && angle < -67.5f) return hello_DOWN;
	else if(angle > -67.5f && angle < -22.5f) return hello_DOWNRIGHT;
}