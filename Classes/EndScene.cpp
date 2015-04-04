#include <algorithm>
#include "SimpleAudioEngine.h"
#include <vector>
#include "GameWorld.h"
#include "EndScene.h"
#include "SelectScene.h"

// This is a Scene

Scene* EndScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = EndScene::create();

    // add layer as a child to scene
    scene->addChild(layer);


    // return the scene

    return scene;
}

bool EndScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    //add the backGround
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	if(GameWorld::getInstance()->teamA->getScore() >= GameWorld::getInstance()->teamB->getScore()){
		auto logo = Sprite::create("lose.png");
		logo->setPosition(visibleSize.width / 2  - 300,visibleSize.height / 2);
		this->addChild(logo);
		auto label = Label::createWithTTF("YOU LOSE.....", "fonts/MyFont.ttf", 100);
		label->setPosition(visibleSize.width / 2 + 100, visibleSize.height / 2);
		auto left = MoveBy::create(2.0f ,Vec2(10.0f ,20.0f));
		auto leftSim = Spawn::create(left ,TintTo::create(2.0f ,255 ,255 ,0) ,NULL);
		auto right = MoveBy::create(2.0f ,Vec2(-10.0f ,-20.0f));
		auto rightSim = Spawn::create(right ,TintTo::create(2.0f ,0 ,255 ,255) ,NULL);
		auto seq = Sequence::create(leftSim ,rightSim ,NULL);
		label->runAction(RepeatForever::create(seq));
		this->addChild(label);
	}
	else{
		auto logo = Sprite::create("win.png");
		logo->setPosition(visibleSize.width / 2  - 300,visibleSize.height / 2);
		this->addChild(logo);
		auto label = Label::createWithTTF("YOU WIN!!!!!!!!!!!", "fonts/MyFont.ttf", 100);
		label->setPosition(visibleSize.width / 2 + 100, visibleSize.height / 2);
		auto left = MoveBy::create(2.0f ,Vec2(10.0f ,20.0f));
		auto leftSim = Spawn::create(left ,TintTo::create(2.0f ,255 ,255 ,0) ,NULL);
		auto right = MoveBy::create(2.0f ,Vec2(-10.0f ,-20.0f));
		auto rightSim = Spawn::create(right ,TintTo::create(2.0f ,0 ,255 ,255) ,NULL);
		auto seq = Sequence::create(leftSim ,rightSim ,NULL);
		label->runAction(RepeatForever::create(seq));
		this->addChild(label);
	}

	// add back button

	auto sprite = cocos2d::MenuItemImage::create("fanhui1.png", "fanhui1.png", CC_CALLBACK_1(EndScene::back, this));
	sprite->setPosition(0, -280);
	auto menu = cocos2d::Menu::create(sprite, NULL);
	this->addChild(menu);

	return true;
}

void EndScene::back(cocos2d::Ref* pSender) {
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/foreverYoung.mp3" ,true);
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
	auto scene = SelectScene::createScene();
	cocos2d::Director::getInstance()->replaceScene(scene);
}
