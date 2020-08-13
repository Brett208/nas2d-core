// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "MixerSDL.h"
#include "../Configuration.h"
#include "../Exception.h"
#include "../Utility.h"
#include "../ContainerUtils.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <array>
#include <iostream>
#include <functional>
#include <algorithm>

using namespace NAS2D;
using namespace NAS2D::Exception;


namespace {
	// ==================================================================================
	// INTEROP WITH SDL2_MIXER
	// ==================================================================================
	// Global so it can be accessed without capturing `this`
	Signals::Signal<> musicFinished;
	// ==================================================================================


	constexpr int AudioVolumeMin = 0;
	constexpr int AudioVolumeMax = 128;

	constexpr int AudioNumChannelsMin = 1;
	constexpr int AudioNumChannelsMax = 2;

	constexpr int AudioQualityLow = 11025;
	constexpr int AudioQualityMedium = 22050;
	constexpr int AudioQualityHigh = 44100;

	constexpr auto AllowedMixRate = std::array{AudioQualityLow, AudioQualityMedium, AudioQualityHigh};

	constexpr int AudioBufferSizeMin = 256;
	constexpr int AudioBufferSizeMax = 4096;

	MixerSDL::Options InvalidToDefault(const MixerSDL::Options& options)
	{
		return {
			has(AllowedMixRate, options.mixRate) ? options.mixRate : AudioQualityMedium,
			std::clamp(options.numChannels, AudioNumChannelsMin, AudioNumChannelsMax),
			std::clamp(options.sfxVolume, AudioVolumeMin, AudioVolumeMax),
			std::clamp(options.musicVolume, AudioVolumeMin, AudioVolumeMax),
			std::clamp(options.bufferSize, AudioBufferSizeMin, AudioBufferSizeMax)
		};
	}

	MixerSDL::Options ReadConfigurationOptions()
	{
		const auto& configuration = Utility<Configuration>::get();
		return {
			configuration.audioMixRate(),
			configuration.audioStereoChannels(),
			configuration.audioSfxVolume(),
			configuration.audioMusicVolume(),
			configuration.audioBufferSize()
		};
	}
}


/*
 * C'tor.
 */
MixerSDL::MixerSDL() : MixerSDL(InvalidToDefault(ReadConfigurationOptions()))
{
}


MixerSDL::MixerSDL(const Options& options)
{
	std::cout << "Initializing Mixer... ";

	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		throw mixer_backend_init_failure(SDL_GetError());
	}

	if (Mix_OpenAudio(options.mixRate, MIX_DEFAULT_FORMAT, options.numChannels, options.bufferSize))
	{
		throw mixer_backend_init_failure(Mix_GetError());
	}

	soundVolume(options.sfxVolume);
	musicVolume(options.musicVolume);

	musicFinished.connect(this, &MixerSDL::onMusicFinished);
	Mix_HookMusicFinished([](){ musicFinished(); });

	std::cout << "done." << std::endl;
}


/*
 * D'tor.
 */
MixerSDL::~MixerSDL()
{
	stopAllAudio();

	Mix_CloseAudio();

	Mix_HookMusicFinished(nullptr);
	musicFinished.disconnect(this, &MixerSDL::onMusicFinished);

	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	std::cout << "Mixer Terminated." << std::endl;
}

void MixerSDL::onMusicFinished()
{
	mMusicComplete.emit();
}


void MixerSDL::playSound(const Sound& sound)
{
	Mix_PlayChannel(-1, sound.sound(), 0);
}


void MixerSDL::stopSound()
{
	Mix_HaltChannel(-1);
}


void MixerSDL::pauseSound()
{
	Mix_Pause(-1);
}


void MixerSDL::resumeSound()
{
	Mix_Resume(-1);
}


void MixerSDL::stopMusic()
{
	Mix_HaltMusic();
}


void MixerSDL::pauseMusic()
{
	Mix_PauseMusic();
}


void MixerSDL::resumeMusic()
{
	Mix_ResumeMusic();
}


void MixerSDL::fadeInMusic(const Music& music, int loops, int time)
{
	Mix_FadeInMusic(music.music(), loops, time);
}


void MixerSDL::fadeOutMusic(int delay)
{
	Mix_FadeOutMusic(delay);
}


bool MixerSDL::musicPlaying() const
{
	return Mix_PlayingMusic() == 1;
}


void MixerSDL::soundVolume(int volume)
{
	Mix_Volume(-1, std::clamp(volume, 0, SDL_MIX_MAXVOLUME));
}


void MixerSDL::musicVolume(int volume)
{
	Mix_VolumeMusic(std::clamp(volume, 0, SDL_MIX_MAXVOLUME));
}


int MixerSDL::soundVolume() const
{
	return Mix_Volume(-1, -1);
}


int MixerSDL::musicVolume() const
{
	return Mix_VolumeMusic(-1);
}
