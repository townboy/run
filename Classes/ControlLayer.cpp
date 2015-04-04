#include "ControlLayer.h"

Scene* ContronlLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ContronlLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

	auto joystick = Joystick::create();
	scene->addChild(joystick, 100);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool ContronlLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
    auto _listener = EventListenerCustom::create(JoystickEvent::EVENT_JOYSTICK, [=](EventCustom* event){
        JoystickEvent* jsevent = static_cast<JoystickEvent*>(event->getUserData());
    });

    _eventDispatcher->addEventListenerWithFixedPriority(_listener, 1);
    return true;
}

