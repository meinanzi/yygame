#ifndef __GAMEINITIAL_SCENE_H__
#define __GAMEINITIAL_SCENE_H__

#include "HNUIExport.h"
#include "cocos2d.h"
#include "ui/UIVideoPlayer.h"
#include "ui/CocosGUI.h"

class GameInitial : public HNLayer
{
public:
	// 创建场景
    static cocos2d::Scene* createScene();

	// 初始化
    virtual bool init() override;
	virtual void onEnterTransitionDidFinish() override;

	// 播放视频
	void showVideo();

	// 创建对象
	CREATE_FUNC(GameInitial);

	// 动画播放回调(win32)
	void onPlayVideoOverCallback();

	/**
	* 视频播放状态，只有在android和ios平台有效
	*/
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	void videoEventCallback(Ref* sender, cocos2d::experimental::ui::VideoPlayer::EventType eventType);
#endif	

protected:
	GameInitial();
	virtual ~GameInitial();
	void waitting(float dt);   
private:
	bool _isCustomer;
};

#endif // __GAMEINITIAL_SCENE_H__
