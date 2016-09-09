#pragma once

#include <vector>
#include <IS/Utils/Singleton.h>
#include <IS/Audio/Audio.h>
#include <IS/Audio/Manager.h>
#include <IS/Audio/Playlist.h>
#include <iostream>
#include <thread>
#include <stdio.h>


namespace IS
{
	namespace Audio
	{
		class PlayList : public IS::Utils::Singleton<PlayList>
		{
		public:
			PlayList();
			~PlayList();

			void loop();
			bool Play();
			bool Play(int index);
			bool Next(bool force = false);
			bool Prev(bool force = false);
			bool Stop();
			bool setVolume(float vol);

		private:
			IS::Audio::Audio *currentAudio;
			std::vector<std::string> _musics;
			sf::Music _music;
			int isPlaying;
			int stopSound;
			int musicID;
			int keep_alive;
		};

	}
}
