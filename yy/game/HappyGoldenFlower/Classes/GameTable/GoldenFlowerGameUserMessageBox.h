#ifndef __GoldenFlowerGAMEUSERMESSAGE_LAYER_H__
#define __GoldenFlowerGAMEUSERMESSAGE_LAYER_H__

#include "cocos2d.h"
#include "HNNetExport.h"

USING_NS_CC;

using namespace HN;

namespace goldenflower
{
	class GameUserMessageBox : public HNDialogBase
	{
		Rect _userDataBoxRect;
	public:
		bool init(cocos2d::Node* parent, INT userID, std::string headImage);
		static GameUserMessageBox* createMessageBox(cocos2d::Node* parent,INT userID, std::string headImage);
	};
}



#endif // __GoldenFlowerGAMEUSERMESSAGE_LAYER_H__
