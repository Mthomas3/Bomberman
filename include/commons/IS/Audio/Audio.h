#pragma once

#include <SFML/Audio.hpp>
#include <string>

namespace IS
{
	namespace Audio
	{
		enum AudioType
		{
			NONE = 0,
			MUSIC = 1,
			SOUND = 2,
		};

		class Audio
		{

		protected:
			std::string filename;
			AudioType type;
			sf::SoundBuffer buffer;
			sf::Sound sound;
			sf::Music music;
			int volume;
			bool loaded;
		public:
			Audio();
			Audio(const std::string &file);
			~Audio();
			bool Play();
			bool Stop();
			bool Pause();
			void setVolume(int volume);
			bool setFile(const std::string &file);

			bool isPlaying();

		};
	}
}