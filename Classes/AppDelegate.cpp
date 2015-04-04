#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "GreenSward.h"
#include "StartScene.h"
#include "SimpleAudioEngine.h"
#include "LogoScene.h"
#include "LoginScene.h"
#include "GameScene.h"
#include "ControlLayer.h"
#include "SelectScene.h"
#include "EndScene.h"
#include "TrainScene.h"

using namespace CocosDenshion;

USING_NS_CC;

AppDelegate::AppDelegate() {
}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }
	glview->setDesignResolutionSize(1920, 1080, ResolutionPolicy::SHOW_ALL);
	glview->setFrameSize(1920 ,1080);

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
	
//	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/foreverYoung.mp3" ,true);
    // create a scene. it's an autorelease object

	auto scene = LogoScene::createScene();

    // run
    director->runWithScene(scene);
	//scene->addChild(GameWorld::getInstance());
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
     SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
