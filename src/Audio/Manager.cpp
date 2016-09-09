#include <IS/Audio/Manager.h>
#include <string>
#include <iostream>
#include <thread>
#include <algorithm>


IS::Audio::AudioManager::AudioManager()
{

}

void startSound(IS::Audio::Audio *audio)
{
	audio->Play();
}

void executeSound(const std::string &file, int volume)
{
	IS::Audio::Audio audio(file);
	audio.setVolume(volume);
	audio.Play();
}

IS::Audio::Audio *IS::Audio::AudioManager::playSound(const std::string &file)
{
	IS::Audio::Audio *audio = new IS::Audio::Audio(file);
	audio->setVolume(100);
	std::thread task(startSound, audio);
	task.detach();

	env_audio.push_back(audio);
	return audio;
}

IS::Audio::Audio *IS::Audio::AudioManager::playSound(const std::string &file, int volume)
{
	IS::Audio::Audio *audio = new IS::Audio::Audio(file);
	audio->setVolume(volume);
	std::thread task(startSound, audio);
	task.detach();

	env_audio.push_back(audio);
	return audio;
}

bool IS::Audio::AudioManager::stopSound(IS::Audio::Audio *audio)
{
	std::vector<Audio *>::iterator it = std::find(this->env_audio.begin(), this->env_audio.end(), audio);
	if (it != this->env_audio.end())
	{
		(*it)->Stop();
		delete (*it);
		this->env_audio.erase(it);
		return true;
	}
	else
		return false;
}

bool IS::Audio::AudioManager::stopSounds()
{
	for (int index = 0; index < this->env_audio.size(); index++)
	{
		this->env_audio[index]->Stop();
		delete this->env_audio[index];
	}
	this->env_audio.clear();
	return true;
}