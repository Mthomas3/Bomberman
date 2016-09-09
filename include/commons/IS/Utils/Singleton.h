#pragma once

namespace IS
{
	namespace Utils
	{
		template <class T>
		class Singleton
		{
		protected:
			Singleton() { }
			virtual ~Singleton() { }

		public:
			static T	*getInstance()
			{
				if (_instance == nullptr)
					_instance = new T;
				return _instance;
			}

			static void	Destroy()
			{
				if (_instance != nullptr)
				{
					delete _instance;
					_instance = nullptr;
				}
			}

		private:
			static T	*_instance;
		};

		template<class T>
		T	*Singleton<T>::_instance = nullptr;
	}
}