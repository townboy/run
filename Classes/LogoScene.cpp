#include "LogoScene.h"
#include "ui/CocosGUI.h"
#include "StartScene.h"
#include "cocostudio\CocoStudio.h"
#include "VideoScene.h"

using namespace cocostudio::timeline;
USING_NS_CC;

Scene* LogoScene::createScene()
{
    auto scene = Scene::create();
    auto layer = LogoScene::create();
    scene->addChild(layer);
    return scene;
}

void LogoScene::update(float dt)
{
	rock_sprite->setPositionY(rock_sprite->getPositionY() + 2);
}

bool LogoScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	//add the logo
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto logo_png = Sprite::create("logo.png");
    logo_png->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	this->addChild(logo_png);

	//add the rock
	Vector<SpriteFrame *> frames;
	char buff[255];
	rock_sprite = Sprite::create();
	for(int i = 1;i <= 6;i ++)
	{
		sprintf(buff ,"dk_loading%d.png" ,i);
		//log(buff);
		auto pic = Director::getInstance()->getTextureCache()->addImage(buff);
		auto frame = SpriteFrame::createWithTexture(pic,Rect(0,0,pic->getContentSize().width,pic->getContentSize().height));
		rock_sprite->setPosition(Vec2(pic->getContentSize().width / 2 + 100,pic->getContentSize().height / 3));
		//frame = AtlasLoader::getInstance()->getSpriteFrameByName("bg_day");
		frames.pushBack(frame);
	}
	auto info = Animation::createWithSpriteFrames(frames ,1.0f / 600.f ,1000);
	auto action = Animate::create(info);
	rock_sprite->runAction(action);
	this->addChild(rock_sprite);

	//add the update
	scheduleUpdate();

	this->setTouchEnabled(true);
	this->setTouchMode(cocos2d::Touch::DispatchMode::ONE_BY_ONE);
	this->to_next();
    return true;
}

void LogoScene::to_next()
{
    auto callfunc = CallFunc::create([]()
    {
        auto scene = VideoScene::createScene();
        Director::getInstance()->replaceScene(scene);
    });
    auto remove = RemoveSelf::create();
	auto rotate = RotateTo::create(5 ,180);
	
    auto seq = Sequence::create(rotate, remove, callfunc, NULL);
    this->runAction(seq);
}

