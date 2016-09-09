#pragma once

#include <chrono>
#include <IS/Utils/Singleton.h>

namespace IS
{
	namespace Utils
	{
		class Time : public Singleton<Time>
		{
		public:
			Time();

			void	update();
			inline float	getDeltaTime() { return _deltaTime;  }
			inline void		setRatio(float ratio) { _ratio = ratio; }
			inline float	getRatio() const { return _ratio; }

		private:
			std::chrono::time_point<std::chrono::system_clock>	_lastTime;
			float												_deltaTime;
			float												_ratio;
		};
	}
}