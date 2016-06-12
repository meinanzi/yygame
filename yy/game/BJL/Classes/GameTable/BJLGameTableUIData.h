#ifndef __BJLGameTableUIData_h__
#define __BJLGameTableUIData_h__

#include "HNNetExport.h"
#include "BJLGameLogic.h"
#include "BJLGameTableUICallback.h"

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <string>

using namespace cocostudio;
using namespace ui;
using namespace std;

namespace BJL
{
	#define  NUM_OF_VIEW 4   //滚动层中显示4个

//////////////////////////////////////////////////////////////////////////
//上庄列表模块

	typedef struct tagDealer
	{
		std::string dealerName;
	}T_DEALER;

	typedef struct  tagDealerList
	{
		ListView*			_listView;
		ImageView*			_item;


		void initDealerList(ListView* listView, ImageView* item)
		{
			_listView = listView;
			_item = item;
			_listView->setItemModel(_item);
		}

		void setDealerData(const vector<T_DEALER>& dealers)
		{
			dealerList = dealers;
		}

		void resetDealerList(const vector<T_DEALER>& dealers)
		{
			dealerList = dealers;
			BYTE i = 0;
			
			auto length = _listView->getItems().size();
			for (i; i < length; i++)
			{
				_listView->removeLastItem();
			}

			for (i = 0; i < dealerList.size(); i++)
			{
				auto item = _listView->getItem(i);
				if (nullptr == item)
				{
					_listView->pushBackDefaultItem();
					item = _listView->getItem(i);
				}
				item->setVisible(true);
				auto name = (Text* ) item->getChildByName("name"); 
				name->setString(dealerList[i].dealerName.c_str());
			}
			
		}

		void addDealer(const T_DEALER& dealer)
		{
			dealerList.push_back(dealer);
			_listView->pushBackDefaultItem();
			auto length = _listView->getItems().size();
			auto item = _listView->getItem(length - 1);
			auto name = (Text* ) _item->getChildByName("name"); 
			name->setString(GBKToUtf8(dealer.dealerName.c_str()));
		}

		vector<T_DEALER>	dealerList;
	}T_DEALER_LIST;



	//////////////////////////////////////////////////////////////////////////
	//下注筹码模块
	//筹码按钮
	typedef struct CurJtInfo 
	{
		int         val;
		int         xpos;
		CurJtInfo()
		{
			init();
		}
		void init()
		{
			val = 0;
			xpos = 0;
		}
	}CurJtInfo;


	typedef struct NoteViewInfo
	{
		std::vector<Sprite *> noteviews;
		TextAtlas *numview;
		TextAtlas *mynumview;
		Button *m_imgtmp;
		int curindex;
		int curzorder;
		int curnotes;
		int mycurnotes;
		int minx;
		int maxx;
		int miny;
		int maxy;
		Rect rect;
		NoteViewInfo()
		{
			curindex = 0;
			curzorder = 0;
			curnotes = 0;
			mycurnotes= 0;
			minx = 0;
			maxx = 0;
			miny = 0;
			maxy = 0;
		}

		void init(Button* ptmp, TextAtlas *pnumview, TextAtlas *pnummy)
		{
			m_imgtmp = ptmp;
			curzorder = ptmp->getLocalZOrder();
			numview = pnumview;
			mynumview = pnummy;
			this->rect = ptmp->getBoundingBox();
			minx = rect.getMinX() + rect.size.width * 0.2;
			maxx = rect.getMaxX() - rect.size.width * 0.2;
			miny = rect.getMinY() + rect.size.height * 0.2;
			maxy = rect.getMaxY() - rect.size.height * 0.2;
		}

		void hide()
		{
			while (!noteviews.empty())
			{
				noteviews.back()->removeFromParent();
				noteviews.pop_back();
			}
			curindex = 0;
			curnotes = 0;
			curzorder = 0;
			mycurnotes= 0;
			numview->setVisible(false);
			mynumview->setVisible(false);
		}

		/*
		@param type is the type of jetton, means who bet
		type = 1   means is me, otherwise means other
		*/

		void dropone(const CurJtInfo &curjtinfo , BYTE type)
		{
			//jettonnode.png
			//Rect texrect = Rect(curjtinfo.xpos*70, 0, 70, 70);
			if (type == 0)
			{
				curnotes += curjtinfo.val;
				char str[15];
				sprintf(str,"%d",curnotes);
				numview->setString(str);
				numview->setVisible(true);
			}
			else
			{
				mycurnotes += curjtinfo.val;
				char str[15];
				sprintf(str,"%d",mycurnotes);
				mynumview->setString(str);
				mynumview->setVisible(true);

				return ;
			}

			/*Vec2 pt = getRandPositionInTheRect();
			char str[30] ={0};
			sprintf(str,"BT_JETTON_%d.png", curjtinfo.xpos + 1);
			auto pnew = Sprite::createWithSpriteFrameName(str);
			pnew->setPosition(pt);
			pnew->setScale(0.7f);
			m_imgtmp->getParent()->addChild(pnew , 40);
			noteviews.push_back(pnew);*/

			curindex = (curindex+1)%100;
			if (0==curzorder) curzorder++;
		}
	
		//在这个区域内随机获取一个位置
		Vec2 getRandPositionInTheRect()
		{
			Vec2 pt;
			pt.x = minx + ( rand() % (maxx-minx) );
			pt.y = maxy - ( rand() % (maxy-miny) );
			return pt;
		}

		bool isInrect(const Vec2 &pt)
		{   
			return rect.containsPoint(pt);
		}
	}NoteViewInfo;

}
#endif // __BJLGameTableUIData_h__

