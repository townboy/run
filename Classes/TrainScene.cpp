#include "cocos2d.h"
#include "TrainScene.h"
#include "string"
#include "SelectScene.h"
#include "qianghuaScene.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <android/log.h>
#endif

cocos2d::Scene* TrainScene::shared = nullptr;

TrainScene::TrainScene() {
}

int taskInt[5] = {0, 500, 5000, 50000, 5000000};

bool TrainScene::init() {
	auto bg = cocos2d::Sprite::create("1514_2560x1600xiu.png");
	bg->setPosition(700, 700);
	this->addChild(bg);

	auto already = cocos2d::MenuItemLabel::create(cocos2d::LabelTTF::create("you already run       J ", "DoodleJumpBold_v2.ttf", 90), [&](Ref* sender)
	{
	#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		cocos2d::JniMethodInfo info;
		bool ret = cocos2d::JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "Share", "()V");
		if(ret) {
			log("call void func1() success");
			info.env->CallStaticVoidMethod(info.classID, info.methodID);
		}
	#endif

	} );
	already->setPosition(500, 280);
	auto text = cocos2d::Menu::create(already, NULL);
	this->addChild(text);

	char task[5][100] = {"Wake Up Early", "500J", "5000J", "50000J", "5000000J"};

	for(int i = 0;i < 5; i++)
		this->isFinish[i] = false;


	for(int i = 0;i < 5; i++) {
		cocos2d::Sprite * sprite;
		auto ttf = cocos2d::LabelTTF::create(task[i], "DoodleJumpBold_v2.ttf", 100);
		if(true == this->isFinish[i])
			sprite = cocos2d::Sprite::create("checkbox_white.png");
		else
			sprite = cocos2d::Sprite::create("check_white.png");
		this->addChild(sprite);
		sprite->setPosition(1000, 820 - 130 * i);
		this->addChild(ttf);
		ttf->setPosition(500, 820 - 130 * i);
	}

	this->cost = cocos2d::LabelTTF::create("0", "DoodleJumpBold_v2.ttf", 100);
	this->addChild(cost);
	this->cost->setPosition(1680, 820);

	this->scheduleOnce(schedule_selector(TrainScene::startDetector), 2.0f);
//	this->schedule(schedule_selector(TrainScene::getCost), 1.0f, kRepeatForever, 3.0f);

	auto closeItem = MenuItemImage::create(
                                           "fanhui1.png",
                                           "fanhui1.png",
                                           CC_CALLBACK_1(TrainScene::menuCloseCallback, this));
	
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	closeItem->setPosition(1450, 550);

	auto qianghuaItem = MenuItemImage::create(
                                           "kaishi1.png",
                                           "kaishi2.png",
                                           CC_CALLBACK_1(TrainScene::qianghuaCallback, this));
	
	qianghuaItem->setPosition(1450, 350);

    auto menu = Menu::create(closeItem, qianghuaItem, NULL);
    menu->setPosition(Vec2::ZERO);
	this->addChild(menu , 100);

	return true;
}

void TrainScene::qianghuaCallback(Ref* pSender) {
	int add = 0;
	for(int i = 0 ;i < 5; i++)
		if(true == this->isFinish[i])
			add ++;

	auto scene = qianghuaScene::createScene(add);
	Director::getInstance()->replaceScene(scene);
}

void TrainScene::menuCloseCallback(Ref* pSender) {
	auto scene = SelectScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void TrainScene::startDetector(float dt) {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	cocos2d::JniMethodInfo info;
	bool ret = cocos2d::JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/TestJni", "startStepDetector", "()V");
	if(ret) {
		log("call void func1() success");
		info.env->CallStaticVoidMethod(info.classID, info.methodID);
	}
#endif
}

void TrainScene::getCost(float dt) {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	log("success");
	cocos2d::JniMethodInfo info;
	bool ret = cocos2d::JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/TestJni", "getCost", "()I");

	if(ret) {
		log("call getCost");
		jint cost = info.env->CallStaticIntMethod(info.classID,info.methodID);
		        //使用jstring2string函数将返回的jstring类型的值转化为c++中的string类型

		char display[10];
		sprintf(display, "%d", cost);
		std::string text = display;
		//std::string text = JniHelper::jstring2string(jstr);
		for(int i = 1; i < 5; i++) {
			if((false == this->isFinish[i]) && (cost >= taskInt[i])) {
				this->isFinish[i] = true;
				auto sprite = cocos2d::Sprite::create("checkbox_white.png");
				this->addChild(sprite);
				sprite->setPosition(1000, 820 - 130 * i);
			}

		}
		this->cost->setString(text);
	}
#endif


#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	ret = cocos2d::JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/TestJni", "isEight", "()I");

	if(ret) {
		jint isEight = info.env->CallStaticIntMethod(info.classID,info.methodID);
		        //使用jstring2string函数将返回的jstring类型的值转化为c++中的string类型
		if(1 == isEight)
			this->isFinish[0] = true;
		else
			this->isFinish[0] = false;
	}

#endif
}

cocos2d::Scene* TrainScene::createScene() {
    // 'scene' is an autorelease object
	if(nullptr == shared) {
		auto scene = cocos2d::Scene::create();

		// 'layer' is an autorelease object
		auto layer = TrainScene::create();

		// add layer as a child to scene
		scene->addChild(layer, 0, 1);
		shared = scene;
		shared->retain();
	}
    // return the scene
	shared->getChildByTag(1)->schedule(schedule_selector(TrainScene::getCost), 1.0f, kRepeatForever, 3.0f);
    return shared;
}
