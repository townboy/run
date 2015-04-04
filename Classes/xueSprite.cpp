#include "xueSprite.h"
#include "cocos2d.h"


bool xueSprite::init() {
	if(!Sprite::init())
		return false;

	this->xue = cocos2d::Sprite::create("xue.png");
	this->xue->setAnchorPoint(cocos2d::Vec2::ZERO);
	this->addChild(xue);
	xue->setPosition(-130, -35);
	return true;
}

void xueSprite::fuzhi(int people, int skill, int abi) {
	this->people = people;
	this->skill = skill;
	this->abi = abi;

	char display[10];
	sprintf(display, "%d", abi);
	this->ttf = cocos2d::LabelTTF::create(display, "DoodleJumpBold_v2.ttf", 60);
	this->addChild(ttf);
	//ttf->setPosition(-150, 0);
	this->updateXue(abi);
}

void xueSprite::updateXue(int abi) {
	this->xue->setScaleX(abi / 100.0f);
	int length = 2.5f * abi;
	this->ttf->setPosition(length - 100, 0);
	char temp[10];
	sprintf(temp, "%d", abi);
	std::string tem = temp;
	this->ttf->setString(tem);
	this->abi = abi;
}