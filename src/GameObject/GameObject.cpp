#include <IS/GameObject/GameObject.h>

IS::GameObject::GameObject::GameObject(IS::Scene::SceneManager *sceneManager) : _sceneManager(sceneManager)
{
	_sceneNode = _sceneManager->getSceneManager()->getRootSceneNode()->createChildSceneNode();
}

IS::GameObject::GameObject::~GameObject()
{
	for(auto component : _components)
		delete component;
	_components.clear();
	Ogre::SceneNode::ObjectIterator itObject = _sceneNode->getAttachedObjectIterator();
	while (itObject.hasMoreElements())
	{
		Ogre::MovableObject* pObject = static_cast<Ogre::MovableObject *>(itObject.getNext());
		_sceneNode->getCreator()->destroyMovableObject(pObject);
	}
	_sceneManager->getSceneManager()->destroySceneNode(_sceneNode);
}

void IS::GameObject::GameObject::update()
{
	for (auto component : _components)
		component->update();
}