#ifndef __TRAINSCENE_H__
#define __TRAINSCENE_H__

#include "cocos2d.h"

USING_NS_CC;
class TrainScene : public cocos2d::Layer {
public:
	static cocos2d::Scene* shared;
	static cocos2d::Scene* createScene();
	TrainScene();
	bool init();
	bool isFinish[5];
    CREATE_FUNC(TrainScene);
	static cocos2d::Scene* getInstance();
	void startDetector(float dt);
	void getCost(float dt);
	cocos2d::LabelTTF * cost;
	void menuCloseCallback(Ref* pSender);
	void qianghuaCallback(Ref* pSender);
};

#endif
