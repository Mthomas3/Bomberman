#include <IS/Scene/SceneManager.h>

IS::Scene::SceneManager::SceneManager(Ogre::Root *root, const std::string &name) : _root(root)
{
	_sceneManager = _root->createSceneManager(name, name);
	_sceneManager->setAmbientLight(Ogre::ColourValue(1.0F, 1.0f, 1.0f));
}

IS::Scene::SceneManager::~SceneManager()
{
	_sceneManager->destroyAllCameras();
	_root->destroySceneManager(_sceneManager);
}

void IS::Scene::SceneManager::setAmbiantLight(float r, float g, float b)
{
	_sceneManager->setAmbientLight(Ogre::ColourValue(r, g, b));
}
