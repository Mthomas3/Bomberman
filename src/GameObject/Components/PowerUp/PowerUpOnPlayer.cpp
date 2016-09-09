#include "IS/GameObject/Components/PowerUp/PowerUpOnPlayer.h"
#include <IS/GameObject/Components/Bomb/BombManager.h>

IS::GameObject::Component::PowerUpOnPlayer::PowerUpOnPlayer(Ogre::SceneNode *sceneNode, IS::Scene::Scene *scene, IS::GameObject::Component::APlayer *player) : IS::GameObject::Component::AComponent(sceneNode, scene)
{
	_player = player;
	_range = 5.0f;
}

IS::GameObject::Component::PowerUpOnPlayer::~PowerUpOnPlayer()
{

}

void IS::GameObject::Component::PowerUpOnPlayer::update()
{
	checkRay();
}

void IS::GameObject::Component::PowerUpOnPlayer::checkRay()
{
	checkRayUp();
	checkRayDown();
	checkRayRight();
	checkRayLeft();
}

void IS::GameObject::Component::PowerUpOnPlayer::checkRayUp()
{
	_ray = _scene->getSceneManager()->getSceneManager()->createRayQuery(Ogre::Ray(_node->getPosition(), Ogre::Vector3(1, 0, 0)));
	_ray->setRay(_ray->getRay());
	_ray->setSortByDistance(true);
	Ogre::RaySceneQueryResult& result = _ray->execute();
	Ogre::RaySceneQueryResult::iterator it;
	for (it = result.begin(); it != result.end(); it++)
	{

		if (it->movable && it->movable->getMovableType() == "Entity")
		{
			if (it->distance > _range)
			{
				break;
			}
			if (_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode()) && _scene->getGameObjectBySceneNode(it->movable->getParentSceneNode())->getComponent<IS::GameObject::Component::PowerUp>())
			{
				IS::GameObject::Component::PowerUp *power = _scene->getGameObjectBySceneNode(it->movable->getParentSceneNode())->getComponent<IS::GameObject::Component::PowerUp>();
				if (power)
				{
					switch (power->getPowerUpEnum())
					{
					case IS::GameObject::Component::PowerUp::PowerUpEnum::SPEED:
						_player->setSpeed(_player->getSpeed() + 1);
						break;
					case IS::GameObject::Component::PowerUp::PowerUpEnum::RANGE:
						_player->getBombManager()->setRange(_player->getBombManager()->getRange() + 10.0f);
						break;
					case IS::GameObject::Component::PowerUp::PowerUpEnum::MOREBOMB:
						_player->getBombManager()->setNbBombDispo(_player->getBombManager()->getNbBombDispo() + 1);
						break;
					case IS::GameObject::Component::PowerUp::PowerUpEnum::TIME:
						_player->setSlow(true);
						break;
					case IS::GameObject::Component::PowerUp::PowerUpEnum::SLOW:
						_player->setSpeed(_player->getSpeed() - 1);
						break;
					}
					//_scene->deleteObject(_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode()));
					_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode())->getSceneNode()->setVisible(false);
					power->setPowerUpEnum(IS::GameObject::Component::PowerUp::PowerUpEnum::NONE);
					//	_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode())->removeComponent<IS::GameObject::Component::PowerUp>();
				}
			}
		}
	}
}

void IS::GameObject::Component::PowerUpOnPlayer::checkRayDown()
{
	_ray = _scene->getSceneManager()->getSceneManager()->createRayQuery(Ogre::Ray(_node->getPosition(), Ogre::Vector3(-1, 0, 0)));
	_ray->setRay(_ray->getRay());
	_ray->setSortByDistance(true);
	Ogre::RaySceneQueryResult& result = _ray->execute();
	Ogre::RaySceneQueryResult::iterator it;
	for (it = result.begin(); it != result.end(); it++)
	{
		if (it->movable && it->movable->getMovableType() == "Entity")
		{
			if (it->distance > _range)
			{
				break;
			}
			if (_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode()) && _scene->getGameObjectBySceneNode(it->movable->getParentSceneNode())->getComponent<IS::GameObject::Component::PowerUp>())
			{
				IS::GameObject::Component::PowerUp *power = _scene->getGameObjectBySceneNode(it->movable->getParentSceneNode())->getComponent<IS::GameObject::Component::PowerUp>();
				if (power)
				{
					switch (power->getPowerUpEnum())
					{
					case IS::GameObject::Component::PowerUp::PowerUpEnum::SPEED:
						_player->setSpeed(_player->getSpeed() + 1);
						break;
					case IS::GameObject::Component::PowerUp::PowerUpEnum::RANGE:
						_player->getBombManager()->setRange(_player->getBombManager()->getRange() + 50.0f);
						break;
					case IS::GameObject::Component::PowerUp::PowerUpEnum::MOREBOMB:
						_player->getBombManager()->setNbBombDispo(_player->getBombManager()->getNbBombDispo() + 1);
						break;
					case IS::GameObject::Component::PowerUp::PowerUpEnum::TIME:
						_player->setSlow(true);
						break;
					case IS::GameObject::Component::PowerUp::PowerUpEnum::SLOW:
						_player->setSpeed(_player->getSpeed() - 1);
						break;
					}
					//_scene->deleteObject(_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode()));
					_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode())->getSceneNode()->setVisible(false);
					power->setPowerUpEnum(IS::GameObject::Component::PowerUp::PowerUpEnum::NONE);

					//_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode())->removeComponent<IS::GameObject::Component::PowerUp>();
				}
			}
		}
	}
}

