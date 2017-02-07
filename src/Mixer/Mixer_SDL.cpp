// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2014 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Mixer/Mixer_SDL.h"

#include <iostream>

using namespace NAS2D;

/*
 * C'tor.
 */
Mixer_SDL::Mixer_SDL():	Mixer("SDL Mixer")
{
	init();
}


/*
 * D'tor.
 */
Mixer_SDL::~Mixer_SDL()
{
	// Save current volume levels in the Configuration.
	Utility<Configuration>::get().audioSfxVolume(Mix_Volume(-1, -1));
	Utility<Configuration>::get().audioMusicVolume(Mix_VolumeMusic(-1));

	stopAllAudio();

	Mix_CloseAudio();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	cout << "Mixer Terminated." << endl;
}


void Mixer_SDL::init()
{
	cout << "Initializing Mixer... ";
	// Initialize SDL's Audio Subsystems.
	if(SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		cout << endl << "\tAudio driver not initialized: " << SDL_GetError() << endl;
		throw Exception(0, "Unable to initialize audio", SDL_GetError());
	}
	
	Configuration& c = Utility<Configuration>::get();
    // Initialize the Audio Mixer
    if(Mix_OpenAudio(c.audioMixRate(), MIX_DEFAULT_FORMAT, c.audioStereoChannels(), c.audioBufferSize()))
	{
		cout << endl << "\tAudio driver not initialized: " << Mix_GetError() << endl;
		throw Exception(0, "Unable to initialize audio", SDL_GetError());
	}

	setSfxVolume(c.audioSfxVolume());
	setMusVolume(c.audioMusicVolume());

	cout << "done." << endl;
}


void Mixer_SDL::playSound(Sound& sound)
{
	if(!sound.loaded())
		return;

	Mix_PlayChannel(-1, sound.sound(), 0);
}


void Mixer_SDL::stopSound()
{
	Mix_HaltChannel(-1);
}


void Mixer_SDL::pauseSound()
{
	Mix_Pause(-1);
}


void Mixer_SDL::resumeSound()
{
	Mix_Resume(-1);
}


void Mixer_SDL::playMusic(Music& music)
{
	if(!music.loaded())
		return;

	Mix_PlayMusic(music.music(), -1);
}


void Mixer_SDL::stopMusic()
{
	Mix_HaltMusic();
}



void Mixer_SDL::pauseMusic()
{
	Mix_PauseMusic();
}


void Mixer_SDL::resumeMusic()
{
	Mix_ResumeMusic();
}


void Mixer_SDL::fadeInMusic(Music& music, int loops, int delay)
{
	Mix_FadeInMusic(music.music(), loops, delay);
}


void Mixer_SDL::fadeOutMusic(int delay)
{
	Mix_FadeOutMusic(delay);
}


bool Mixer_SDL::musicPlaying() const
{
	return Mix_PlayingMusic() == 1;
}


void Mixer_SDL::setSfxVolume(int volume)
{
	Mix_Volume(-1, clamp(volume, 0, SDL_MIX_MAXVOLUME));
}


void Mixer_SDL::setMusVolume(int volume)
{
	Mix_VolumeMusic(clamp(volume, 0, SDL_MIX_MAXVOLUME));
}


void Mixer_SDL::mute()
{
	setMusVolume(0);
	setSfxVolume(0);
}


void Mixer_SDL::unmute()
{
	setMusVolume(Mix_VolumeMusic(-1));
	setSfxVolume(Mix_Volume(-1, -1));
}
