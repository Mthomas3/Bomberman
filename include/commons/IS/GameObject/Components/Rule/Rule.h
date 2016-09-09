#pragma once

#include <IS/GameObject/Components/AComponent.h>
#include <OGRE/Ogre.h>
#include <IS/Map/Map.hpp>

namespace IS
{
	namespace GameObject
	{
		namespace Component
		{
			class Rule
			{
			public:
				Rule(Scene::Scene *, IS::Scene::SceneManager *sceneManager, int nbLife, int nbPlayer);
				virtual ~Rule();

				virtual void updateRule();

				void setLife(int nbLife);
				int getLife() const;
				void setPlayer(int player);
				int getPlayer() const;
				std::shared_ptr<IS::Map> getMap() const;
				
				void initialisation();
				void initMap();
				void initChara();
				void deleteChara();
			private:
				IS::Scene::SceneManager							*_sceneManager;
				Scene::Scene									*_scene;
				int												_nbLife;
				std::vector<IS::GameObject::GameObject *>		_charaVec;
				std::shared_ptr<IS::Map>					    _map;
				bool											_victory;
				int												_nbPlayer;
				bool											_isPlaying;
				bool											_done;
			};
		}
	}
}