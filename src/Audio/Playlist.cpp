
#include <IS/input/Input.hh>
#include <IS/Audio/Playlist.h>
#include <iomanip>
#include <dirent.h>
#include <sys/stat.h>

static std::vector<std::string> getFilesList(std::string folder)
{
	std::vector<std::string> _files;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(folder.c_str())) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			if (ent->d_type == DT_REG)
				_files.push_back(ent->d_name);
		}
		closedir(dir);
	}
	return (_files);
}

IS::Audio::PlayList::PlayList()
{
	this->_musics = getFilesList("Media/audio/musics/");
	keep_alive = 0;
	this->currentAudio = NULL;
}

IS::Audio::PlayList::~PlayList()
{

}

void IS::Audio::PlayList::loop()
{
	bool playing = false;
	int maxMusics = this->_musics.size();
	srand(time(NULL));
	this->musicID = rand() % maxMusics;
	this->stopSound = 0;
	while (this->keep_alive == 1)
	{
		if (playing == false)
		{
			if ((musicID >= 0 && musicID < maxMusics))
			{
				if (this->_music.openFromFile("Media/audio/musics/" + this->_musics[musicID]))
				{
					std::cout << "=====================================================" << std::endl;
					std::cout << "MUSIC: " << this->_musics[musicID] << std::endl;
					this->_music.play();
					playing = true;
					while (this->_music.getStatus() == sf::Music::Playing && this->stopSound == 0)
					{
						sf::sleep(sf::milliseconds(100));
						std::cout << "\rPlaying... " << std::fixed << std::setprecision(2) << this->_music.getPlayingOffset().asSeconds() << " sec   ";
						std::cout << std::flush;
					}
					this->stopSound = 0;
					playing = false;
				}
			}
			this->Next();
		}
	}
}

static void test()
{
	while (true)
	{
		if (InputManager::InputListener::getInstance()->isKey(InputManager::InputListener::Key::KEY_B))
			IS::Audio::PlayList::getInstance()->Prev(true);
		if (InputManager::InputListener::getInstance()->isKey(InputManager::InputListener::Key::KEY_N))
			IS::Audio::PlayList::getInstance()->Next(true);
	}
}

bool IS::Audio::PlayList::Play()
{
	keep_alive = 1;
	std::thread task(&IS::Audio::PlayList::loop, this);
	task.detach();
	std::thread task2(test);
	task2.detach();

	return true;
}

bool IS::Audio::PlayList::Next(bool force)
{
	if (this->musicID + 1 >= this->_musics.size())
		this->musicID = 0;
	else
		musicID++;
	if (force)
		this->stopSound = 1;
	return true;
}

bool IS::Audio::PlayList::Prev(bool force)
{
	this->musicID = (this->musicID - 2) % _musics.size();
	if (force)
		this->stopSound = 1;
	return true;
}

bool IS::Audio::PlayList::Stop()
{	
	this->keep_alive = 0;
}

bool IS::Audio::PlayList::setVolume(float volume)
{
	this->_music.setVolume(volume * 100);
	return true;
}