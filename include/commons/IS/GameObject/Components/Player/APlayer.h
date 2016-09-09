#pragma once

#include <IS/GameObject/Components/AComponent.h>
#include <OGRE/Ogre.h>
#include <IS/GameObject/Components/Action/AAction.h>
#include "IS/GameObject/Components/PowerUp/PowerUp.h"
#include "IS/GameObject/Components/PowerUp/PowerUpOnPlayer.h"

namespace IS
{
	namespace GameObject
	{
		namespace Component
		{
			class PowerUpOnPlayer;
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
			class APlayer : public AAction
			{
			public:
				APlayer(Ogre::SceneNode * sceneNode, IS::Scene::Scene *, int id, Dir, Ogre::Entity *entity);
				APlayer(Ogre::SceneNode * sceneNode, IS::Scene::Scene *, int id, float speed, Dir, Ogre::Entity *entity);
				virtual ~APlayer();

				virtual void update() = 0;

				IS::GameObject::Component::BombManager *getBombManager() const;
				int getNbLife() const;
				void setNbLife(int nbLife);
				bool getAlive() const;
				void setAlive(bool alive);
				void setSlow(bool slow);


			protected:
				int											_id;
				IS::GameObject::Component::BombManager		*_bombManager;
				IS::GameObject::Component::PowerUpOnPlayer	*_powerUp;
				int											_nbLife;
				bool										_isAlive;
				bool										_isSlow;
				bool										_underSlow;
				float										_slowTimer;
			};
		}
	}
}
