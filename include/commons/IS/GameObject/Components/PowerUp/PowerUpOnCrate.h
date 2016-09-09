#pragma once

#include <IS/GameObject/Components/AComponent.h>
#include <IS/Physics/Core/PhysicObject.h>
#include <OGRE/Ogre.h>
#include <IS/GameObject/GameObject.h>
#include <IS/GameObject/Components/Mesh/MeshComponent.h>

namespace IS
{
	namespace GameObject
	{
		namespace Component
		{
			class PowerUpOnCrate : public AComponent
			{
			public:
				PowerUpOnCrate(Ogre::SceneNode *sceneNode, IS::Scene::Scene *scene);
				virtual ~PowerUpOnCrate();

				virtual void update();

			};
		}
	}
}
