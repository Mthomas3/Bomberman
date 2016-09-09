#include <IS/Utils/Time/Time.h>

IS::Utils::Time::Time()
{
	_lastTime = std::chrono::system_clock::now();
	_deltaTime = 0.0f;
	_ratio = 1.0f;
}

void IS::Utils::Time::update()
{
	std::chrono::time_point<std::chrono::system_clock> cur = std::chrono::system_clock::now();
	std::chrono::duration<float> diff = cur - _lastTime;
	_deltaTime = diff.count() * _ratio;
	_lastTime = cur;
}