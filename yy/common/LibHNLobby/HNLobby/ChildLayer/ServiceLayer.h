#ifndef ServiceLayer_h__
#define ServiceLayer_h__

#include "HNUIExport.h"
#include "HNNetExport.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class ServiceLayer : public HNLayer, public HNHttpDelegate
{
public:
	// 显示
	void showService(Node* parent, Vec2 vec, int zorder, int tag);

	// 隐藏
	void closeService();

	// 初始化
	virtual bool init() override; 

	// 设置位置
	void setBGPositon(Vec2 vec);

	// 创建
	CREATE_FUNC(ServiceLayer);

protected:
	// 按钮回调
	void onServiceClick(Ref* pRef);

	// 客服信息回调
	virtual void onHttpResponse(const std::string& requestName, bool isSucceed, const std::string &responseData) override;

	// 构造函数
	ServiceLayer();

	// 析构函数
	virtual ~ServiceLayer();

	// 获取客服信息
	void requestServiceInfo();

private:
	// 布局文件
	Node*           _csNode;
	
	// 客服电话
	std::string		_phone;

	// 客服信息
	std::string		_message;

	// 客服邮箱
	std::string		_email;

	// 窗体大小
	Size            _winSize;

	// 按钮
	Button*         _buttonPhone;
	Button*         _buttonMessage;
	Button*         _buttonEmail;
};

#endif // ServiceLayer_h__
