#pragma once

#include "Audio.h"

#include <vector>
#include <memory>


class AudioSDL : public Audio
{
public:
    AudioSDL();
    ~AudioSDL();

    virtual void PlaySound(int soundID, float volume) override;
    virtual void StopSound(int soundID) override;
    virtual void StopAllSounds() override;
    virtual int LoadSound(std::string path) override;

private:

    class AudioSDLImpl;
    std::unique_ptr<AudioSDLImpl> m_pImpl;
};

