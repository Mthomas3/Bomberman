#pragma once

#include <OGRE/Ogre.h>
#include "IComponent.h"
#include "IS/Scene/Scene.h"

namespace IS
{
	namespace Scene
	{
		class Scene;
	}
}

namespace IS
{
	namespace GameObject
	{
		namespace Component
		{
			class AComponent : public IComponent
			{
			public:
				AComponent(Ogre::SceneNode *node, Scene::Scene *scene) : _node(node), _scene(scene) { }
				virtual ~AComponent() { }

				void	update() = 0;

			protected:
				Ogre::SceneNode		*_node;
				IS::Scene::Scene	*_scene;
			};
		}
	}
}