void IS::GameObject::Component::PowerUpOnPlayer::checkRayLeft()
{
	_ray = _scene->getSceneManager()->getSceneManager()->createRayQuery(Ogre::Ray(_node->getPosition(), Ogre::Vector3(0, 0, 1)));
	_ray->setRay(_ray->getRay());
	_ray->setSortByDistance(true);
	Ogre::RaySceneQueryResult& result = _ray->execute();
	Ogre::RaySceneQueryResult::iterator it;
	for (it = result.begin(); it != result.end(); it++)
	{
		if (it->movable && it->movable->getMovableType() == "Entity")
		{
			if (it->distance > _range)
			{
				break;
			}
			if (_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode()) && _scene->getGameObjectBySceneNode(it->movable->getParentSceneNode())->getComponent<IS::GameObject::Component::PowerUp>())
			{
				IS::GameObject::Component::PowerUp *power = _scene->getGameObjectBySceneNode(it->movable->getParentSceneNode())->getComponent<IS::GameObject::Component::PowerUp>();
				if (power)
				{
					switch (power->getPowerUpEnum())
					{
					case IS::GameObject::Component::PowerUp::PowerUpEnum::SPEED:
						_player->setSpeed(_player->getSpeed() + 1);
						break;
					case IS::GameObject::Component::PowerUp::PowerUpEnum::RANGE:
						_player->getBombManager()->setRange(_player->getBombManager()->getRange() + 50.0f);
						break;
					case IS::GameObject::Component::PowerUp::PowerUpEnum::MOREBOMB:
						_player->getBombManager()->setNbBombDispo(_player->getBombManager()->getNbBombDispo() + 1);
						break;
					case IS::GameObject::Component::PowerUp::PowerUpEnum::TIME:
						_player->setSlow(true);
						break;
					case IS::GameObject::Component::PowerUp::PowerUpEnum::SLOW:
						_player->setSpeed(_player->getSpeed() - 1);
						break;
					}
					//_scene->deleteObject(_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode()));
					_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode())->getSceneNode()->setVisible(false);
					power->setPowerUpEnum(IS::GameObject::Component::PowerUp::PowerUpEnum::NONE);

//					_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode())->removeComponent<IS::GameObject::Component::PowerUp>();
				}
			}
		}
	}
}

void IS::GameObject::Component::PowerUpOnPlayer::checkRayRight()
{
	_ray = _scene->getSceneManager()->getSceneManager()->createRayQuery(Ogre::Ray(_node->getPosition(), Ogre::Vector3(0, 0, -1)));
	_ray->setRay(_ray->getRay());
	_ray->setSortByDistance(true);
	Ogre::RaySceneQueryResult& result = _ray->execute();
	Ogre::RaySceneQueryResult::iterator it;
	for (it = result.begin(); it != result.end(); it++)
	{
		if (it->movable && it->movable->getMovableType() == "Entity")
		{
			if (it->distance > _range)
			{
				break;
			}
			if (_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode()) && _scene->getGameObjectBySceneNode(it->movable->getParentSceneNode())->getComponent<IS::GameObject::Component::PowerUp>())
			{
				IS::GameObject::Component::PowerUp *power = _scene->getGameObjectBySceneNode(it->movable->getParentSceneNode())->getComponent<IS::GameObject::Component::PowerUp>();
				if (power)
				{
					switch (power->getPowerUpEnum())
					{
					case IS::GameObject::Component::PowerUp::PowerUpEnum::SPEED:
						_player->setSpeed(_player->getSpeed() + 0.5);
						break;
					case IS::GameObject::Component::PowerUp::PowerUpEnum::RANGE:
						_player->getBombManager()->setRange(_player->getBombManager()->getRange() + 50.0f);
						break;
					case IS::GameObject::Component::PowerUp::PowerUpEnum::MOREBOMB:
						_player->getBombManager()->setNbBombDispo(_player->getBombManager()->getNbBombDispo() + 1);
						break;
					case IS::GameObject::Component::PowerUp::PowerUpEnum::TIME:
						_player->setSlow(true);
						break;
					case IS::GameObject::Component::PowerUp::PowerUpEnum::SLOW:
						_player->setSpeed(_player->getSpeed() - 0.5);
						break;
					}
					const IS::Map::unitPos &&uPos = _scene->getMap()->ogreToMap(IS::Map::realPos(_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode())->getSceneNode()->getPosition().x, _scene->getGameObjectBySceneNode(it->movable->getParentSceneNode())->getSceneNode()->getPosition().z));
					const IS::Map::realPos &&pair = _scene->getMap()->mapToOgre(uPos);
					_scene->getMap()->setAt(pair, Map::TileType::None);
					//_scene->deleteObject(_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode()));
					_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode())->getSceneNode()->setVisible(false);
					power->setPowerUpEnum(IS::GameObject::Component::PowerUp::PowerUpEnum::NONE);

//					_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode())->removeComponent<IS::GameObject::Component::PowerUp>();
				}
			}
		}
	}
}
