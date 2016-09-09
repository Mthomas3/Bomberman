#include <IS/GameObject/Components/DestroyPhysic/DestroyPhysicDynamic.h>

IS::GameObject::Component::DestroyPhysicDynamic::DestroyPhysicDynamic(Ogre::SceneNode * sceneNode, IS::Scene::Scene *scene, IS::Physics::DynamicObject *physic) : IS::GameObject::Component::AComponent(sceneNode, scene)
{
	_physic = physic;
}

IS::GameObject::Component::DestroyPhysicDynamic::~DestroyPhysicDynamic()
{
	delete _physic;
}

void IS::GameObject::Component::DestroyPhysicDynamic::update()
{

}

