#include "qianghuaScene.h"
#include "cocos2d.h"
#include "xueSprite.h"
#include "SelectScene.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "client_sdk/network_tool_win.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "client_sdk/network_tool.h"
#endif

#include "client_sdk/data_def.h"
#include "GameWorld.h"

char skillName[3][10] = {"Pace", "Strength", "Shoot"};

void qianghuaScene::changeSkill(int add) {
	this->skill_point = add;
	char display[10];
	sprintf(display, "%d", this->skill_point);
	this->left->setString(display);
}

void qianghuaScene::sendMsg() {
	auto instance = NetworkTool::get_instance();

	auto send = new UserData();
	send->potency = this->skill_point;
	send->user_name = GameWorld::getInstance()->id;
	FootballerData temp;
	for(int i = 0 ;i < 4; i++) {
		temp.speed = dynamic_cast<xueSprite *>(this->getChildByTag(i * 3) )->abi;
		temp.strength = dynamic_cast<xueSprite *>(this->getChildByTag(i * 3 + 1))->abi;
		temp.accuracy = dynamic_cast<xueSprite *>(this->getChildByTag(i * 3 + 2) )->abi;
		send->player[i] = temp;
	}
	CCLOG("send");
	NetworkTool::get_instance()->push_user_data(send);
}

