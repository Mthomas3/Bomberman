#pragma once

#include <map>
#include <IS/GameObject/IGameObject.h>
#include <IS/Scene/SceneManager.h>
#include <IS/GameObject/Components/IComponent.h>

namespace IS
{
	namespace GameObject
	{
		class GameObject : public IGameObject
		{
		public:
			GameObject(IS::Scene::SceneManager *_sceneManager);
			virtual ~GameObject();

			template <class T>
			void			addComponent(T *component);

			virtual Ogre::SceneNode	*getSceneNode() const { return _sceneNode; }
			
			virtual void	update();

			bool	operator==(Ogre::SceneNode *node) { return _sceneNode == node; }

			template <class T>
			T	*getComponent() const;

			template <class T>
			bool	removeComponent()
			{
				try
				{
					if (_mapComponents.find(typeid(T).name()) == _mapComponents.end())
						return false;
					std::vector<IS::GameObject::Component::IComponent *> &vec = _mapComponents[typeid(T).name()];
					for (IS::GameObject::Component::IComponent *lol : vec)
					{
						_components.erase(std::remove(_components.begin(), _components.end(), lol), _components.end());
						delete lol;
					}
					vec.clear();
				}
				catch (...)
				{
					return false;
				}
				return true;
			}

		private:
			IS::Scene::SceneManager														*_sceneManager;
			Ogre::SceneNode																*_sceneNode;
			std::vector<IS::GameObject::Component::IComponent *>						_components;
			std::map<std::string, std::vector<IS::GameObject::Component::IComponent *>>	_mapComponents;
		};

		template<class T>
		inline void GameObject::addComponent(T *component)
		{
			_components.push_back(reinterpret_cast<IS::GameObject::Component::IComponent *>(component));
			_mapComponents[typeid(T).name()].push_back(component);
		}

		template<class T>
		inline T *GameObject::getComponent() const
		{
			if (_mapComponents.find(typeid(T).name()) == _mapComponents.end())
				return nullptr;
			return (reinterpret_cast<T *>(_mapComponents.at(typeid(T).name())[0]));
		}
	}
}