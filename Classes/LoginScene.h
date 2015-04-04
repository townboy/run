#ifndef _LOGINSCENE__H_
#define _LOGINSCENE__H_

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "client_sdk/network_tool.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "client_sdk/network_tool_win.h"
#endif

class LoginScene : public cocos2d::Layer {

public:
	static cocos2d::Scene* createScene();
	LoginScene();
	bool init();
	void addId();
    CREATE_FUNC(LoginScene);
	bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event);
	cocos2d::TextFieldTTF* id;

private:
	NetworkTool * network_handle;
	UserData user_data;
};

#endif