bool qianghuaScene::init() {
	cocos2d::Sprite * sprite;
	char name[100];

	auto data = new UserData();
	NetworkTool::get_instance()->get_user_data(GameWorld::getInstance()->id, data);

	for(int i = 0 ;i < 4; i++) {
		sprintf(name, "Owner_Character_Level0%d_mip_0.png", i + 1);
		CCLOG(name);
		sprite = cocos2d::Sprite::create(name);
		this->addChild(sprite);
		sprite->setPosition(450 * i + 400, 850);

		for(int f = 0; f < 3; f++) {
			auto xue = xueSprite::create();
			if(0 == f)
				xue->fuzhi(i, f, data->player[i].speed);
			else if(1 == f)
				xue->fuzhi(i, f, data->player[i].strength);
			else
				xue->fuzhi(i, f, data->player[i].accuracy);
			
			xue->setPosition(450 * i + 300, 320 + 100 * f);
			this->addChild(xue, 0, i * 3 + f);
		}
	}

	for(int i = 0 ;i < 3; i++) {
		auto temp = cocos2d::LabelTTF::create(skillName[i], "DoodleJumpBold_v2.ttf", 60);
		this->addChild(temp);
		temp->setPosition(100, 320 + 100 * i);
	}

	auto str = cocos2d::LabelTTF::create("left    skill point", "DoodleJumpBold_v2.ttf", 90);
	this->left = cocos2d::LabelTTF::create("5", "DoodleJumpBold_v2.ttf", 90);
	this->addChild(str);
	str->setPosition(500, 150);
	this->addChild(left);
	left->setPosition(430, 150);

	auto confirmItem = MenuItemImage::create(
                                           "queren1.png",
                                           "queren2.png",
                                           CC_CALLBACK_1(qianghuaScene::confirmCallback, this));
	confirmItem->setPosition(1500, 160);

	auto cancel = MenuItemImage::create(
                                           "fanhui1.png",
                                           "fanhui1.png",
										   [&](cocos2d::Ref* sender) {
		auto scene = SelectScene::createScene();
		cocos2d::Director::getInstance()->replaceScene(scene);
	});
	cancel->setPosition(1000, 160);

	auto jia0 = cocos2d::MenuItemImage::create("map.png", "map.png", CC_CALLBACK_1(qianghuaScene::jia0Callback, this));
	jia0->setPosition(520, 320);

	auto jian0 = cocos2d::MenuItemImage::create("reduce.png", "reduce.png", CC_CALLBACK_1(qianghuaScene::jian0Callback, this));
	jian0->setPosition(180, 320);

	auto jia1 = cocos2d::MenuItemImage::create("map.png", "map.png", CC_CALLBACK_1(qianghuaScene::jia1Callback, this));
	jia1->setPosition(520, 420);

	auto jian1 = cocos2d::MenuItemImage::create("reduce.png", "reduce.png", CC_CALLBACK_1(qianghuaScene::jian1Callback, this));
	jian1->setPosition(180, 420);

	auto jia2 = cocos2d::MenuItemImage::create("map.png", "map.png", CC_CALLBACK_1(qianghuaScene::jia2Callback, this));
	jia2->setPosition(520, 520);

	auto jian2 = cocos2d::MenuItemImage::create("reduce.png", "reduce.png", CC_CALLBACK_1(qianghuaScene::jian2Callback, this));
	jian2->setPosition(180, 520);

	auto jia3 = cocos2d::MenuItemImage::create("map.png", "map.png", CC_CALLBACK_1(qianghuaScene::jia3Callback, this));
	jia3->setPosition(970, 320);

	auto jian3 = cocos2d::MenuItemImage::create("reduce.png", "reduce.png", CC_CALLBACK_1(qianghuaScene::jian3Callback, this));
	jian3->setPosition(630, 320);

	auto jia4 = cocos2d::MenuItemImage::create("map.png", "map.png", CC_CALLBACK_1(qianghuaScene::jia4Callback, this));
	jia4->setPosition(970, 420);

	auto jian4 = cocos2d::MenuItemImage::create("reduce.png", "reduce.png", CC_CALLBACK_1(qianghuaScene::jian4Callback, this));
	jian4->setPosition(630, 420);

	auto jia5 = cocos2d::MenuItemImage::create("map.png", "map.png", CC_CALLBACK_1(qianghuaScene::jia5Callback, this));
	jia5->setPosition(970, 520);

	auto jian5 = cocos2d::MenuItemImage::create("reduce.png", "reduce.png", CC_CALLBACK_1(qianghuaScene::jian5Callback, this));
	jian5->setPosition(630, 520);

	auto jia6 = cocos2d::MenuItemImage::create("map.png", "map.png", CC_CALLBACK_1(qianghuaScene::jia6Callback, this));
	jia6->setPosition(1420, 320);

	auto jian6 = cocos2d::MenuItemImage::create("reduce.png", "reduce.png", CC_CALLBACK_1(qianghuaScene::jian6Callback, this));
	jian6->setPosition(1080, 320);

	auto jia7 = cocos2d::MenuItemImage::create("map.png", "map.png", CC_CALLBACK_1(qianghuaScene::jia7Callback, this));
	jia7->setPosition(1420, 420);

	auto jian7 = cocos2d::MenuItemImage::create("reduce.png", "reduce.png", CC_CALLBACK_1(qianghuaScene::jian7Callback, this));
	jian7->setPosition(1080, 420);

	auto jia8 = cocos2d::MenuItemImage::create("map.png", "map.png", CC_CALLBACK_1(qianghuaScene::jia8Callback, this));
	jia8->setPosition(1420, 520);

	auto jian8 = cocos2d::MenuItemImage::create("reduce.png", "reduce.png", CC_CALLBACK_1(qianghuaScene::jian8Callback, this));
	jian8->setPosition(1080, 520);

	auto jia9 = cocos2d::MenuItemImage::create("map.png", "map.png", CC_CALLBACK_1(qianghuaScene::jia9Callback, this));
	jia9->setPosition(1870, 320);

	auto jian9 = cocos2d::MenuItemImage::create("reduce.png", "reduce.png", CC_CALLBACK_1(qianghuaScene::jian9Callback, this));
	jian9->setPosition(1530, 320);

	auto jia10 = cocos2d::MenuItemImage::create("map.png", "map.png", CC_CALLBACK_1(qianghuaScene::jia10Callback, this));
	jia10->setPosition(1870, 420);

	auto jian10 = cocos2d::MenuItemImage::create("reduce.png", "reduce.png", CC_CALLBACK_1(qianghuaScene::jian10Callback, this));
	jian10->setPosition(1530, 420);

	auto jia11 = cocos2d::MenuItemImage::create("map.png", "map.png", CC_CALLBACK_1(qianghuaScene::jia11Callback, this));
	jia11->setPosition(1870, 520);

	auto jian11 = cocos2d::MenuItemImage::create("reduce.png", "reduce.png", CC_CALLBACK_1(qianghuaScene::jian11Callback, this));
	jian11->setPosition(1530, 520);

	auto menu = cocos2d::Menu::create(jia0, confirmItem, jian0, jia1, jian1, jia2, jian2, jia3, jian3, jia4, jian4, jia5, jian5, jia6, jian6,\
		jia7, jian7, jia8, jian8, jia9, jian9, jia10, jian10, jia11, jian11, cancel, NULL);
	menu->setPosition(cocos2d::Vec2::ZERO);
	this->addChild(menu);

	return true;
}

void qianghuaScene::jia0Callback(cocos2d::Ref* pSender) {
	if(0 == this->skill_point)
		return ;
	int old = (dynamic_cast<xueSprite *>(this->getChildByTag(0)) )->abi;
	if(100 == old)
		return ;
	dynamic_cast<xueSprite *>(this->getChildByTag(0))->updateXue(old + 1);
	this->changeSkill(this->skill_point - 1);
}

