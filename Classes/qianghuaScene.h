#ifndef __QIANGHUASCENE_H__
#define __QIANGHUASCENE_H__

#include "cocos2d.h"
class qianghuaScene : public cocos2d::Layer {
public:
    static cocos2d::Scene* createScene(int add);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    CREATE_FUNC(qianghuaScene);
	int skill_point;
	void changeSkill(int add);
	cocos2d::LabelTTF * left;
	void sendMsg();
	void confirmCallback(cocos2d::Ref* pSender);
	void jia0Callback(cocos2d::Ref* pSender);
	void jian0Callback(cocos2d::Ref* pSender);
	void jia1Callback(cocos2d::Ref* pSender);
	void jian1Callback(cocos2d::Ref* pSender);
	void jia2Callback(cocos2d::Ref* pSender);
	void jian2Callback(cocos2d::Ref* pSender);
	void jia3Callback(cocos2d::Ref* pSender);
	void jian3Callback(cocos2d::Ref* pSender);
	void jia4Callback(cocos2d::Ref* pSender);
	void jian4Callback(cocos2d::Ref* pSender);
	void jia5Callback(cocos2d::Ref* pSender);
	void jian5Callback(cocos2d::Ref* pSender);
	void jia6Callback(cocos2d::Ref* pSender);
	void jian6Callback(cocos2d::Ref* pSender);
	void jia7Callback(cocos2d::Ref* pSender);
	void jian7Callback(cocos2d::Ref* pSender);
	void jia8Callback(cocos2d::Ref* pSender);
	void jian8Callback(cocos2d::Ref* pSender);
	void jia9Callback(cocos2d::Ref* pSender);
	void jian9Callback(cocos2d::Ref* pSender);
	void jia10Callback(cocos2d::Ref* pSender);
	void jian10Callback(cocos2d::Ref* pSender);
	void jia11Callback(cocos2d::Ref* pSender);
	void jian11Callback(cocos2d::Ref* pSender);
	void displayqhz(float dt);
};

#endif
