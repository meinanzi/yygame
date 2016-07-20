#ifndef __libCommon_LoadingLayer_h__
#define __libCommon_LoadingLayer_h__

#include "Base/HNLayer.h"
#include <string>

namespace HN
{

	class LoadingLayer : public HNLayer
	{
	public:
		// 创建loading
		static LoadingLayer* createLoading(Node* parent, const std::string& text, float fontSize, const std::string& image);
		static LoadingLayer* createLoading(const std::string& text, float fontSize, const std::string& image, const std::string& image_bg);

		static void removeLoading(Node* parent);		
		virtual bool init() override;
		void setText(const std::string& text);
		CREATE_FUNC(LoadingLayer);
        
        void setRemoveTimer(int timer, std::function<void()> removecall = nullptr);
	protected:
		// 创建界面
		void createDialog(const std::string& text, float fontSize, const std::string& image);	
		void createDialog(const std::string& text, float fontSize, const std::string& image, const std::string& image_bg);
	private:
		Label*	_label;
		LoadingLayer();
		virtual ~LoadingLayer();
        
        EventListenerTouchOneByOne* _listener;
	};

}
#endif // __libCommon_LoadingLayer_h__
