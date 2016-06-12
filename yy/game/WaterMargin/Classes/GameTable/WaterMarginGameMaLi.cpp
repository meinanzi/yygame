#include "WaterMarginGameMaLi.h"
#include "WaterMarginGameTableLogic.h"
#include "WaterMarginGameRollImage.h"
#include "WaterMarginGameAudio.h"
#include "cocostudio/CocoStudio.h"

#define Anm_WAIT 33

namespace WaterMargin
{
	GameMaLi::GameMaLi()
		:_time(0)
		,_Count(0)
		,_imageIndex(-1)
		,_father(nullptr)
	{
		memset(_imageType, 0, sizeof(_imageType));
	}

	GameMaLi::~GameMaLi()
	{

	}

	GameMaLi* GameMaLi::create(GameTableLogic* tableLogic, Node* node)
	{
		GameMaLi* ptr = new GameMaLi();
		if (nullptr != ptr && ptr->init(tableLogic, node))
		{
			ptr->autorelease();
			return ptr;
		}
		return nullptr;
	}

	bool GameMaLi::init(GameTableLogic* tableLogic, Node* node)
	{
		GameAudio::playGundong();
		_MaLiLogic = tableLogic;
		_father = node;

		Size winSize = Director::getInstance()->getVisibleSize();

		auto MaliNode = CSLoader::createNode("WaterMargin/tableui/MaLi.csb");
		auto secondWidget = (Layout*)MaliNode->getChildByName("Panel_6");
		//auto secondWidget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("WaterMargin/tableui/MaLi.json");
		secondWidget->setAnchorPoint(Vec2(0.5, 0.5));
		secondWidget->setPosition(Vec2(640,360));
		this->addChild(MaliNode);
		secondWidget->setScale(winSize.width/1280, winSize.height/720);

		_MaLiBG = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(secondWidget, "bg"));
		auto size = _MaLiBG->getContentSize();
		//_MaLiBG->setScale(winSize.width/size.width, winSize.height/size.height);
		HNLOG("wide = %f,          high = %f", winSize.width/size.width, winSize.height/size.height);
		//24张滚动图
		_image.reserve(24);
		char name[30] = {0};
		for (int i = 0; i < 24; i++)
		{
			sprintf(name, "x_%d", i+1);
			auto image = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(_MaLiBG, name));
			image->setVisible(false);
			_image.push_back(image);
		}		
		////4张结果图
		//for (int i = 0; i < 4; i++)
		//{
		//	sprintf(name, "Image_%d", i+1);
		//	auto image = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(_MaLiBG, name));
		//	_resultpos.push_back(image);
		//}

		//四个显示数据
		for (int i = 1; i < 5; i++)
		{
			sprintf(name, "data_%d", i);
			auto Bdata = dynamic_cast<TextAtlas*>(ui::Helper::seekWidgetByName(_MaLiBG, name));
			_data.push_back(Bdata);
		}

		//大、小、和按钮
		/*for (int i = 1; i < 4; i++)
		{
			sprintf(name, "select_%d", i);
			auto btn = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_secondBG, name));
			btn->addTouchEventListener(CC_CALLBACK_2(GameBiBei::menuClickCallback, this));
			btn->setVisible(false);
			_btn.push_back(btn);
		}*/

		//比倍、得分按钮
		/*for (int i = 1; i < 3; i++)
		{
			sprintf(name, "btn_%d", i);
			auto btn = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_secondBG, name));
			btn->addTouchEventListener(CC_CALLBACK_2(GameBiBei::menuClickCallback, this));	
			_btn.push_back(btn);
		}*/

		//三个筹码显示
		/*for (int i = 0; i < 3; i++)
		{
			sprintf(name, "note_%d", i+1);
			auto note = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(_secondBG, name));
			note->setVisible(false);
			_note.push_back(note);
		}
*/
		//播放roll动画
		createImage();
		startPlay();

		return true;
	}


	void GameMaLi::menuClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
	{
		if (Widget::TouchEventType::ENDED != touchtype)	return;
		Button* ptr = (Button*)pSender;
		switch (ptr->getTag())
		{
		case 224:
			
			break;
		case 225:
			
			break;
		default:
			break;
		}

	}

	void GameMaLi::startPlay()
	{
		_time = 0.01f;
		this->runAction(Sequence::create(DelayTime::create(1.0f), 
			CallFunc::create(CC_CALLBACK_0(GameMaLi::playgo, this)), nullptr));
	}

	void GameMaLi::playgo()
	{
		_data[3]->setString("5");
		_data[3]->setTag(5);
		schedule(schedule_selector(GameMaLi::rolling), _time);
		schedule(schedule_selector(GameMaLi::runTime), 1.0);
		//schedule(schedule_selector(GameMaLi::runImage),)
	}

	void GameMaLi::runTime(float delay)
	{
		char name[10] = {0};
		int tag = _data[3]->getTag()-1;
		_data[3]->setTag(tag);
		sprintf(name,"%d", tag);
		_data[3]->setString(name);
		if (tag == 0)
		{
			unschedule(schedule_selector(GameMaLi::runTime));
		}
	}

	void GameMaLi::stopRoll()
	{
		unschedule(schedule_selector(GameMaLi::rolling));
	}

	void GameMaLi::rolling(float delay)
	{
		for (int i = 0; i < 4; i++)
		{

			if (_rollImage[i]->getRun())			_rollImage[i]->imageRun();
			if (_readyImage[i]->getRun())		_readyImage[i]->imageRun();
		}


		for (int i = 0; i < 24; i++)
		{
			if (_image[i]->isVisible())
			{
				
				if (i == _imageIndex-1 && _data[3]->getTag() < 2 && _time >= 0.07)
				{
					stopRoll();
					_time = 0.01f;
					_MaLiLogic->handleMaLiViewGold();
					_image[_imageIndex-1]->runAction(Sequence::create(Blink::create(2.0f, 6), 
						CallFunc::create([&](){_MaLiLogic->sendMaLiMessage();}),nullptr));

					if (_Count == 1)
					{
						if (_imageIndex == 4 || _imageIndex == 10 || _imageIndex == 16 || _imageIndex == 22)
						{
							_MaLiLogic->MaLiJudge();
						}
					}
					return;
				}

				_image[i]->setVisible(false);

				if (i == 23)
				{
					_image[0]->setVisible(true);
					if (_data[3]->getTag() <= 2)
					{
						//裁剪滚动停止
						char name[30] = {0};
						int i = 0;
						for (; i < 4; i++)
						{
							if (_rollImage[i]->getRun() && _readyImage[i]->getRun())
							{
								_rollImage[i]->setRun(false);
								_readyImage[i]->setRun(false);
								sprintf(name, "image%d.png", _imageType[i]+1); 
								_rollImage[i]->imageStopAndSetFrame(name);
								_readyImage[i]->resetReadyImage();

							}
						}
						_time = 0.08f;
						stopRoll();
						schedule(schedule_selector(GameMaLi::rolling), _time);
						
					}							
					return;
				}
				_image[i+1]->setVisible(true);
				return;
			}
		}
		_image[0]->setVisible(true);
	}

	void GameMaLi::createResultImage(int* str)
	{
		char name[40] = {0};
		for (int i = 0; i < 4; i++)
		{
			sprintf(name, "WaterMargin/tableui/MaLi/%d.png", str[i]);
			_resultpos[i]->loadTexture(name);
			_resultpos[i]->setVisible(true);
			/*auto image = ImageView::create(name);
			_MaLiBG->addChild(image);
			image->setPosition(_resultpos[i]->getPosition());
			image->setContentSize(_resultpos[i]->getCustomSize());*/
		}
	}

	void GameMaLi::createImage()
	{
		char name[64] = {0};
		for (int i = 0; i < 4; i++)
		{
			sprintf(name, "Panel_%d", i+1);
			auto cliper = dynamic_cast<Layout*>(_MaLiBG->getChildByName(name));
			//cliper->setZOrder(-5);
			cliper->ignoreAnchorPointForPosition(false);
			auto size = cliper->getContentSize();

			for (int j = 0; j < 2; j++)
			{
				if (j == 0)
				{
					int num = CCRANDOM_0_1()*8 + 2;
					sprintf(name,"image%d.png", num);
					auto image = RollImage::create(name);
					cliper->addChild(image);
					image->setPosition(Vec2(size.width/2, size.height/2));
					auto rsize = image->getContentSize();
					image->setScale(size.width/rsize.width, size.height/rsize.height);
					_rollImage.push_back(image);
				}
				else
				{
					int num = CCRANDOM_0_1()*8 + 2;
					sprintf(name,"image%d.png", num);
					auto image = RollImage::create(name);
					cliper->addChild(image);
					image->setPosition(Vec2(size.width/2, size.height*3/2));
					auto rsize = image->getContentSize();
					image->setScale(size.width/rsize.width, size.height/rsize.height);
					_readyImage.push_back(image);
				}

			}

		}
	}
}
