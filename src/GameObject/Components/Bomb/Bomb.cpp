#include "IS/GameObject/Components/Bomb/Bomb.h"
#include <IS/Audio/Audio.h>
#include <IS/Audio/Manager.h>
#include <IS/GUI/GUI.hh>


IS::GameObject::Component::Bomb::Bomb(Ogre::SceneNode *sceneNode, IS::Scene::Scene *scene, IS::GameObject::Component::BombManager *bomManager, float range) : IS::GameObject::Component::AComponent(sceneNode, scene)
{
	_range = range;
	std::string name;
	name = _node->getName() + "Particule";
	_explodeRangeUp = range;
	_explodeRangeDown = range;
	_explodeRangeLeft = range;
	_explodeRangeRight = range;
	_timer = 4.0f;
	_decal = 30;
	_caseSize = 50.0f;
	_expodeMeshSize = 25.0f;
	_bombManager = bomManager;
	_id = 0;
	_bombPhysics = IS::Physics::newPhysicObject<IS::Physics::StaticObject>(_node, _scene, IS::Physics::CollisionType::COLLISION_ACCURATE);
}

IS::GameObject::Component::Bomb::~Bomb()
{
	delete _bombPhysics;
	_bombManager->setNbBombDispo(_bombManager->getNbBombDispo() + 1);
}

