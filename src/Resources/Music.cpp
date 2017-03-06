// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Resources/Music.h"
#include "NAS2D/Resources/MusicInfo.h"


#include <iostream>
#include <string>

#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#elif __linux__
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#else
#include "SDL.h"
#include "SDL_mixer.h"
#endif


using namespace NAS2D;


std::map<std::string, MusicInfo>	MUSIC_REF_MAP;		/*< Lookup table for music resource references. */


/**
 * Default c'tor.
 */
Music::Music():	Resource()
{}


/**
 * C'tor.
 * 
 * \param filePath	Path of the music file to load.
 */
Music::Music(const std::string& filePath):	Resource(filePath)
{
	load();
}


/**
 * Copy c'tor.
 */
Music::Music(const Music& _m): Resource(_m.name())
{
	auto it = MUSIC_REF_MAP.find(name());
	if(it != MUSIC_REF_MAP.end())
		it->second.ref_count++;

	loaded(_m.loaded());
}


/**
 * Copy operator.
 */
Music& Music::operator=(const Music& _m)
{
	auto it = MUSIC_REF_MAP.find(name());
	if(it != MUSIC_REF_MAP.end())
		it->second.ref_count++;

	name(_m.name());
	loaded(_m.loaded());

	return *this;
}


/**
 * D'tor.
 */
Music::~Music()
{
	auto it = MUSIC_REF_MAP.find(name());
	if(it == MUSIC_REF_MAP.end())
		return;

	it->second.ref_count--;

	// No more references to this resource.
	if(it->second.ref_count < 1)
	{
		if(it->second.music)
			Mix_FreeMusic(static_cast<Mix_Music*>(it->second.music));

		if(it->second.buffer)
			delete it->second.buffer;

		MUSIC_REF_MAP.erase(it);
	}
}


/**
 * Loads a specified music file.
 * 
 * \note	This function is called internally during instantiation.
 */
void Music::load()
{
	if(MUSIC_REF_MAP.find(name()) != MUSIC_REF_MAP.end())
	{
		MUSIC_REF_MAP.find(name())->second.ref_count++;
		loaded(true);
		return;
	}

	File* file = new File(Utility<Filesystem>::get().open(name()));
	if (file->empty())
	{
		delete file;
		return;
	}

	Mix_Music* music = Mix_LoadMUS_RW(SDL_RWFromConstMem(file->raw_bytes(), file->size()), 0);
	if (!music)
	{
		std::cout << "Music::load(): " << Mix_GetError() << std::endl;
		return;
	}

	auto record = MUSIC_REF_MAP.find(name());
	record->second.buffer = file;
	record->second.music = music;
	record->second.ref_count++;

	loaded(true);
}
