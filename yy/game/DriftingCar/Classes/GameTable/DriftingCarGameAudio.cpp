
#include "DriftingCarGameAudio.h"
#include "HNUIExport.h"

namespace DriftingCar
{
#define GAME_AUDIO_ROOT "DriftingCar/CardUi/Res/Sound/"

	void DriftingCar::GameAudio::PlayBackground()
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("background.mp3");

		HNAudioEngine::getInstance()->playBackgroundMusic(filename.c_str(),1);

//		GameAudioPlaySound("background.mp3");
	}

	void DriftingCar::GameAudio::StopBackground()
	{
		HNAudioEngine::getInstance()->stopBackgroundMusic();
	}

	void DriftingCar::GameAudio::StartPaoMaDeng()
	{
		GameAudioPlaySound("Start_car.mp3");
	}

	void DriftingCar::GameAudio::PlayBetChouma()
	{
		GameAudioPlaySound("chouma.mp3");
	}

	void GameAudio::BetTishi()
	{
		GameAudioPlaySound("start_bet.mp3");
	}

	void GameAudio::StopCarAnimation()
	{
		GameAudioPlaySound("stopcar_baofeng.mp3");
	}

	void GameAudio::TimeWarnining()
	{
		GameAudioPlaySound("timewarning.mp3");
	}

	void GameAudio::pauseBackground()
	{
		HNAudioEngine::getInstance()->pauseBackgroundMusic();
	}

	void GameAudio::resumeBackground()
	{
		HNAudioEngine::getInstance()->resumeBackgroundMusic();
	}

	void DriftingCar::GameAudio::GameAudioPlaySound( const std::string& strName )
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append(strName);

		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

}
