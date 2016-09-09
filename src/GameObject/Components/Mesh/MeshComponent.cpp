#include <IS/GameObject/Components/Mesh/MeshComponent.h>

IS::GameObject::Component::MeshComponent::MeshComponent(Ogre::SceneNode * sceneNode, IS::Scene::Scene *scene, Ogre::SceneManager *sceneManager, const std::string &name, const std::string &path) : IS::GameObject::Component::AComponent(sceneNode, scene), _sceneManager(sceneManager)
{
	_entity = _sceneManager->createEntity(name, path);
	_node->attachObject(_entity);
}

IS::GameObject::Component::MeshComponent::~MeshComponent()
{
	_sceneManager->destroyEntity(_entity);
}

void IS::GameObject::Component::MeshComponent::update()
{
}