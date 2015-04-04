#include "GuideScene.h"
#include "cocos2d.h"
#include "SelectScene.h"

bool GuideScene::init() {
	this->now = 0;
	this->endPage = 8;

	auto dialog = cocos2d::Sprite::create("dialog.png");
	this->addChild(dialog);
	dialog->setPosition(950, 100);
	auto human = cocos2d::Sprite::create("human.png");
	this->addChild(human);
	human->setPosition(350, 260);

	this->ttf = cocos2d::LabelTTF::create("", "DoodleJumpBold_v2.ttf", 60);
	//this->ttf->setAnchorPoint(cocos2d::Vec2(0, 300));
	this->ttf->setDimensions(cocos2d::Size(1200, 0));
	this->ttf->setHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
	ttf->setPosition(1060, 230);
	this->addChild(ttf);

	this->changeNow();

	this->setTouchEnabled(true);
	this->setTouchMode(cocos2d::Touch::DispatchMode::ONE_BY_ONE);
	return true;
}

cocos2d::Scene * GuideScene::createScene() {
	// 'scene' is an autorelease object
	auto scene = cocos2d::Scene::create();

	// 'layer' is an autorelease object
	auto layer = GuideScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

void GuideScene::changeNow() {
	cocos2d::CCDictionary *strings = cocos2d::CCDictionary::createWithContentsOfFile("string.xml");

	char display[10]; 
	sprintf(display, "%d", this->now);
	const char *context = ((cocos2d::CCString*)strings->objectForKey(display))->getCString();
	this->ttf->setString(context);

	if(0 != this->now) {
		this->removeChildByTag(233);
	}

	sprintf(display ,"%d.jpg", this->now);
	auto sprite = cocos2d::Sprite::create(display);
	this->addChild(sprite, -1, 233);
	sprite->setPosition(950, 700);
}

bool GuideScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *event) { 
	CCLOG("Touch in Guide");
	this->now ++;
	if(this->now == this->endPage) {
		auto scene = SelectScene::createScene();
		cocos2d::Director::getInstance()->replaceScene(scene);
		return true;
	}
	this->changeNow();
	return true;
}