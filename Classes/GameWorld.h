#ifndef __GAMEWORLD_SCENE_H__
#define __GAMEWORLD_SCENE_H__

#include "cocos2d.h"
#include "TeamSprite.h"
#include "football.h"
#include "GameUI.h"
#include "AI.h"
#include "string"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "client_sdk/network_tool_win.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "client_sdk/network_tool.h"
#endif

class GameScene;
class GameWorld : public cocos2d::Layer
{
public:
	static GameWorld * getInstance();
	static void destroyInstance();
	void update(float e);
	void setTeamAPoint(TeamSprite *);
	void setInitPostion(std::string pos);
	void setTeamBPoint(TeamSprite *);
	void setFootballPoint(Football *);
	void setStickJoyDirection(int *direction);
	void setGameUI(GAMEUI *);
	Vec2 getFootballPostion();
	std::string getInitPostion();
	void contronlTeam();
	void afterBallOut();
	void resetWorld();
	TeamSprite *teamA;
	TeamSprite *teamB;
	Football *football;
	GAMEUI *gameUI;
	int *joyStickDirection;
	AI *ai;
	std::string id;
	bool passBall;
	bool shootBall;
	Point footballTarget;
	void setScene(GameScene * tmp);
	UserData A;
	UserData B;
	int aiCount;
protected:
	GameWorld();
	virtual bool init();
	static GameWorld * sharedGameWorld;
private:
	static int counts ;
	NetworkTool *network_handle;
	int frame_num ;
	std::string postion;
	FrameSyncData sync_data;
	FrameUploData uplo_data;
	GameScene *game_scene;

	void back(cocos2d::Ref* pSender);
	
};
#endif // __HELLOWORLD_SCENE_H__