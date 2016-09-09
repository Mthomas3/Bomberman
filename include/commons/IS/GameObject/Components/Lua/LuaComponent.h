#pragma once

#include <initializer_list>
#include <IS/GameObject/Components/Player/Player.h>
#include <IS/Map/Map.hpp>
#include <utility>

extern "C" {
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

namespace IS
{
	namespace GameObject
	{
		namespace Component
		{
			class PowerUpOnPlayer;
		}
	}
}

namespace IS
{
	namespace GameObject
	{
		namespace Component
		{
			class LuaComponent : public Player
			{
			public:
				LuaComponent(Ogre::SceneNode *sceneNode, Scene::Scene *scene, Ogre::SceneManager *sceneManager, const std::string &path, int id, Dir, Ogre::Entity *entity, std::shared_ptr<IS::Map> map);
				virtual ~LuaComponent();

				virtual void update();

				bool isSafe(int x, int y);
				void getOffGoal(int &x, int &y);

				std::shared_ptr<IS::Map>		getMap() const { return _map; }
				inline const std::pair<int8_t, int8_t>			&&getPos() const
				{
					return (_map->ogreToMap(std::pair<float, float>(_sceneNode->getPosition().x, _sceneNode->getPosition().z)));
				}

			private:
				int		callIntFunction(const std::string &name, std::initializer_list<size_t> args);
				void	callVoidFunction(const std::string &name, std::initializer_list<size_t> args);
				void	callVoidFunction(const std::string &name);

				void	updateColliders();
				void	updateDangers();

				std::string						_path;
				Ogre::SceneManager				*_sceneManager;
				lua_State						*_lState;
				std::shared_ptr<IS::Map>		_map;
				int								*_collidersMap;
				int								*_dangersMap;
			};
		}
	}
}