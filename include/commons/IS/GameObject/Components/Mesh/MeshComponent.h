#pragma once

#include <IS/GameObject/Components/AComponent.h>
#include <OGRE/Ogre.h>

namespace IS
{
	namespace GameObject
	{
		namespace Component
		{
			class MeshComponent : public AComponent
			{
			public:
				MeshComponent(Ogre::SceneNode *sceneNode, Scene::Scene *, Ogre::SceneManager *sceneManager, const std::string &name, const std::string &path);
				virtual ~MeshComponent();

				virtual void update();

				Ogre::Entity *getEntity() const { return _entity; }

			private:
				Ogre::SceneManager	*_sceneManager;
				Ogre::Entity		*_entity;
			};
		}
	}
}