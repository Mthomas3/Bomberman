#include <IS/GameObject/Components/DestroyPhysic/DestroyPhysic.h>

IS::GameObject::Component::DestroyPhysic::DestroyPhysic(Ogre::SceneNode * sceneNode, IS::Scene::Scene *scene, IS::Physics::StaticObject *physic) : IS::GameObject::Component::AComponent(sceneNode, scene)
{
	_physic = physic;
}

IS::GameObject::Component::DestroyPhysic::~DestroyPhysic()
{
	delete _physic;
}

void IS::GameObject::Component::DestroyPhysic::update()
{

}

