#include <IS/GameObject/Components/DestroyComponent/DestroyComponent.h>

IS::GameObject::Component::DestroyComponent::DestroyComponent(Ogre::SceneNode * sceneNode, IS::Scene::Scene *scene, Ogre::SceneManager *sceneManager) : IS::GameObject::Component::AComponent(sceneNode, scene)
{
	_timer = 0.3f;
}

IS::GameObject::Component::DestroyComponent::~DestroyComponent()
{

}

void IS::GameObject::Component::DestroyComponent::update()
{
	_timer -= IS::Utils::Time::getInstance()->getDeltaTime();
	if (_timer < 0.0f)
	{
		_node->setVisible(false);
		//_scene->deleteObject(_scene->getGameObjectBySceneNode(_node));
	}
}