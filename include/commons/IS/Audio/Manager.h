#pragma once

#include <IS/Utils/Singleton.h>
#include <IS/Audio/Audio.h>

#include <vector>

namespace IS
{
	namespace Audio
	{
		class AudioManager : public IS::Utils::Singleton<AudioManager>
		{
		private:

			std::vector<Audio *> env_audio;


		public:
			AudioManager();

			Audio *playSound(const std::string &file);
			Audio *playSound(const std::string &file, int volume);


			bool stopSounds();
			bool stopSound(Audio *audio);
			
		};
	}
}