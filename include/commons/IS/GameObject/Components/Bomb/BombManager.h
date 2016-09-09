#pragma once

#include <IS/GameObject/Components/AComponent.h>
#include <OGRE/Ogre.h>
#include <IS/GameObject/GameObject.h>
#include <IS/GameObject/Components/Mesh/MeshComponent.h>
#include <IS/GameObject/Components/Bomb/Bomb.h>

namespace IS
{
	namespace GameObject
	{
		namespace Component
		{
			class Bomb;
		}
	}
}

namespace IS
{
	namespace GameObject
	{
		namespace Component
		{
			class BombManager : public AComponent
			{
			public:
				BombManager(Ogre::SceneNode *sceneNode, IS::Scene::Scene *scene, float range = 200.0f);
				virtual ~BombManager();

				virtual void update();

				void createBomb();
				int getNbBombDispo() const;
				void setNbBombDispo(int nb);
				float getRange() const;
				void setRange(float range);
				void bombToDestroy(IS::GameObject::Component::Bomb *bomb);
			
			private:
				std::string		_name;
				int				_id;
				int				_nbBombDispo;
				float			_range;
			};

		}
	}
}