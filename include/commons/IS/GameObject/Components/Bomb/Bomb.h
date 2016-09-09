#pragma once

#include <IS/GameObject/Components/AComponent.h>
#include <OGRE/Ogre.h>
#include <IS/Utils/Time/Time.h>
#include <IS/Physics/Core/PhysicObject.h>
#include <IS/GameObject/Components/Bomb/BombManager.h>
#include <OgreRay.h>
#include <IS/GameObject/Components/DestroyComponent/DestroyComponent.h>
#include <IS/GameObject/Components/Player/Player.h>

namespace IS
{
	namespace GameObject
	{
		namespace Component
		{
			class BombManager;
		}
	}
}

namespace IS
{
	namespace GameObject
	{
		namespace Component
		{
			class Bomb : public AComponent
			{
			public:
				Bomb(Ogre::SceneNode *sceneNode, IS::Scene::Scene *scene, IS::GameObject::Component::BombManager *bombManager, float range);
				virtual ~Bomb();

				virtual void update();
				
				Ogre::SceneNode *getNode();
				IS::Scene::Scene *getScene();

			private:
				void	checkRay();
				void	checkRayUp();
				void	checkRayDown();
				void	checkRayLeft();
				void	checkRayRight();

				float									_range;
				float									_posUp;
				float									_posDown;
				float									_posRight;
				float									_posLeft;
				float									_explodeRangeUp;
				float									_explodeRangeDown;
				float									_explodeRangeRight;
				float									_explodeRangeLeft;
				float									_timer;
				IS::GameObject::Component::BombManager	*_bombManager;
				IS::Physics::StaticObject				*_bombPhysics;
				Ogre::RaySceneQuery						*_rayUp;
				bool									_destroy;
				std::string								_name;
				int										_id;
				int										_decal;
				float									_caseSize;
				float									_expodeMeshSize;
			};
		}
	}
}

