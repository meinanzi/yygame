#ifndef __GoldenToad_LOADING_LAYER_H__
#define __GoldenToad_LOADING_LAYER_H__

#include "HNNetExport.h"
#include "HNUIExport.h"
#include "cocos2d.h"

USING_NS_CC;

namespace GoldenToad
{
	class GoldenToadGameLoading : public HNLayer
	{
		struct FishSpirteInfo
		{
			std::string name;
			BYTE	count;
			FLOAT	time;
		};

		FishSpirteInfo _fishName[26];

		BYTE  fileNum;

	public:
		typedef std::function<void ()> CloseCallBack;
		CloseCallBack  onCloseCallBack;
	public:
		static GoldenToadGameLoading* create(bool loading);
		void loadingTextureCallback(Texture2D* textureData, std::string plist);
		void loadingCallback(float dt);
		void loadRes();
	protected:
		GoldenToadGameLoading();
		virtual ~GoldenToadGameLoading();

	private:
		bool init(bool loading);
		bool _loading;
	};
}
#endif // __GoldenToad_LOADING_LAYER_H__
