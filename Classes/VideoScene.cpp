#include "cocos2d.h"
#include "VideoScene.h"
#include "ui/CocosGUI.h"
#include "ui/UIVideoPlayer.h"
#include "StartScene.h"

bool VideoScene::init() {
    if (!Layer::init())
        return false;

	this->showmp4();
	return true;
}

cocos2d::Scene * VideoScene::createScene() {
	auto scene = cocos2d::Scene::create();

	auto layer = VideoScene::create();
	scene->addChild(layer);
	return scene;
}

void VideoScene::showmp4() {
	cocos2d::Size size = cocos2d::Director::getInstance()->getVisibleSize();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	auto videoPlayer = cocos2d::experimental::ui::VideoPlayer::create();
	videoPlayer->setPosition(Vec2(size.width/2, size.height/2));
	//videoPlayer->setPosition(cocos2d::Point());
	videoPlayer->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	videoPlayer->setContentSize(cocos2d::Size(size.width , size.height));
//	videoPlayer->setTouchEnabled(false);
	this->addChild(videoPlayer, 100);
	if (videoPlayer)
	{
		videoPlayer->setFileName("start2.mp4");
		videoPlayer->seekTo(5);
		videoPlayer->play();
		//videoPlayer->addEventListener(CC_CALLBACK_2(HelloWorld::videoEventCallback, this));
	}
	this->scheduleOnce(schedule_selector(VideoScene::to_next), 90.0f);
	return ;
#endif
	this->scheduleOnce(schedule_selector(VideoScene::to_next), 0.0f);
}

void VideoScene::to_next(float dt) {
	auto scene = StartScene::createScene();
	cocos2d::Director::getInstance()->replaceScene(scene);
}
