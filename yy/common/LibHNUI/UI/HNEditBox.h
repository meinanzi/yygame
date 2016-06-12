#ifndef __libCommon_HNEditBox_h__
#define __libCommon_HNEditBox_h__

#include "cocos2d.h"
#include <string>
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

namespace HN
{

	class HNEditBox : public ui::EditBox
	{
	public:
		//使用方法：参数1为编辑器上摆放的输入框，参数2为使用这个输入框的类，需要继承自ui::EditBoxDelegate
		static HNEditBox* createEditBox(TextField* module, ui::EditBoxDelegate* pDelegate);

	private:
		HNEditBox(); 
		virtual ~HNEditBox();

	public:
		bool init(TextField* module, ui::EditBoxDelegate* pDelegate);
		//设置输入框内容
		void setString(std::string text);
		//获取输入框内容
		std::string getString();
		//获取输入框内容长度
		int getStringLength();
		//设置是否显示密码
		void setPasswordEnabled(bool enable);
		//查询是否显示密码
		bool isPasswordEnabled();
	};

}
#endif // __libCommon_HNEditBox_h__