void IS::GameObject::Component::Bomb::update()
{
	_timer -= IS::Utils::Time::getInstance()->getDeltaTime();
	if (_timer < 0.0f)
	{
		IS::Audio::AudioManager::getInstance()->playSound("Media/audio/effects/bomb_explosion.flac", 50);
		_node->setVisible(false);
		_posUp = _node->getPosition().x;
		_posDown = _node->getPosition().x;
		_posRight = _node->getPosition().z;
		_posLeft = _node->getPosition().z;
		_name = _node->getName() + "_explo_" + std::to_string(_id++);
		IS::GameObject::GameObject *object = new IS::GameObject::GameObject(_scene->getSceneManager());
		object->getSceneNode()->setScale(Ogre::Vector3(_expodeMeshSize));
		object->getSceneNode()->setPosition(_node->getPosition());
		IS::GameObject::Component::MeshComponent *mesh = new IS::GameObject::Component::MeshComponent(object->getSceneNode(), _scene, _scene->getSceneManager()->getSceneManager(), _name, "explosion_center.mesh");
		mesh->getEntity()->getSubEntity(0)->setMaterialName("OceanHLSL_GLSL");
		object->addComponent<IS::GameObject::Component::MeshComponent>(mesh);
		IS::GameObject::Component::DestroyComponent *destroy = new IS::GameObject::Component::DestroyComponent(object->getSceneNode(), _scene, _scene->getSceneManager()->getSceneManager());
		object->addComponent<IS::GameObject::Component::DestroyComponent>(destroy);
		_scene->addObject(object);
		const IS::Map::unitPos &&uPos = _scene->getMap()->ogreToMap(IS::Map::realPos(object->getSceneNode()->getPosition().x, object->getSceneNode()->getPosition().z));
		const IS::Map::realPos &&pair = _scene->getMap()->mapToOgre(uPos);
		_scene->getMap()->setAt(pair, Map::TileType::None);

		checkRay();
		if (_explodeRangeUp > _range)
			_explodeRangeUp = _range;
		if (_explodeRangeDown > _range)
			_explodeRangeDown = _range;
		if (_explodeRangeLeft > _range)
			_explodeRangeLeft = _range;
		if (_explodeRangeRight > _range)
			_explodeRangeRight = _range;
		while (_explodeRangeUp > _caseSize || _explodeRangeDown > _caseSize)
		{
			_name = _node->getName() + "_explo_" + std::to_string(_id++);
			if (_explodeRangeUp > _caseSize)
			{
				IS::GameObject::GameObject *object = new IS::GameObject::GameObject(_scene->getSceneManager());
				object->getSceneNode()->setScale(Ogre::Vector3(_expodeMeshSize));
				_posUp += _caseSize;
				object->getSceneNode()->setPosition(_node->getPosition());
				object->getSceneNode()->setPosition(Ogre::Vector3(_posUp , object->getSceneNode()->getPosition().y, object->getSceneNode()->getPosition().z));
				IS::GameObject::Component::MeshComponent *mesh = new IS::GameObject::Component::MeshComponent(object->getSceneNode(), _scene, _scene->getSceneManager()->getSceneManager(), _name, "explosion_arrow_red.mesh");
				object->addComponent<IS::GameObject::Component::MeshComponent>(mesh);
				IS::GameObject::Component::DestroyComponent *destroy = new IS::GameObject::Component::DestroyComponent(object->getSceneNode(), _scene, _scene->getSceneManager()->getSceneManager());
				object->addComponent<IS::GameObject::Component::DestroyComponent>(destroy);

				_scene->addObject(object);
				_explodeRangeUp -= _caseSize;
			}
			_name = _node->getName() + "_explo_" + std::to_string(_id++);
			if (_explodeRangeDown > _caseSize)
			{
				IS::GameObject::GameObject *object = new IS::GameObject::GameObject(_scene->getSceneManager());
				object->getSceneNode()->setScale(Ogre::Vector3(_expodeMeshSize));
				object->getSceneNode()->setPosition(_node->getPosition());
				object->getSceneNode()->setDirection(0, 0, 1);
				_posDown -= _caseSize;
				object->getSceneNode()->setPosition(Ogre::Vector3(_posDown , object->getSceneNode()->getPosition().y, object->getSceneNode()->getPosition().z));
				IS::GameObject::Component::MeshComponent *mesh = new IS::GameObject::Component::MeshComponent(object->getSceneNode(), _scene, _scene->getSceneManager()->getSceneManager(), _name, "explosion_arrow_red.mesh");
				object->addComponent<IS::GameObject::Component::MeshComponent>(mesh);
				IS::GameObject::Component::DestroyComponent *destroy = new IS::GameObject::Component::DestroyComponent(object->getSceneNode(), _scene, _scene->getSceneManager()->getSceneManager());
				object->addComponent<IS::GameObject::Component::DestroyComponent>(destroy);
				_scene->addObject(object);
				_explodeRangeDown -= _caseSize;
			}
			_name = _node->getName() + "_explo_" + std::to_string(_id++);
			if (_explodeRangeLeft > _caseSize)
			{
				IS::GameObject::GameObject *object = new IS::GameObject::GameObject(_scene->getSceneManager());
				object->getSceneNode()->setScale(Ogre::Vector3(_expodeMeshSize));
				object->getSceneNode()->setDirection(1, 0, 0);
				_posLeft += _caseSize;
				object->getSceneNode()->setPosition(_node->getPosition());
				object->getSceneNode()->setPosition(Ogre::Vector3(object->getSceneNode()->getPosition().x , object->getSceneNode()->getPosition().y, _posLeft));
				IS::GameObject::Component::MeshComponent *mesh = new IS::GameObject::Component::MeshComponent(object->getSceneNode(), _scene, _scene->getSceneManager()->getSceneManager(), _name, "explosion_arrow_red.mesh");
				object->addComponent<IS::GameObject::Component::MeshComponent>(mesh);
				IS::GameObject::Component::DestroyComponent *destroy = new IS::GameObject::Component::DestroyComponent(object->getSceneNode(), _scene, _scene->getSceneManager()->getSceneManager());
				object->addComponent<IS::GameObject::Component::DestroyComponent>(destroy);

				_scene->addObject(object);
				_explodeRangeLeft -= _caseSize;
			}
			_name = _node->getName() + "_explo_" + std::to_string(_id++);
			if (_explodeRangeRight > _caseSize)
			{
				IS::GameObject::GameObject *object = new IS::GameObject::GameObject(_scene->getSceneManager());
				object->getSceneNode()->setScale(Ogre::Vector3(_expodeMeshSize));
				object->getSceneNode()->setPosition(_node->getPosition());
				object->getSceneNode()->setDirection(-1, 0, 0);
				_posRight -= _caseSize;
				object->getSceneNode()->setPosition(Ogre::Vector3(object->getSceneNode()->getPosition().x , object->getSceneNode()->getPosition().y, _posRight));
				IS::GameObject::Component::MeshComponent *mesh = new IS::GameObject::Component::MeshComponent(object->getSceneNode(), _scene, _scene->getSceneManager()->getSceneManager(), _name, "explosion_arrow_red.mesh");
				object->addComponent<IS::GameObject::Component::MeshComponent>(mesh);
				IS::GameObject::Component::DestroyComponent *destroy = new IS::GameObject::Component::DestroyComponent(object->getSceneNode(), _scene, _scene->getSceneManager()->getSceneManager());
				object->addComponent<IS::GameObject::Component::DestroyComponent>(destroy);
				_scene->addObject(object);
				_explodeRangeRight -= _caseSize;
			}
		}
		_bombManager->bombToDestroy(this);
		
	}
}

