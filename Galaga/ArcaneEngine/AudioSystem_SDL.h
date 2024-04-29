#pragma once

#include "Audio.h"

#include <vector>
#include <memory>

namespace AE
{
    class AudioSystem_SDL : public AudioSystem
    {
    public:
        AudioSystem_SDL();
        ~AudioSystem_SDL();

        virtual void PlaySound(int soundID, float volume) override;
        virtual void StopSound(int soundID) override;
        virtual void StopAllSounds() override;
        virtual int LoadSound(std::string path) override;

    private:

        class AudioSDLImpl;
        std::unique_ptr<AudioSDLImpl> m_pImpl;
    };
}
