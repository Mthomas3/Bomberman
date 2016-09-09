#pragma once

#include <OGRE/Ogre.h>
#include <IS/GameObject/Components/IComponent.h>

namespace IS
{
	namespace GameObject
	{
		class IGameObject
		{
		public:
			virtual ~IGameObject() { }

			virtual void			update() = 0;
			virtual Ogre::SceneNode	*getSceneNode() const = 0;
		};
	}
}