Ogre::SceneNode * IS::GameObject::Component::Bomb::getNode()
{
	return _node;
}

IS::Scene::Scene * IS::GameObject::Component::Bomb::getScene()
{
	return _scene;
}

void IS::GameObject::Component::Bomb::checkRay()
{
	checkRayUp();
	checkRayDown();
	checkRayRight();
	checkRayLeft();
}

void IS::GameObject::Component::Bomb::checkRayUp()
{
	_rayUp = _scene->getSceneManager()->getSceneManager()->createRayQuery(Ogre::Ray(_node->getPosition(), Ogre::Vector3(1, 0, 0)));
	_rayUp->setRay(_rayUp->getRay());
	_rayUp->setSortByDistance(true);
	Ogre::RaySceneQueryResult& result = _rayUp->execute();
	Ogre::RaySceneQueryResult::iterator it;
	for (it = result.begin(); it != result.end(); it++)
	{
		if (it->movable && it->movable->getMovableType() == "Entity")
		{
			std::cout << it->movable->getName() << std::endl;
			if (it->distance > _range || (it->movable->getName()[0] == 'B' && it->movable->getName()[1] == 'o'))
			{
				_explodeRangeUp = it->distance;
				break;
			}
			if (_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode()) && _scene->getGameObjectBySceneNode(it->movable->getParentSceneNode())->getComponent<IS::GameObject::Component::PowerUp>())
				continue;
			if (_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode()) && _scene->getGameObjectBySceneNode(it->movable->getParentSceneNode())->getComponent<IS::GameObject::Component::Player>())
			{
				_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode())->getComponent<IS::GameObject::Component::Player>()->setAlive(false);
			}
			else
			{
				if ((it->movable->getName()[0] == 'B' && it->movable->getName()[1] == 'l'))
				{
					const IS::Map::unitPos &&uPos = _scene->getMap()->ogreToMap(IS::Map::realPos(it->movable->getParentSceneNode()->getPosition().x, it->movable->getParentSceneNode()->getPosition().z));
					const IS::Map::realPos &&pair = _scene->getMap()->mapToOgre(uPos);
					_scene->getMap()->setAt(pair, Map::TileType::None);
					_explodeRangeUp = it->distance + _caseSize;
					_scene->deleteObject(_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode()));
					break;
				}
				_scene->deleteObject(_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode()));
			}
		}
	}
}

void IS::GameObject::Component::Bomb::checkRayDown()
{
	_rayUp = _scene->getSceneManager()->getSceneManager()->createRayQuery(Ogre::Ray(_node->getPosition(), Ogre::Vector3(-1, 0, 0)));
	_rayUp->setRay(_rayUp->getRay());
	_rayUp->setSortByDistance(true);
	Ogre::RaySceneQueryResult& result = _rayUp->execute();
	Ogre::RaySceneQueryResult::iterator it;
	for (it = result.begin(); it != result.end(); it++)
	{
		if (it->movable && it->movable->getMovableType() == "Entity")
		{
			std::cout << it->movable->getName() << std::endl;
			if (it->distance > _range || (it->movable->getName()[0] == 'B' && it->movable->getName()[1] == 'o'))
			{
				_explodeRangeDown = it->distance;
				break;
			}
			if (_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode()) && _scene->getGameObjectBySceneNode(it->movable->getParentSceneNode())->getComponent<IS::GameObject::Component::PowerUp>())
				continue;
			if (_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode()) && _scene->getGameObjectBySceneNode(it->movable->getParentSceneNode())->getComponent<IS::GameObject::Component::Player>())
			{
				_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode())->getComponent<IS::GameObject::Component::Player>()->setAlive(false);
			}
			else
			{
				if ((it->movable->getName()[0] == 'B' && it->movable->getName()[1] == 'l'))
				{
					const IS::Map::unitPos &&uPos = _scene->getMap()->ogreToMap(IS::Map::realPos(it->movable->getParentSceneNode()->getPosition().x, it->movable->getParentSceneNode()->getPosition().z));
					const IS::Map::realPos &&pair = _scene->getMap()->mapToOgre(uPos);
					_scene->getMap()->setAt(pair, Map::TileType::None);
					_explodeRangeDown = it->distance + _caseSize;
					_scene->deleteObject(_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode()));
					break;
				}
				_scene->deleteObject(_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode()));
			}
		}
	}
}

