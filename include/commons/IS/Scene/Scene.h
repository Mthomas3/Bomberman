#pragma once

#include <vector>
#include <IS/Scene/SceneManager.h>
#include <IS/Scene/Camera.h>
#include <IS/GameObject/GameObject.h>
#include <IS/Map/Map.hpp>

namespace IS
{
	namespace Scene
	{
		class Scene
		{
		public:
			Scene(Ogre::Root *root, Ogre::RenderWindow *renderWindow);
			virtual ~Scene();

			virtual void update() { _camera->update(); for (auto gameObject : _gameObjects) gameObject->update(); }

			void	addObject(IS::GameObject::GameObject *gameObject);
			void	deleteObject(IS::GameObject::GameObject *gameObject);

			IS::Scene::SceneManager	*getSceneManager() const;

			GameObject::GameObject	*getGameObjectBySceneNode(Ogre::SceneNode *node) const
			{
				for (IS::GameObject::GameObject *object : _gameObjects)
					if (*object == node)
						return object;
				return nullptr;
			}
			IS::Scene::Camera *getCamera() const;
			std::shared_ptr<IS::Map> getMap() const;
			void setMap(std::shared_ptr<IS::Map> map);
			void ResetAll();
		protected:
			IS::Scene::SceneManager						*_sceneManager;
			IS::Scene::Camera							*_camera;
			Ogre::Root									*_root;
			Ogre::RenderWindow							*_renderWindow;
			std::vector<IS::GameObject::GameObject *>	_gameObjects;
			std::shared_ptr<IS::Map>					    _map;
		};
	}
}