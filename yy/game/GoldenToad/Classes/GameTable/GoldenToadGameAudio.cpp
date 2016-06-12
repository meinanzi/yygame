#include "GoldenToadGameAudio.h"
#include "HNUIExport.h"

namespace GoldenToad
{
	const char*		GAME_AUDIO_ROOT	 =	"goldenToad/sounds/effect/";

	void GoldenToadGameAudio::playBackgroundMusic()
	{
		char str[12];
		std::string filename("goldenToad/sounds/bgm/");
		int num = rand()%5 + 1;
		sprintf(str, "bgm%d.mp3", num);
		filename.append(str);
		HNAudioEngine::getInstance()->playBackgroundMusic(filename.c_str(), true);
	}

	void GoldenToadGameAudio::stopBackgroundMusic()
	{
		HNAudioEngine::getInstance()->stopBackgroundMusic();
	}

	void GoldenToadGameAudio::pauseBackgroundMusic()
	{
		HNAudioEngine::getInstance()->pauseBackgroundMusic();
	}

	void GoldenToadGameAudio::resumeBackgroundMusic()
	{
		HNAudioEngine::getInstance()->resumeBackgroundMusic();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void GoldenToadGameAudio::playSmallFishDiead(FishKind kind)
	{
		std::string filename(GAME_AUDIO_ROOT);

		int num = rand()%2 + 1;
		char str[24];
		sprintf(str, "fish%d_%d.mp3", kind + 1, num);
		filename.append(str);
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void GoldenToadGameAudio::playFireEffect()
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("fire.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void GoldenToadGameAudio::playNetEffect()
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("bgm_net.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void GoldenToadGameAudio::playIonFireEffect()
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("ion_fire.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void GoldenToadGameAudio::playBingoEffect()
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("bingo.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void GoldenToadGameAudio::playGoldEffect()
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("gold.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void GoldenToadGameAudio::playWaveEffect()
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("wave.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void GoldenToadGameAudio::playCatchEffect()
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("catch.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void GoldenToadGameAudio::playBombEffect()
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("superarm.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void GoldenToadGameAudio::playAddEffect()
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("updownpao.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

}
