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

        virtual void PlaySound(int audioClipID) override;
        virtual unsigned short CreateSoundClip(std::string path, unsigned short volume) override;

        virtual void StartSoundQueue() override;

        virtual void Stop() override;
        virtual void ToggleMute() override;

    private:

        class AudioSDLImpl;
        std::unique_ptr<AudioSDLImpl> m_pImpl;
    };
}
