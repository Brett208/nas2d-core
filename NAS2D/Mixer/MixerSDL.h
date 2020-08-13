// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#pragma once

#include "Mixer.h"

namespace NAS2D {

/**
 * \class MixerSDL
 * \brief SDL Mixer.
 *
 * Implements all Mixer functions with the SDL API.
 *
 * \warning	Ensure that the mixer is not playing any audio resources
 *			before freeing them. Not doing so results in crashes.
 */
class MixerSDL : public Mixer
{
public:
	struct Options
	{
		int mixRate;
		int numChannels;
		int sfxVolume;
		int musicVolume;
		int bufferSize;
	};

	static MixerSDL::Options InvalidToDefault(const MixerSDL::Options& options);
	static MixerSDL::Options ReadConfigurationOptions();

	MixerSDL();
	MixerSDL(const Options& options);
	MixerSDL(const MixerSDL&) = delete;
	MixerSDL& operator=(const MixerSDL&) = delete;
	MixerSDL(MixerSDL&&) = default;
	MixerSDL& operator=(MixerSDL&&) = default;
	~MixerSDL() override;

	// Sound Functions
	void playSound(const Sound& sound) override;
	void stopSound() override;
	void pauseSound() override;
	void resumeSound() override;

	// Music Functions
	void stopMusic() override;
	void pauseMusic() override;
	void resumeMusic() override;

	void fadeInMusic(const Music& music, int loops, int time) override;
	void fadeOutMusic(int time) override;

	bool musicPlaying() const override;

	// Global Functions
	void soundVolume(int level) override;
	void musicVolume(int level) override;

	int soundVolume() const override;
	int musicVolume() const override;

private:
	void onMusicFinished();
};

}
