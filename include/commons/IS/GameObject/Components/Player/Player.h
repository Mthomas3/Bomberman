#pragma once

#include <IS/GameObject/Components/AComponent.h>
#include <OGRE/Ogre.h>
#include <IS/GameObject/Components/Action/AAction.h>
#include "IS/GameObject/Components/Bomb/BombManager.h"
#include "IS/GameObject/Components/PowerUp/PowerUp.h"
#include "IS/GameObject/Components/PowerUp/PowerUpOnPlayer.h"
#include <IS/GameObject/Components/Player/APlayer.h>

namespace IS
{
	namespace GameObject
	{
		namespace Component
		{
			class PowerUpOnPlayer;
		}
	}
}

namespace IS
{
	namespace GameObject
	{
		namespace Component
		{
			class Player : public APlayer
			{
				public:
					Player(Ogre::SceneNode * sceneNode, IS::Scene::Scene *, int id, Dir, Ogre::Entity *entity);
					Player(Ogre::SceneNode * sceneNode, IS::Scene::Scene *, int id, float speed, Dir, Ogre::Entity *entity);
					virtual ~Player();

					virtual void update();

			//		IS::GameObject::Component::BombManager *getBombManager() const;

				private:
//					int											_id;
	//				IS::GameObject::Component::BombManager		*_bombManager;
		//			IS::GameObject::Component::PowerUpOnPlayer	*_powerUp;
			};	
		}
	}
}
