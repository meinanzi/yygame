#include "HNAudioEngine.h"
#include <SimpleAudioEngine.h>

namespace HN
{
	static HNAudioEngine* instant = nullptr;

	HNAudioEngine::HNAudioEngine()
	{
		_switchOfMusic = true;
		_switchOfEffect = true;
	}

	HNAudioEngine* HNAudioEngine::getInstance()
	{
		if (nullptr == instant)
		{
			instant = new HNAudioEngine();
		}
		return instant;
	}

	void HNAudioEngine::destroyInstance()
	{
		CC_SAFE_DELETE(instant);
	}

	void HNAudioEngine::setSwitchOfMusic(bool isOpen)
	{
		_switchOfMusic = isOpen;
	}

	bool HNAudioEngine::getSwitchOfMusic() const
	{
		return _switchOfMusic;
	}

	void HNAudioEngine::setSwitcjofEffect(bool isOpen)
	{
		_switchOfEffect = isOpen;
	}

	bool HNAudioEngine::getSwitcjofEffect() const
	{
		return _switchOfEffect;
	}

	void HNAudioEngine::playBackgroundMusic(const char* pszFilePath, bool bLoop)
	{
		if (_switchOfMusic)
		{
			if (nullptr != pszFilePath)
			{
				if (0 != _currentBackgroundName.compare(pszFilePath))
				{
					_currentBackgroundName = pszFilePath;
					CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(pszFilePath, bLoop);
				}
			}
		}
	}

	void HNAudioEngine::stopBackgroundMusic(bool bReleaseData)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(bReleaseData);
	}

	void HNAudioEngine::pauseBackgroundMusic()
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	}

	void HNAudioEngine::resumeBackgroundMusic()
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	}

	unsigned int HNAudioEngine::playEffect(const char* pszFilePath, bool bLoop, float pitch, float pan, float gain)
	{
		if (_switchOfEffect)
		{
			return CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(pszFilePath, bLoop, pitch, pan, gain);
		}
		return 0;
	}

	void HNAudioEngine::setEffectsVolume(float volume)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(volume);
	}

	float HNAudioEngine::getEffectsVolume() const
	{
		return CocosDenshion::SimpleAudioEngine::getInstance()->getEffectsVolume();
	}

	void HNAudioEngine::setBackgroundMusicVolume(float volume)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(volume);
	}

	float HNAudioEngine::getBackgroundMusicVolume() const
	{
		return CocosDenshion::SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();
	}
}
