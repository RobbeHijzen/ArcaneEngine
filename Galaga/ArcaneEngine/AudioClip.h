#pragma once

#include <string>

class AudioClip
{
public:

	AudioClip(std::string soundPath, unsigned short volume = 70)
		: m_SoundPath{ soundPath }
		, m_Volume{ volume }
	{
	}

	bool IsLoaded() const { return m_IsLoaded; }

	unsigned short GetVolume() const { return m_Volume; }
	unsigned short GetSoundID() const { return m_SoundID; }

	void SetSoundID(unsigned short soundID) { m_SoundID = soundID;}
	void SetLoaded() { m_IsLoaded = true; }

	std::string GetSoundPath() const { return m_SoundPath; }
	
private:

	bool m_IsLoaded{ false };

	unsigned short m_Volume;
	unsigned short m_SoundID{};

	std::string m_SoundPath;
};

