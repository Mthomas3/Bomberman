#include "IS/GameObject/Components/Bomb/BombManager.h"

IS::GameObject::Component::BombManager::BombManager(Ogre::SceneNode *sceneNode, IS::Scene::Scene *scene, float range) : IS::GameObject::Component::AComponent(sceneNode, scene)
{
	_name = "";
	_id = 0;
	_range = range;
	_nbBombDispo = 1;
}

IS::GameObject::Component::BombManager::~BombManager()
{

}

void IS::GameObject::Component::BombManager::update()
{

}

void IS::GameObject::Component::BombManager::createBomb()
{
	if (_nbBombDispo > 0)
	{
		IS::GameObject::GameObject *object = new IS::GameObject::GameObject(_scene->getSceneManager());
		_name = _node->getName() + "_bomb_" + std::to_string(_id);
		IS::GameObject::Component::MeshComponent *mesh = new IS::GameObject::Component::MeshComponent(object->getSceneNode(), _scene, _scene->getSceneManager()->getSceneManager(), _name, "bomb_default.mesh");
		mesh->getEntity()->getSubEntity(0)->setMaterialName("OceanHLSL_GLSL");
		object->addComponent<IS::GameObject::Component::MeshComponent>(mesh);
		object->getSceneNode()->setScale(Ogre::Vector3(35.5));
		const IS::Map::unitPos &&uPos = _scene->getMap()->ogreToMap(IS::Map::realPos(_node->getPosition().x, _node->getPosition().z));
		const IS::Map::realPos &&pair = _scene->getMap()->mapToOgre(uPos);
		object->getSceneNode()->setPosition(pair.first , 0, pair.second);
		_scene->addObject(object);
		//_scene->getMap()->setAt(pair, Map::TileType::Bomb);
		IS::GameObject::Component::Bomb *bomb = new IS::GameObject::Component::Bomb(object->getSceneNode(), _scene, this, _range);
		object->addComponent<IS::GameObject::Component::Bomb>(bomb);
		_id++;
		_nbBombDispo--;
	}
}

int IS::GameObject::Component::BombManager::getNbBombDispo() const
{
	return _nbBombDispo;
}

void IS::GameObject::Component::BombManager::setNbBombDispo(int nb)
{
	_nbBombDispo = nb;
}

float IS::GameObject::Component::BombManager::getRange() const
{
	return _range;
}

void IS::GameObject::Component::BombManager::setRange(float range)
{
	_range = range;
}

void IS::GameObject::Component::BombManager::bombToDestroy(IS::GameObject::Component::Bomb *bomb)
{
	_scene->deleteObject(bomb->getScene()->getGameObjectBySceneNode(bomb->getNode()));
}
