#include "WaterMarginGameRollImage.h"
#include "HNNetExport.h"

namespace WaterMargin
{

	bool RollImage::init()
	{
		if (!HNSprite::init()) return false;
		return true;
	}
	RollImage* RollImage::create(string name)
	{
		//加载滚动图片
		auto cache = SpriteFrameCache::getInstance();
		auto image = new RollImage();
		if (image && image->init())
		{
			image->autorelease();
			image->m_run = true;
			image->initWithSpriteFrame(cache->getSpriteFrameByName(name));
			return image;
		}
		delete image;
		image = NULL;
		return image;
	}

	//图片滚动
	void RollImage::imageRun()
	{
		auto cache = SpriteFrameCache::getInstance();
		this->setPositionY(this->getPositionY()-20);//每帧图片高度加20
		if (this->getPositionY() < -60)
		{
			INT num = rand() % 9 + 1;
			sprintf(name, "image%d.png", num);
			//当图片完全溢出了显示区域则随机更换一个frame，并把位置移动到显示区域下方
			this->setSpriteFrame(cache->getSpriteFrameByName(name));
			this->setPositionY(180);
		}
	}

	//图片停止并替换frame
	void RollImage::imageStopAndSetFrame(string name)
	{
		auto cache = SpriteFrameCache::getInstance();
		this->setSpriteFrame(cache->getSpriteFrameByName(name));
		this->stopAllActions();
		this->setPositionY(80);
		this->runAction(MoveBy::create(0.03f, Vec2(0, -20)));
	}
	
	//复位第二张图片
	void RollImage::resetReadyImage()
	{
		this->stopAllActions();
		this->setPositionY(180);
		//this->setVisible(false);
	}

}