#pragma once

#include <IS/GameObject/Components/AComponent.h>
#include <IS/Utils/Time/Time.h>
#include <OGRE/Ogre.h>

namespace IS
{
	namespace GameObject
	{
		namespace Component
		{
			class DestroyComponent : public AComponent
			{
			public:
				DestroyComponent(Ogre::SceneNode *sceneNode, Scene::Scene *, Ogre::SceneManager *sceneManager);
				virtual ~DestroyComponent();

				virtual void update();
			private:
				float		_timer;
			};
		}
	}
}