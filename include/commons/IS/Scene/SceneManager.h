#pragma once

#include <OGRE/Ogre.h>

namespace IS
{
	namespace Scene
	{
		class SceneManager
		{
		public:
			SceneManager(Ogre::Root *root, const std::string &name = "DefaultSceneManager");
			virtual ~SceneManager();

			void	setAmbiantLight(float r, float g, float b);

			Ogre::SceneManager	*getSceneManager() const { return _sceneManager;  }

		private:
			Ogre::SceneManager	*_sceneManager;
			Ogre::Root			*_root;
		};
	}
}