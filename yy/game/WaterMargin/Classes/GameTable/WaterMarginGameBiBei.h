#ifndef __WaterMargin_GAME_BIBEI_H__
#define __WaterMargin_GAME_BIBEI_H__

#include "HNNetExport.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "HNUIExport.h"
#include "ui/UICheckBox.h"
#include "WaterMarginGameAnimation.h"
#include <vector>

USING_NS_CC;

using namespace cocostudio;
using namespace std;
using namespace ui;
using namespace HN;


namespace WaterMargin
{
	class GameTableLogic;
	class GameBiBei : public  HNLayer
	{
	protected:
		GameBiBei();
		~GameBiBei();
	public:
		static GameBiBei* create(GameTableLogic* tableLogic, Node* str);
	//	CREATE_FUNC(GameBiBei);
		bool init(GameTableLogic* tableLogic, Node* str);

		void startPlay();						//playArmature
		void playgo();
		void playArmature(BYTE aniType); //0-等待，1-输，2-赢，3-摇色子
		void setNote(float delay);
		void viewSaiZi(int s1, int s2);
		void BiBeiOver();						//按下大小和按钮后的处理
		void stopWait();

		void menuClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);		//按钮回调函数
		void playAnimation(float delay);

		bool onTouchBegan(Touch *touch, Event *unused_event);
		void onTouchMoved(Touch *touch, Event *unused_event);
		void onTouchEnded(Touch *touch, Event *unused_event);
		bool containRectTouch();
		void initBIBEI(int value);

		Point _touchDown;
		vector<int> _touchpos;

		ImageView* _secondBG;
		vector<TextAtlas*> _Bdata;
		GameTableLogic* _bibeiLogic;
		vector<ImageView*> _note;
		vector<Button*> _btn;
		vector<ImageView*> _saizi;
		vector<ImageView*> _history;
		Node* _father;
		int _touchIndex;
		Armature* _Armature;
		Size winSize;
		GameAnimation* _gameAni;

		Point _pos[3];
	
		struct ANIMATE
		{
			char*		name;
			int			count;
			float		playtime;
			float		removeTime;
			int			playCount;

			ANIMATE()
			{
				memset(this, 0, sizeof(ANIMATE));
			}
		};

		enum aniType
		{
			WAIT = 0,
			LOST = 1,
			WIN = 2,
			ROLL = 3
		} aniTypeEnum;

		ANIMATE _myAnimation;

		Sprite* _bossSprite;
		Sprite* _leftSprite;
		Sprite* _rightSprite;

		Sprite* _aniB;
		Sprite* _aniL;
		Sprite* _aniR;

		Vec2 _pos_bosslost;
		Vec2 _pos_bossopen;
		Vec2 _pos_bossthrow;
		Vec2 _pos_bosswait;
		Vec2 _pos_bosswin;
		Vec2 _pos_leftlost;
		Vec2 _pos_leftwait;
		Vec2 _pos_leftwin;
		Vec2 _pos_rightlost;
		Vec2 _pos_rightwait;
		Vec2 _pos_rightwin;

	};
}
#endif