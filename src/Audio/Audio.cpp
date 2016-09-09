#include <IS/Audio/Audio.h>
#include <IS/Audio/Manager.h>

#include <thread>
#include <iomanip>
#include <iostream>

static bool check_extension(const std::string &str, const std::string &suffix)
{
	return str.size() >= suffix.size() &&
		str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

IS::Audio::Audio::Audio()
{
	this->filename = nullptr;
	this->loaded = false;
	this->type = NONE;
	this->volume = 100;
}

IS::Audio::Audio::Audio(const std::string &file)
{
	this->filename = file;
	this->loaded = false;
	this->type = NONE;
	this->volume = 100;
	if (check_extension(file, ".wav"))
	{
		if (this->buffer.loadFromFile(this->filename))
		{
			this->type = SOUND;
			this->loaded = true;
		}
	}
	if (check_extension(file, ".ogg") || check_extension(file, ".flac"))
	{
		if (this->music.openFromFile(this->filename))
		{
			this->type = MUSIC;
			this->loaded = true;
		}
	}
}

IS::Audio::Audio::~Audio()
{
	//IS::Audio::AudioManager::getInstance()->stopSound(this);
	this->Stop();
}

bool IS::Audio::Audio::Play()
{
	if (this->type == SOUND && this->loaded == true)
	{
		this->sound.setBuffer(this->buffer);
		sound.setVolume(this->volume);
		this->sound.play();
		while (this->sound.getStatus() == sf::Sound::Playing)
			sf::sleep(sf::milliseconds(100));
		return true;
	}

	if (this->type == MUSIC && this->loaded == true)
	{
		this->music.setVolume(this->volume);
		this->music.play();
		return true;
	}
	return false;
}

bool IS::Audio::Audio::Stop()
{
	if (this->type == SOUND && this->loaded == true)
	{
		this->sound.stop();
		this->sound.resetBuffer();
		return true;
	}

	if (this->type == MUSIC && this->loaded == true)
	{
		this->music.stop();
		return true;
	}
	return (false);
}

bool IS::Audio::Audio::Pause()
{
	if (this->type == SOUND && this->loaded == true)
	{
		this->sound.pause();
		return true;
	}

	if (this->type == MUSIC && this->loaded == true)
	{
		this->music.pause();
		return true;
	}
	return (false);
}

void IS::Audio::Audio::setVolume(int volume)
{
	this->volume = volume;
	if (this->type == SOUND)
		this->sound.setVolume(volume);
	if (this->type == MUSIC)
		this->music.setVolume(volume);
}

bool IS::Audio::Audio::setFile(const std::string &file)
{
	this->filename = file;
	this->loaded = false;
	this->type = NONE;
	this->volume = 100;
	if (check_extension(file, ".wav"))
	{
		if (this->buffer.loadFromFile(this->filename))
		{
			this->type = SOUND;
			this->loaded = true;
			return true;
		}
	}
	if (check_extension(file, ".ogg") || check_extension(file, ".flac"))
	{
		if (this->music.openFromFile(this->filename))
		{
			this->type = MUSIC;
			this->loaded = true;
			return true;
		}
	}
	return false;
}

bool IS::Audio::Audio::isPlaying()
{
	if (this == nullptr)
		return false;
	try
	{
		if (this->type == MUSIC)
			return (this->music.Playing ? true : false);
		if (this->type == SOUND)
			return (this->sound.Playing ? true : false);
	}
	catch (...)
	{
		return false;
	}
	return false;
}