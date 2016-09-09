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
			class DestroyPhysicDynamic : public AComponent
			{
			public:
				DestroyPhysicDynamic(Ogre::SceneNode *sceneNode, Scene::Scene *, IS::Physics::DynamicObject *physic);
				virtual ~DestroyPhysicDynamic();

				virtual void update();
			private:
				IS::Physics::DynamicObject		*_physic;
			};
		}
	}
}