void IS::GameObject::Component::Bomb::checkRayLeft()
{
	_rayUp = _scene->getSceneManager()->getSceneManager()->createRayQuery(Ogre::Ray(_node->getPosition(), Ogre::Vector3(0, 0, 1)));
	_rayUp->setRay(_rayUp->getRay());
	_rayUp->setSortByDistance(true);
	Ogre::RaySceneQueryResult& result = _rayUp->execute();
	Ogre::RaySceneQueryResult::iterator it;
	for (it = result.begin(); it != result.end(); it++)
	{
		if (it->movable && it->movable->getMovableType() == "Entity")
		{
			std::cout << it->movable->getName() << std::endl;
			if (it->distance > _range || (it->movable->getName()[0] == 'B' && it->movable->getName()[1] == 'o'))
			{
				_explodeRangeLeft = it->distance;
				break;
			}
			if (_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode()) && _scene->getGameObjectBySceneNode(it->movable->getParentSceneNode())->getComponent<IS::GameObject::Component::PowerUp>())
				continue;
			if (_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode()) && _scene->getGameObjectBySceneNode(it->movable->getParentSceneNode())->getComponent<IS::GameObject::Component::Player>())
			{
				_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode())->getComponent<IS::GameObject::Component::Player>()->setAlive(false);
			}
			else
			{
				if ((it->movable->getName()[0] == 'B' && it->movable->getName()[1] == 'l'))
				{
					const IS::Map::unitPos &&uPos = _scene->getMap()->ogreToMap(IS::Map::realPos(it->movable->getParentSceneNode()->getPosition().x, it->movable->getParentSceneNode()->getPosition().z));
					const IS::Map::realPos &&pair = _scene->getMap()->mapToOgre(uPos);
					_scene->getMap()->setAt(pair, Map::TileType::None);
					_explodeRangeLeft = it->distance + _caseSize;
					_scene->deleteObject(_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode()));
					break;
				}
				_scene->deleteObject(_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode()));
			}
		}
	}
}

void IS::GameObject::Component::Bomb::checkRayRight()
{
	bool done = false;
	_rayUp = _scene->getSceneManager()->getSceneManager()->createRayQuery(Ogre::Ray(_node->getPosition(), Ogre::Vector3(0, 0, -1)));
	_rayUp->setRay(_rayUp->getRay());
	_rayUp->setSortByDistance(true);
	Ogre::RaySceneQueryResult& result = _rayUp->execute();
	Ogre::RaySceneQueryResult::iterator it;
	for (it = result.begin(); it != result.end(); it++)
	{
		if (it->movable && it->movable->getMovableType() == "Entity")
		{
			std::cout << it->movable->getName() << std::endl;
			if (it->distance > _range || (it->movable->getName()[0] == 'B' && it->movable->getName()[1] == 'o'))
			{
				_explodeRangeRight = it->distance;
				break;
			}
			if (_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode()) && _scene->getGameObjectBySceneNode(it->movable->getParentSceneNode())->getComponent<IS::GameObject::Component::PowerUp>())
				continue;
			if (_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode()) && _scene->getGameObjectBySceneNode(it->movable->getParentSceneNode())->getComponent<IS::GameObject::Component::Player>())
			{
				_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode())->getComponent<IS::GameObject::Component::Player>()->setAlive(false);
			}
			else
			{
				if ((it->movable->getName()[0] == 'B' && it->movable->getName()[1] == 'l'))
				{
					const IS::Map::unitPos &&uPos = _scene->getMap()->ogreToMap(IS::Map::realPos(it->movable->getParentSceneNode()->getPosition().x, it->movable->getParentSceneNode()->getPosition().z));
					const IS::Map::realPos &&pair = _scene->getMap()->mapToOgre(uPos);
					_scene->getMap()->setAt(pair, Map::TileType::None);
					_explodeRangeRight = it->distance + _caseSize;
					_scene->deleteObject(_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode()));
					break;
				}
				_scene->deleteObject(_scene->getGameObjectBySceneNode(it->movable->getParentSceneNode()));
			}
		}
	}
}
