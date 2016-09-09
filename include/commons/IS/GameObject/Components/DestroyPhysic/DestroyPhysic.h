#pragma once

#include <IS/GameObject/Components/AComponent.h>
#include <IS/Utils/Time/Time.h>
#include <OGRE/Ogre.h>
#include <IS/Physics/Core/PhysicObject.h>

namespace IS
{
	namespace GameObject
	{
		namespace Component
		{
			class DestroyPhysic : public AComponent
			{
			public:
				DestroyPhysic(Ogre::SceneNode *sceneNode, Scene::Scene *, IS::Physics::StaticObject *physic);
				virtual ~DestroyPhysic();

				virtual void update();
			private:
				IS::Physics::StaticObject		*_physic;
			};
		}
	}
}