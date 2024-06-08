#pragma once

#include <string>
#include "Audio.h"

class AudioClip
{
public:

	AudioClip(std::string soundPath, unsigned short volume = 10, AE::SoundType soundType = AE::SoundType::Chunk)
		: m_SoundPath{ soundPath }
		, m_Volume{ volume }
		, m_SoundType{soundType}
	{
	}

	bool IsLoaded() const { return m_IsLoaded; }

	unsigned short GetVolume() const { return m_Volume; }
	unsigned short GetSoundID() const { return m_SoundID; }
	AE::SoundType GetSoundType() const { return m_SoundType; }

	void SetSoundID(unsigned short soundID) { m_SoundID = soundID;}
	void SetLoaded() { m_IsLoaded = true; }

	std::string GetSoundPath() const { return m_SoundPath; }
	
private:

	bool m_IsLoaded{ false };

	unsigned short m_Volume;
	unsigned short m_SoundID{};

	std::string m_SoundPath;
	AE::SoundType m_SoundType{AE::SoundType::Chunk};
};

