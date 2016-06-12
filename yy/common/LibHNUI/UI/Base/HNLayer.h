#ifndef __HN_HNLayer_H__
#define __HN_HNLayer_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

namespace HN {

	class HNLayer : public Layer
	{
	public:
		HNLayer(void);
		virtual ~HNLayer(void);

		virtual bool init() override;

	public:
		void setBackGroundImage(const std::string& name);

		void enableKeyboard();
		
		void startShade(int opacity = 100);

        void quicklyShade(int opacity = 100);
        
        // 释放无用的资源
        void releaseUnusedRes();

	public:
		float getRealScaleX(int designWidth);

		float getRealScaleY(int designHeight);

	public:
		bool switchToLayoutControl(Layout* layout, const std::string& controlName, Node* newControl);
        
    protected:
        // 释放资源
        void releaseGameRes(float delta);

	private:
		virtual void updateOpacity(float dt);

	protected:
		Layer*			_colorLayer;
		unsigned char	_opacity;
		Size _winSize;
	};

};

#endif	//__HN_HNLayer_H__
