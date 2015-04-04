#include <algorithm>
#include <vector>
#include "BlackScene.h"

// This is a Scene

Scene* BlackScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = BlackScene::create();

    // add layer as a child to scene
    scene->addChild(layer);


    // return the scene

    return scene;
}

bool BlackScene::init()
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
	auto waitAction = DelayTime::create(1.0f);
	auto delAction = CallFunc::create(this ,callfunc_selector(BlackScene::fadeOut));
	this->runAction(Sequence::create(waitAction ,delAction ,NULL));
}

void BlackScene::fadeOut()
{
	Director::getInstance()->popScene();
}