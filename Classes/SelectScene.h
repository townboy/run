#ifndef _SELECTSCENE_H_
#define _SELECTSCENE_H_
#include "cocos2d.h"

class SelectScene : public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();
	SelectScene();
	virtual bool init();
	CREATE_FUNC(SelectScene);
	void enterGame(float dt);
};

#endif