void qianghuaScene::jian0Callback(cocos2d::Ref* pSender) {
	int old = (dynamic_cast<xueSprite *>(this->getChildByTag(0)) )->abi;
	if(0 == old)
		return ;
	dynamic_cast<xueSprite *>(this->getChildByTag(0))->updateXue(old - 1);
	this->changeSkill(this->skill_point + 1);
}
void qianghuaScene::jia1Callback(cocos2d::Ref* pSender) {
	if(0 == this->skill_point)
		return ;
	int old = (dynamic_cast<xueSprite *>(this->getChildByTag(1)) )->abi;
	if(100 == old)
		return ;
	dynamic_cast<xueSprite *>(this->getChildByTag(1))->updateXue(old + 1);
	this->changeSkill(this->skill_point - 1);
}

void qianghuaScene::jian1Callback(cocos2d::Ref* pSender) {
	int old = (dynamic_cast<xueSprite *>(this->getChildByTag(1)) )->abi;
	if(0 == old)
		return ;
	dynamic_cast<xueSprite *>(this->getChildByTag(1))->updateXue(old - 1);
	this->changeSkill(this->skill_point + 1);
}
void qianghuaScene::jia2Callback(cocos2d::Ref* pSender) {
	if(0 == this->skill_point)
		return ;
	int old = (dynamic_cast<xueSprite *>(this->getChildByTag(2)) )->abi;
	if(100 == old)
		return ;
	dynamic_cast<xueSprite *>(this->getChildByTag(2))->updateXue(old + 1);
	this->changeSkill(this->skill_point - 1);
}

void qianghuaScene::jian2Callback(cocos2d::Ref* pSender) {
	int old = (dynamic_cast<xueSprite *>(this->getChildByTag(2)) )->abi;
	if(0 == old)
		return ;
	dynamic_cast<xueSprite *>(this->getChildByTag(2))->updateXue(old - 1);
	this->changeSkill(this->skill_point + 1);
}
void qianghuaScene::jia3Callback(cocos2d::Ref* pSender) {
	if(0 == this->skill_point)
		return ;
	int old = (dynamic_cast<xueSprite *>(this->getChildByTag(3)) )->abi;
	if(100 == old)
		return ;
	dynamic_cast<xueSprite *>(this->getChildByTag(3))->updateXue(old + 1);
	this->changeSkill(this->skill_point - 1);
}

void qianghuaScene::jian3Callback(cocos2d::Ref* pSender) {
	int old = (dynamic_cast<xueSprite *>(this->getChildByTag(3)) )->abi;
	if(0 == old)
		return ;
	dynamic_cast<xueSprite *>(this->getChildByTag(3))->updateXue(old - 1);
	this->changeSkill(this->skill_point + 1);
}
void qianghuaScene::jia4Callback(cocos2d::Ref* pSender) {
	if(0 == this->skill_point)
		return ;
	int old = (dynamic_cast<xueSprite *>(this->getChildByTag(4)) )->abi;
	if(100 == old)
		return ;
	dynamic_cast<xueSprite *>(this->getChildByTag(4))->updateXue(old + 1);
	this->changeSkill(this->skill_point - 1);
}

void qianghuaScene::jian4Callback(cocos2d::Ref* pSender) {
	int old = (dynamic_cast<xueSprite *>(this->getChildByTag(4)) )->abi;
	if(0 == old)
		return ;
	dynamic_cast<xueSprite *>(this->getChildByTag(4))->updateXue(old - 1);
	this->changeSkill(this->skill_point + 1);
}
void qianghuaScene::jia5Callback(cocos2d::Ref* pSender) {
	if(0 == this->skill_point)
		return ;
	int old = (dynamic_cast<xueSprite *>(this->getChildByTag(5)) )->abi;
	if(100 == old)
		return ;
	dynamic_cast<xueSprite *>(this->getChildByTag(5))->updateXue(old + 1);
	this->changeSkill(this->skill_point - 1);
}

void qianghuaScene::jian5Callback(cocos2d::Ref* pSender) {
	int old = (dynamic_cast<xueSprite *>(this->getChildByTag(5)) )->abi;
	if(0 == old)
		return ;
	dynamic_cast<xueSprite *>(this->getChildByTag(5))->updateXue(old - 1);
	this->changeSkill(this->skill_point + 1);
}
void qianghuaScene::jia6Callback(cocos2d::Ref* pSender) {
	if(0 == this->skill_point)
		return ;
	int old = (dynamic_cast<xueSprite *>(this->getChildByTag(6)) )->abi;
	if(100 == old)
		return ;
	dynamic_cast<xueSprite *>(this->getChildByTag(6))->updateXue(old + 1);
	this->changeSkill(this->skill_point - 1);
}

void qianghuaScene::jian6Callback(cocos2d::Ref* pSender) {
	int old = (dynamic_cast<xueSprite *>(this->getChildByTag(6)) )->abi;
	if(0 == old)
		return ;
	dynamic_cast<xueSprite *>(this->getChildByTag(6))->updateXue(old - 1);
	this->changeSkill(this->skill_point + 1);
}
void qianghuaScene::jia7Callback(cocos2d::Ref* pSender) {
	if(0 == this->skill_point)
		return ;
	int old = (dynamic_cast<xueSprite *>(this->getChildByTag(7)) )->abi;
	if(100 == old)
		return ;
	dynamic_cast<xueSprite *>(this->getChildByTag(7))->updateXue(old + 1);
	this->changeSkill(this->skill_point - 1);
}

