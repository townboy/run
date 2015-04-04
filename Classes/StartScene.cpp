#include "StartScene.h"
#include "LoginScene.h"
#include "cocos2d.h"
#include "SelectScene.h"
#include "SimpleAudioEngine.h"

#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "client_sdk\network_tool_win.h"
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "client_sdk\network_tool.h"
#endif

#include "client_sdk\data_def.h"
#include "GameWorld.h"

Scene* StartScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = StartScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool StartScene::init()
{
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/foreverYoung.mp3" ,true);


	// add the bg image
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	backGround = Sprite::create("start.png");
	backGround->setPosition(visibleSize.width / 2 , visibleSize.height / 2);
	this->addChild(backGround);

	//add the frog
	Vector<SpriteFrame *> frames;
	char buff[255];
	for(int i = 1;i <= 3;i ++)
	{
		sprintf(buff ,"frog_%d.png" ,i);
		auto pic = Director::getInstance()->getTextureCache()->addImage(buff);
		auto frame = SpriteFrame::createWithTexture(pic,Rect(0,0,pic->getContentSize().width,pic->getContentSize().height));
		this->setPosition(Vec2(pic->getContentSize().width / 2 + 100,pic->getContentSize().height / 2));
		frames.pushBack(frame);
	}
	auto info = Animation::createWithSpriteFrames(frames ,1.0f / 5.0f ,1000);
	auto action = Animate::create(info);
	frog = Sprite::create();
	frog->setPosition(0, visibleSize.height / 5);
	auto toRight = MoveBy::create(3.0f ,Vec2(2000.0f ,0.0f));
	frog->runAction(Spawn::create(action ,toRight ,NULL));
	this->addChild(frog);

	//add the startLOGO
	//auto label = Label::createWithTTF("START", "fonts/Marker Felt.ttf", 100);
	auto label = Sprite::create("start_text.png");
	label->setPosition(visibleSize.width / 3 ,visibleSize.height / 20);
	//label->runAction(Blink::create(30.0f , 50));
	auto actionOut = FadeOut::create(3.0f);
	auto actionIn = FadeIn::create(3.0f);
	label->runAction(RepeatForever::create(Sequence::create(actionOut ,actionIn ,NULL)));
	this->addChild(label, 0, 0);

	this->setTouchEnabled(true);
	this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);

	return true;
}

void StartScene::addLogin() {


}

void StartScene::dengluCallback(cocos2d::Ref* pSender) {
	auto network_handle = NetworkTool::get_instance();
	auto name = this->id->getString();
	CCLOG(name.c_str());
	int ret = network_handle->connect_server();
	if(ret != 0) {
		CCLOG("connect error!\n");
		return ;
	}

	UserData user_data;
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
}

bool StartScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *event) {
	/*auto callfunc = CallFunc::create([]()
	{
		auto scene = LoginScene::createScene();
		Director::getInstance()->replaceScene(scene);
	});
	auto remove = RemoveSelf::create();

	auto seq = Sequence::create(remove, callfunc , NULL);
	this->runAction(seq);
	*/

	this->removeChildByTag(0);
	auto sprite = cocos2d::Sprite::create("login.png");
	this->addChild(sprite);
	sprite->setPosition(700, 200);


	this->id = TextFieldTTF::textFieldWithPlaceHolder("", "fonts/Marker Felt.ttf", 90);
	id->setColor(cocos2d::Color3B(0xff, 0xff, 0xff));
	this->addChild(id);
	id->setPosition(630, 290);
	id->attachWithIME();
/*	this->setTouchEnabled(true);
	this->setTouchMode(cocos2d::Touch::DispatchMode::ONE_BY_ONE);
	*/
	auto denglu = cocos2d::MenuItemImage::create("denglu1.png", "denglu2.png", CC_CALLBACK_1(StartScene::dengluCallback, this));
	auto menu = cocos2d::Menu::create(denglu, NULL);
	this->addChild(menu);
	menu->setPosition(650, 180);
	return true;
}
