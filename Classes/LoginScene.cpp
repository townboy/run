#include "cocos2d.h"
#include "LoginScene.h"
#include "SelectScene.h"
#include "GreenSward.h"
#include "GameWorld.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "client_sdk/network_tool_win.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "client_sdk/network_tool.h"
#endif

LoginScene::LoginScene() {
	network_handle = NetworkTool::get_instance(); 
	// remind delete
}

bool LoginScene::init() {
	auto tishi = cocos2d::Sprite::create("tishi.png");

	auto fadeOut = cocos2d::FadeOut::create(3.0f);
	auto fadeIn = cocos2d::FadeIn::create(3.0f);
	this->addChild(tishi);
	tishi->setPosition(1000, 500);
	tishi->setScale(2.5f);
	auto remove = cocos2d::RemoveSelf::create();

	auto addId = CallFunc::create([&]() {
		this->id = TextFieldTTF::textFieldWithPlaceHolder("", "Arial", 100);
		id->setColor(cocos2d::Color3B(0xff, 0xff, 0xff));
		this->addChild(id);
		id->setPosition(1000, 200);
		id->attachWithIME();
		this->setTouchEnabled(true);
		this->setTouchMode(cocos2d::Touch::DispatchMode::ONE_BY_ONE);
	});
	tishi->runAction(cocos2d::Sequence::create(remove, addId, NULL));
	//tishi->runAction(cocos2d::Sequence::create(fadeOut, fadeIn, fadeOut, remove, addId, NULL));
	return true;
}

bool LoginScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *event) {
	auto name = this->id->getString();
	CCLOG(name.c_str());
	int ret = network_handle->connect_server();
	if(ret != 0) {
		CCLOG("connect error!\n");
		return true;
	}

	network_handle->login(name, &user_data);
	GameWorld::getInstance()->id = name;
	CCLOG("user_name %s", user_data.user_name.c_str());
	CCLOG("potency %d", user_data.potency);
	for(int i = 0; i < 4; i++) {
		CCLOG("%d speed %d", i+1, user_data.player[i].speed);
		CCLOG("%d strength %d", i+1, user_data.player[i].strength);
	}

	auto remove = cocos2d::RemoveSelf::create();
	auto select = SelectScene::createScene();
	Director::getInstance()->replaceScene(select);

	return true;
}

cocos2d::Scene* LoginScene::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = LoginScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}
