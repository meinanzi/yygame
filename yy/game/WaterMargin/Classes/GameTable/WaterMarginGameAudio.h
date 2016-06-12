#ifndef __WaterMargin_GAME_AUDIO_H__
#define __WaterMargin_GAME_AUDIO_H__

#include <string>

namespace WaterMargin
{

	class GameAudio
	{
	public:
		static void playBackground();
		static void stopBackground();
		static void pauseBackgroundMusic();
		static void resumeBackgroundMusic();

		static void playMaLiBackground();
		/*static void playBaopai();
		static void playNote();
		static void playTen();
		static void playTenThirty();
		static void playXiaoWang();
		static void playGuoWu();
		static void playClock();
		static void playGameButton();
		static void playSendHandCard();*/
		
		static void playBiBeiWin();
		static void playSHZWin();
		static void playLose();

		static void playDianshu(int value);
		static void playBiBeiBg();
		static void playGundong();
		static void playFirstAnimate();
		static void playSecondAnimate(int value);
		static void playDefeng();
		static void playShangfeng();
		static void playYaosaizi();

		//…Ë÷√“Ù¡ø
		static void setEffectsVolume(float volume);
		static void setBackgroundMusicVolume(float volume);
	};
}


#endif