void qianghuaScene::jian7Callback(cocos2d::Ref* pSender) {
	int old = (dynamic_cast<xueSprite *>(this->getChildByTag(7)) )->abi;
	if(0 == old)
		return ;
	dynamic_cast<xueSprite *>(this->getChildByTag(7))->updateXue(old - 1);
	this->changeSkill(this->skill_point + 1);
}
void qianghuaScene::jia8Callback(cocos2d::Ref* pSender) {
	if(0 == this->skill_point)
		return ;
	int old = (dynamic_cast<xueSprite *>(this->getChildByTag(8)) )->abi;
	if(100 == old)
		return ;
	dynamic_cast<xueSprite *>(this->getChildByTag(8))->updateXue(old + 1);
	this->changeSkill(this->skill_point - 1);
}

void qianghuaScene::jian8Callback(cocos2d::Ref* pSender) {
	int old = (dynamic_cast<xueSprite *>(this->getChildByTag(8)) )->abi;
	if(0 == old)
		return ;
	dynamic_cast<xueSprite *>(this->getChildByTag(8))->updateXue(old - 1);
	this->changeSkill(this->skill_point + 1);
}
void qianghuaScene::jia9Callback(cocos2d::Ref* pSender) {
	if(0 == this->skill_point)
		return ;
	int old = (dynamic_cast<xueSprite *>(this->getChildByTag(9)) )->abi;
	if(100 == old)
		return ;
	dynamic_cast<xueSprite *>(this->getChildByTag(9))->updateXue(old + 1);
	this->changeSkill(this->skill_point - 1);
}

void qianghuaScene::jian9Callback(cocos2d::Ref* pSender) {
	int old = (dynamic_cast<xueSprite *>(this->getChildByTag(9)) )->abi;
	if(0 == old)
		return ;
	dynamic_cast<xueSprite *>(this->getChildByTag(9))->updateXue(old - 1);
	this->changeSkill(this->skill_point + 1);
}

void qianghuaScene::jia10Callback(cocos2d::Ref* pSender) {
	if(0 == this->skill_point)
		return ;
	int old = (dynamic_cast<xueSprite *>(this->getChildByTag(10)) )->abi;
	if(100 == old)
		return ;
	dynamic_cast<xueSprite *>(this->getChildByTag(10))->updateXue(old + 1);
	this->changeSkill(this->skill_point - 1);
}

void qianghuaScene::jian10Callback(cocos2d::Ref* pSender) {
	int old = (dynamic_cast<xueSprite *>(this->getChildByTag(10)) )->abi;
	if(0 == old)
		return ;
	dynamic_cast<xueSprite *>(this->getChildByTag(10))->updateXue(old - 1);
	this->changeSkill(this->skill_point + 1);
}
void qianghuaScene::jia11Callback(cocos2d::Ref* pSender) {
	if(0 == this->skill_point)
		return ;
	int old = (dynamic_cast<xueSprite *>(this->getChildByTag(11)) )->abi;
	if(100 == old)
		return ;
	dynamic_cast<xueSprite *>(this->getChildByTag(11))->updateXue(old + 1);
	this->changeSkill(this->skill_point - 1);
}

void qianghuaScene::jian11Callback(cocos2d::Ref* pSender) {
	int old = (dynamic_cast<xueSprite *>(this->getChildByTag(0)) )->abi;
	if(0 == old)
		return ;
	dynamic_cast<xueSprite *>(this->getChildByTag(0))->updateXue(old - 1);
	this->changeSkill(this->skill_point + 1);
}

void qianghuaScene::confirmCallback(cocos2d::Ref* pSender) {
	CCLOG("diaoyong ");
	this->sendMsg();
	auto sprite = cocos2d::Sprite::create("login.png");
	sprite->setPosition(1000, 700);
	this->addChild(sprite);

	auto qhz = cocos2d::Sprite::create("qhz.png");
	qhz->setPosition(940, 700);
	this->addChild(qhz);

	this->scheduleOnce(schedule_selector(qianghuaScene::displayqhz), 2.0f);
}

void qianghuaScene::displayqhz(float dt) {
	auto scene = SelectScene::createScene();
	cocos2d::Director::getInstance()->replaceScene(scene);
}


cocos2d::Scene* qianghuaScene::createScene(int add)
{
    // 'scene' is an autorelease object
    auto scene = cocos2d::Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = qianghuaScene::create();
	layer->changeSkill(add);

    // add layer as a child to scene
    scene->addChild(layer);
    return scene;
}