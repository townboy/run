#include "GameWorld.h"
#include "GameScene.h"
#include "EndScene.h"
USING_NS_CC;

GameWorld* GameWorld::sharedGameWorld = nullptr;
int GameWorld::counts = 0;

GameWorld * GameWorld::getInstance()
{
	if(sharedGameWorld == NULL){
		sharedGameWorld = new GameWorld();
		if(!sharedGameWorld->init()){
			delete sharedGameWorld;
			sharedGameWorld = NULL;
		}
	}
	return sharedGameWorld;
}

void GameWorld::destroyInstance() {
	CC_SAFE_DELETE(sharedGameWorld);
	sharedGameWorld = NULL;
}

void GameWorld::resetWorld()
{
	aiCount = 120;
	ai->ONOFF = false;
	if(strcmp(GameWorld::getInstance()->getInitPostion().c_str(),"left") == 0) {
		teamA->reset(0 ,4);
		teamB->reset(4 ,8);
	} else {
		teamA->reset(4 ,8);
		teamB->reset(0 ,4);
	}
	Point target = Point(4576.0f  / 2 ,2835.0f / 2 - 150);
	football->runAction(MoveTo::create(3.0f ,target));
}

void GameWorld::afterBallOut()
{
	Athlete * who = GameWorld::getInstance()->football->getControner();
	if(who == NULL) who = GameWorld::getInstance()->football->getLastControner();
}

GameWorld::GameWorld(){
	network_handle = NetworkTool::get_instance();

	frame_num = 0;
	passBall = 0;
	uplo_data.status = 'r';
	auto sprite = cocos2d::MenuItemImage::create("renshu.png", "renshu.png", CC_CALLBACK_1(GameWorld::back, this));
	sprite->setPosition(600,400);
	//sprite->setLocalZOrder(10);
	auto menu = cocos2d::Menu::create(sprite, NULL);
	this->addChild(menu,500);
}

void GameWorld::back(cocos2d::Ref* pSender) {
	uplo_data.status = 'e';
	CCLOG("this botton");
}

void GameWorld::update(float e) 
{
	if(ai->ONOFF == false){
		aiCount --;
		if(aiCount == 0) ai->ONOFF = true;
	}
	ai->cut();
	int a_id = teamA->getControlID();
	if(frame_num % 5 == 0 ) {
		
		int ret = network_handle->get_frame_sync_data(&sync_data);
		if(ret == -1 || sync_data.status == 'e'){
			if(-1 == ret)
				network_handle->connect_server();
			Director::getInstance()->replaceScene(EndScene::createScene());
			return ;
		}
		
		for(int i = 0 ; i< 4;i ++)
			uplo_data.control[i] = 8;
 		
		uplo_data.control[a_id] = *(this->joyStickDirection);
		//CCLOG("dir : %d\n",*(this->joyStickDirection));
		int ai_id = -1,ai_dir= -1;
		
		if(ai->ONOFF) ai->getBall(&ai_id, &ai_dir);
		if(ai_id != -1)
			uplo_data.control[ai_id] = ai_dir;
		for(int i = 0;i < 4;i ++)
		{
			if(!ai->ONOFF) continue;
			if(i == ai_id) continue;
			if(i == GameWorld::getInstance()->teamA->getControlID()) continue;
			uplo_data.control[i] = ai->contronl(i);
		}

		if(passBall == 1) {
			passBall = 0;
			uplo_data.a_pass = 1;
		} else
			uplo_data.a_pass = 0;
		if(shootBall == 1){
			Point target = footballTarget;
			int x = (int)target.x;
			int y = (int)target.y;
			shootBall = 0;
			uplo_data.tar_x = x;
			uplo_data.tar_y = y;
			uplo_data.a_shoot = 1;
		}
		else {
			uplo_data.a_shoot = 0;
			int x ,y;
			x = y = 0;
			uplo_data.tar_x = x;
			uplo_data.tar_y = y;
		}
		int ret1 = GameWorld::getInstance()->gameUI->game_ui_update();
		if(ret1 == -1) {
			uplo_data.status = 'e';
		}
		network_handle->push_frame_uplo_data(&uplo_data);

		for(int i = 0; i < 4; i++) { 
			teamA->contronlPlayer(i, sync_data.control[i]);
		}
		for(int i = 0; i < 4; i++) { 	
			teamB->contronlPlayer(i, sync_data.control[i+4]);
		}
	
		if(sync_data.a_pass) {
			//正在持球
			if(GameWorld::getInstance()->teamA->getControner() == GameWorld::getInstance()->football->getControner())
				game_scene->ApassBall();
			else game_scene->catchBall(GameWorld::getInstance()->teamA);
		}
		if(sync_data.b_pass) {
			//正在持球
			if(GameWorld::getInstance()->teamB->getControner() == GameWorld::getInstance()->football->getControner())
				game_scene->BpassBall();
			else game_scene->catchBall(GameWorld::getInstance()->teamB);
		}
		if(sync_data.a_shoot){
			//正在持球
			if(GameWorld::getInstance()->teamA->getControner() == GameWorld::getInstance()->football->getControner()){
				Point targetPos = Point(sync_data.a_tar_x ,sync_data.a_tar_y);
				game_scene->hitBallA(targetPos);
			}
			else{
				CCLOG("SPEED!!!!!!!!!");
				gameUI->speedUp(teamA);
			}
		}
		if(sync_data.b_shoot){
			if(GameWorld::getInstance()->teamB->getControner() == GameWorld::getInstance()->football->getControner()){
				Point targetPos = Point(sync_data.b_tar_x ,sync_data.b_tar_y);
				game_scene->hitBallB(targetPos);
			}
			else{
				gameUI->speedUp(teamB);
			}
		}
	}
	frame_num = (frame_num + 1) % 60;
}

bool GameWorld::init()
{
	football = NULL;
	teamA = NULL;
	teamB = NULL;
	ai = new AI();
	this->scheduleUpdate();
	return true;
}
std::string GameWorld::getInitPostion() {
	return postion;
}
void GameWorld::setScene(GameScene * tmp) {
	game_scene = tmp;
}
void GameWorld::setInitPostion(std::string pos) {
	postion = pos;
}
void GameWorld::setTeamAPoint(TeamSprite *team)
{
	teamA = team;
}

void GameWorld::setTeamBPoint(TeamSprite *team)
{
	teamB = team;
}

void GameWorld::setFootballPoint(Football *ball)
{
	football = ball;
}
void GameWorld::setStickJoyDirection(int *direction)
{
	joyStickDirection = direction;
}

Vec2 GameWorld::getFootballPostion()
{
	return football->getPosition();
}


void GameWorld::setGameUI(GAMEUI *UI)
{
	this->gameUI = UI;
}
