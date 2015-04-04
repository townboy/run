#include "cocos2d.h"
#include "iostream"
#include "SelectScene.h"
#include "GameScene.h"
#include "GameWorld.h"
#include "TrainScene.h"
#include "GuideScene.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "client_sdk/network_tool_win.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "client_sdk/network_tool.h"
#endif

#include "client_sdk/data_def.h"

SelectScene::SelectScene() {
}

void SelectScene::enterGame(float dt) {
	NetworkTool * p = NetworkTool::get_instance();

	UserData data;
	p->get_user_data(GameWorld::getInstance()->id, &data);
	GameWorld::getInstance()->A = data;

	p->start_game_req(GameWorld::getInstance()->id);
	CCLOG("matching !");
	std::string postion;
	p->start_game_resp(&postion ,&data);
	GameWorld::getInstance()->B = data;

	//Ë³Ğò²»ÄÜÂÒ
	GameWorld::getInstance()->setInitPostion(postion);
	auto scene = GameScene::createScene();
	
	scene->addChild(GameWorld::getInstance());
	Director::getInstance()->replaceScene(scene);
}

bool SelectScene::init() {
	auto bg = cocos2d::Sprite::create("001.jpg");
	this->addChild(bg);
	bg->setPosition(950, 550);

	auto guide = cocos2d::MenuItemImage::create("xinshou1.png", "xinshou2.png", [&](Ref* sender)
                     { CCLOG("Label Callback story");
		auto scene = GuideScene::createScene();
		
		Director::getInstance()->replaceScene(scene);
		});
	guide->setPosition(970, 750);

	auto match = cocos2d::MenuItemImage::create("pipei1.png", "pipei2.png", [&](Ref* sender)
                     { CCLOG("Label Callback match");
		
		auto wait = cocos2d::Sprite::create("login.png");
		wait->setPosition(1020, 600);
		this->addChild(wait, 20);
		auto zhengzai = cocos2d::Sprite::create("zhengzai.png");
		zhengzai->setPosition(950, 600);
		this->addChild(zhengzai, 21);

		this->scheduleOnce(schedule_selector(SelectScene::enterGame), 1.0f);
	} );

	match->setPosition(970, 610);
	auto training = cocos2d::MenuItemImage::create("xunlian1.png", "xunlian2.png", [&](Ref* sender)
                     { CCLOG("Label Callback training");
	auto scene = TrainScene::createScene();
	Director::getInstance()->replaceScene(scene);
	}
                     );
	training->setPosition(970, 470);
    auto menu = cocos2d::Menu::create(guide, match, training, NULL);
	menu->setPosition(0, 0);
	this->addChild(menu, 10);

	auto sprite = cocos2d::Sprite::create("select.png");
	this->addChild(sprite);
	sprite->setPosition(1000, 700);

	return true;
}

cocos2d::Scene* SelectScene::createScene() {
    // 'scene' is an autorelease object

	GameWorld::destroyInstance();
    auto scene = cocos2d::Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SelectScene::create();


    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene

    return scene;
}
