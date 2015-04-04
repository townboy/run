#ifndef __VIDEOSCENE_H__
#define __VIDEOSCENE_H__

#include "cocos2d.h"

class VideoScene : public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
    CREATE_FUNC(VideoScene);
	void showmp4();
	void to_next(float dt);
};

#endif
