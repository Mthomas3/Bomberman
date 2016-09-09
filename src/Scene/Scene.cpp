#include <IS/Scene/Scene.h>
#include <IS/input/Input.hh>

IS::Scene::Scene::Scene(Ogre::Root *root, Ogre::RenderWindow *renderWindow) : _root(root), _renderWindow(renderWindow)
{
	_sceneManager = new IS::Scene::SceneManager(root);
	_camera = new IS::Scene::Camera(_sceneManager->getSceneManager(), _renderWindow);
}

IS::Scene::Scene::~Scene()
{
	_sceneManager->getSceneManager()->clearScene();
	delete _sceneManager;
	delete _camera;
}

void IS::Scene::Scene::addObject(IS::GameObject::GameObject * gameObject)
{
	_gameObjects.push_back(gameObject);
}

void IS::Scene::Scene::deleteObject(IS::GameObject::GameObject * gameObject)
{
	if (!gameObject)
		return;
	for (auto it = _gameObjects.begin(); it != _gameObjects.end(); it++)
		if (*it == gameObject)
		{
			auto *ptr = *it;
			_gameObjects.erase(it);
			delete ptr;
			break;
		}
}

IS::Scene::SceneManager *IS::Scene::Scene::getSceneManager() const
{
	return _sceneManager;
}

IS::Scene::Camera * IS::Scene::Scene::getCamera() const
{
	return _camera;
}

std::shared_ptr<IS::Map> IS::Scene::Scene::getMap() const
{
	return _map;
}

void IS::Scene::Scene::setMap(std::shared_ptr<IS::Map> map)
{
	_map = map;
}

void IS::Scene::Scene::ResetAll()
{
	delete _camera;
}
