#include "IS/GameObject/Components/PowerUp/PowerUpOnCrate.h"
#include "IS/GameObject/Components/PowerUp/PowerUp.h"

IS::GameObject::Component::PowerUpOnCrate::PowerUpOnCrate(Ogre::SceneNode *sceneNode, IS::Scene::Scene *scene) : IS::GameObject::Component::AComponent(sceneNode, scene)
{
}

IS::GameObject::Component::PowerUpOnCrate::~PowerUpOnCrate()
{
	if (std::rand() % 2 == 1)
	{
		std::string name = _node->getName() + "_powerUp_";

		IS::GameObject::GameObject *object2 = new IS::GameObject::GameObject(_scene->getSceneManager());
		object2->getSceneNode()->setPosition(_node->getPosition());
		object2->getSceneNode()->setScale(Ogre::Vector3(50.0f));
		IS::GameObject::Component::MeshComponent *mesh2 = nullptr;
		int rd = std::rand() % 5;
		IS::GameObject::Component::PowerUp *powerUp = nullptr;
		switch (rd)
		{
			case 0 :
				object2->getSceneNode()->setScale(Ogre::Vector3(20.0f));
				mesh2 = new IS::GameObject::Component::MeshComponent(object2->getSceneNode(), _scene, _scene->getSceneManager()->getSceneManager(), name, "bonus_speed_up.mesh");
				powerUp = new IS::GameObject::Component::PowerUp(object2->getSceneNode(), _scene, IS::GameObject::Component::PowerUp::PowerUpEnum::SPEED);
				break;
			case 1:
				mesh2 = new IS::GameObject::Component::MeshComponent(object2->getSceneNode(), _scene, _scene->getSceneManager()->getSceneManager(), name, "bonus_multibomb.mesh");
				powerUp = new IS::GameObject::Component::PowerUp(object2->getSceneNode(), _scene, IS::GameObject::Component::PowerUp::PowerUpEnum::MOREBOMB);
				break;
			case 2:
				mesh2 = new IS::GameObject::Component::MeshComponent(object2->getSceneNode(), _scene, _scene->getSceneManager()->getSceneManager(), name, "bonus_fireup.mesh");
				powerUp = new IS::GameObject::Component::PowerUp(object2->getSceneNode(), _scene, IS::GameObject::Component::PowerUp::PowerUpEnum::RANGE);
				break;
			case 3:
				object2->getSceneNode()->setScale(Ogre::Vector3(20.0f));
				mesh2 = new IS::GameObject::Component::MeshComponent(object2->getSceneNode(), _scene, _scene->getSceneManager()->getSceneManager(), name, "bonus_speed_down.mesh");
				powerUp = new IS::GameObject::Component::PowerUp(object2->getSceneNode(), _scene, IS::GameObject::Component::PowerUp::PowerUpEnum::SLOW);
				break;
			case 4:
				mesh2 = new IS::GameObject::Component::MeshComponent(object2->getSceneNode(), _scene, _scene->getSceneManager()->getSceneManager(), name, "bonus_time.mesh");
				powerUp = new IS::GameObject::Component::PowerUp(object2->getSceneNode(), _scene, IS::GameObject::Component::PowerUp::PowerUpEnum::TIME);
				break;
		}
		const IS::Map::unitPos &&uPos = _scene->getMap()->ogreToMap(IS::Map::realPos(object2->getSceneNode()->getPosition().x, object2->getSceneNode()->getPosition().z));
		const IS::Map::realPos &&pair = _scene->getMap()->mapToOgre(uPos);
		//_scene->getMap()->setAt(pair, Map::TileType::Powerup);
		object2->addComponent<IS::GameObject::Component::MeshComponent>(mesh2);
		object2->addComponent<IS::GameObject::Component::PowerUp>(powerUp);
		_scene->addObject(object2);
	}
}

void IS::GameObject::Component::PowerUpOnCrate::update()
{

}