
#pragma once
#include <string>
namespace DriftingCar{

	class GameAudio
	{
	public:
		static void PlayBackground();
		static void pauseBackground();
		static void resumeBackground();
		static void StopBackground();


		static void StartPaoMaDeng();		//开始跑车动画

		static void PlayBetChouma();		//下注筹码

		static void BetTishi();				//下注提示

		static void StopCarAnimation();		//停止跑车动画

		static void TimeWarnining();		//倒计时警告

	private:
		static void GameAudioPlaySound(const std::string& str);

	};


}