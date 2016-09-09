#pragma once

#include <IS/GameObject/Components/AComponent.h>
#include <IS/Physics/Core/PhysicObject.h>
#include <OGRE/Ogre.h>
#include <IS/GameObject/GameObject.h>
#include <IS/GameObject/Components/Mesh/MeshComponent.h>
#include <IS/GameObject/Components/Player/APlayer.h>

namespace IS
{
	namespace GameObject
	{
		namespace Component
		{
			class APlayer;
		}
	}
}

namespace IS
{
	namespace GameObject
	{
		namespace Component
		{
			class PowerUpOnPlayer : public AComponent
			{
				public:
					PowerUpOnPlayer(Ogre::SceneNode *sceneNode, IS::Scene::Scene *scene, IS::GameObject::Component::APlayer *player);
					virtual ~PowerUpOnPlayer();

					virtual void update();

				protected:
					void	checkRay();
					void	checkRayUp();
					void	checkRayDown();
					void	checkRayLeft();
					void	checkRayRight();

					IS::GameObject::Component::APlayer *_player;
					Ogre::RaySceneQuery					*_ray;
					float								_range;
			};
		}
	}
}
