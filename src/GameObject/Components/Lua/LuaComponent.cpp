#include <IS/GameObject/Components/Lua/LuaComponent.h>
#include <IS/GameObject/Components/Bomb/BombManager.h>
#include <csignal>
#include <ctime>
#include <cstring>

extern "C" {
	int goForward(lua_State *);
	int goDown(lua_State *);
	int goRight(lua_State *);
	int goLeft(lua_State *);
	int	getMapAt(lua_State *l);
	int getPos(lua_State *);
	int dropBomb(lua_State *);
	int isSafe(lua_State *);
	int setOffGoal(lua_State *);
	void abrt_handler(int);
}

static std::vector<size_t>	_ptr;

void abrt_handler(int sig)
{
	sig = sig;
}

int	checkPtr(size_t ptr)
{
	if (std::find(_ptr.begin(), _ptr.end(), ptr) == _ptr.end())
		return 0;
	return 1;
}

int dropBomb(lua_State *l)
{
	size_t ptr;
	IS::GameObject::Component::LuaComponent *lua;

	if (lua_gettop(l) < 1)
		return 0;
	if ((ptr = lua_tonumber(l, -1)) == 0)
		return 0;
	if (!checkPtr(ptr))
		return 0;
	lua = reinterpret_cast<IS::GameObject::Component::LuaComponent *>(ptr);
	lua->getBombManager()->createBomb();
	return 0;
}

int	setOffGoal(lua_State *l)
{
	size_t ptr;
	IS::GameObject::Component::LuaComponent *lua;

	if (lua_gettop(l) < 3)
	{
		lua_pushinteger(l, 0);
		lua_pushinteger(l, 0);
		return 2;
	}
	if ((ptr = lua_tonumber(l, -1)) == 0)
	{
		lua_pushinteger(l, 0);
		lua_pushinteger(l, 0);
		return 2;
	}
	if (!checkPtr(ptr))
	{
		lua_pushinteger(l, 0);
		lua_pushinteger(l, 0);
		return 2;
	}
	lua = reinterpret_cast<IS::GameObject::Component::LuaComponent *>(ptr);
	int x = static_cast<int>(lua_tointeger(l, 1));
	int y = static_cast<int>(lua_tointeger(l, 2));
	lua->getOffGoal(x, y);
	lua_pushinteger(l, x);
	lua_pushinteger(l, y);
	return 2;
}

int	getMapAt(lua_State *l)
{
	size_t ptr;
	IS::GameObject::Component::LuaComponent *lua;

	if (lua_gettop(l) < 3)
	{
		lua_pushnumber(l, -1);
		return 1;
	}
	if ((ptr = lua_tonumber(l, -1)) == 0)
	{
		lua_pushnumber(l, -1);
		return 1;
	}
	if (!checkPtr(ptr))
	{
		lua_pushnumber(l, -1);
		return 1;
	}
	lua = reinterpret_cast<IS::GameObject::Component::LuaComponent *>(ptr);
	int x = lua_tonumber(l, 2);
	int y = lua_tonumber(l, 3);
	int ret = lua->getMap()->getAt(lua->getMap()->mapToOgre(std::pair<int8_t, int8_t>(x, y)));
	lua_pushinteger(l, ret);
	return 1;
}

int getPos(lua_State *l)
{
	size_t ptr;
	IS::GameObject::Component::LuaComponent *lua;

	if (lua_gettop(l) < 1)
	{
		lua_pushinteger(l, 0);
		lua_pushinteger(l, 0);
		return 2;
	}
	if ((ptr = lua_tonumber(l, lua_gettop(l))) == 0)
	{
		lua_pushinteger(l, 0);
		lua_pushinteger(l, 0);
		return 2;
	}
	if (!checkPtr(ptr))
	{
		lua_pushinteger(l, 0);
		lua_pushinteger(l, 0);
		return 2;
	}
	lua = reinterpret_cast<IS::GameObject::Component::LuaComponent *>(ptr);
	const std::pair<int8_t, int8_t> pos = lua->getPos();
	lua_pushinteger(l, static_cast<int>(pos.first));
	lua_pushinteger(l, static_cast<int>(pos.second));
	return 2;
}

int	goRight(lua_State *l)
{
	size_t ptr;
	IS::GameObject::Component::LuaComponent *lua;

	if (lua_gettop(l) < 1)
		return 0;
	if ((ptr = lua_tonumber(l, -1)) == 0)
		return 0;
	if (!checkPtr(ptr))
		return 0;
	lua = reinterpret_cast<IS::GameObject::Component::LuaComponent *>(ptr);
	lua->aIMoveRight();
	return 0;
}

int	goLeft(lua_State *l)
{
	size_t ptr;
	IS::GameObject::Component::LuaComponent *lua;

	if (lua_gettop(l) < 1)
		return 0;
	if ((ptr = lua_tonumber(l, -1)) == 0)
		return 0;
	if (!checkPtr(ptr))
		return 0;
	lua = reinterpret_cast<IS::GameObject::Component::LuaComponent *>(ptr);
	lua->aIMoveLeft();
	return 0;
}

int	goForward(lua_State *l)
{
	size_t ptr;
	IS::GameObject::Component::LuaComponent *lua;

	if (lua_gettop(l) < 1)
		return 0;
	if ((ptr = lua_tonumber(l, -1)) == 0)
		return 0;
	if (!checkPtr(ptr))
		return 0;
	lua = reinterpret_cast<IS::GameObject::Component::LuaComponent *>(ptr);
	lua->aIMoveForward();
	return 0;
}

int	goDown(lua_State *l)
{
	size_t ptr;
	IS::GameObject::Component::LuaComponent *lua;

	if (lua_gettop(l) < 1)
		return 0;
	if ((ptr = lua_tonumber(l, -1)) == 0)
		return 0;
	if (!checkPtr(ptr))
		return 0;
	lua = reinterpret_cast<IS::GameObject::Component::LuaComponent *>(ptr);
	lua->aIMoveDown();
	return 0;
}

int	isSafe(lua_State *l)
{
	size_t ptr;
	IS::GameObject::Component::LuaComponent *lua;

	if (lua_gettop(l) < 3)
		return 0;
	if ((ptr = lua_tonumber(l, 1)) == 0)
		return 0;
	if (!checkPtr(ptr))
		return 0;
	lua = reinterpret_cast<IS::GameObject::Component::LuaComponent *>(ptr);
	int x = lua_tonumber(l, 2);
	int y = lua_tonumber(l, 3);
	if (lua->isSafe(x, y))
		lua_pushinteger(l, 1);
	else
		lua_pushinteger(l, 0);
	return 1;
}

IS::GameObject::Component::LuaComponent::LuaComponent(Ogre::SceneNode *sceneNode, Scene::Scene *scene, Ogre::SceneManager * sceneManager, const std::string & path, int id, Dir dir, Ogre::Entity * entity, std::shared_ptr<IS::Map> map) : IS::GameObject::Component::Player(sceneNode, scene, id, dir, entity), _path(path), _map(map)
{
	std::signal(SIGABRT, abrt_handler);
	_lState = luaL_newstate();
	luaL_openlibs(_lState);
	_ptr.push_back(reinterpret_cast<size_t>(this));
	lua_register(_lState, "goForward", goForward);
	lua_register(_lState, "goDown", goDown);
	lua_register(_lState, "goRight", goRight);
	lua_register(_lState, "goLeft", goLeft);
	lua_register(_lState, "getPos", ::getPos);
	lua_register(_lState, "getMapAt", getMapAt);
	lua_register(_lState, "dropBomb", ::dropBomb);
	lua_register(_lState, "isSafe", ::isSafe);
	lua_register(_lState, "setOffGoal", ::setOffGoal);
	luaL_dofile(_lState, path.c_str());
	_collidersMap = new int[static_cast<int>(_map->getDimension().first) * static_cast<int>(_map->getDimension().second)];
	_dangersMap = new int[static_cast<int>(_map->getDimension().first) * static_cast<int>(_map->getDimension().second)];
	updateColliders();
	updateDangers();
	//callVoidFunction("load", { (size_t)this });
}

IS::GameObject::Component::LuaComponent::~LuaComponent()
{
	//callVoidFunction("unload");
	lua_close(_lState);
	delete _dangersMap;
	delete _collidersMap;
}

void	IS::GameObject::Component::LuaComponent::update()
{
	updateColliders();
	updateDangers();
	//callVoidFunction("update");
}

int IS::GameObject::Component::LuaComponent::callIntFunction(const std::string &name, std::initializer_list<size_t> args)
{
	int ret = 0;

	lua_getglobal(_lState, name.c_str());
	for (auto arg : args)
		lua_pushnumber(_lState, arg);
	if (lua_isfunction(_lState, -1))
	{
		lua_call(_lState, args.size(), 1);
		ret = static_cast<int>(lua_tointeger(_lState, -1));
	}
	lua_pop(_lState, 1);
	return ret;
}

void IS::GameObject::Component::LuaComponent::callVoidFunction(const std::string &name, std::initializer_list<size_t> args)
{
	lua_getglobal(_lState, name.c_str());

	for (auto arg : args)
		lua_pushnumber(_lState, arg);
	lua_call(_lState, args.size(), 0);
	lua_pop(_lState, 1);
}

void IS::GameObject::Component::LuaComponent::callVoidFunction(const std::string &name)
{
	lua_getglobal(_lState, name.c_str());
	lua_call(_lState, 0, 0);
	lua_pop(_lState, 1);
}

void IS::GameObject::Component::LuaComponent::updateColliders()
{
	std::pair<int8_t, int8_t> dim = _map->getDimension();

	for (int x = 0; x < static_cast<int>(dim.first); x++)
	{
		for (int y = 0; y < static_cast<int>(dim.second); y++)
		{
			if (_map->getAt(_map->mapToOgre(std::pair<int8_t, int8_t>(x, y))) > 0)
				_collidersMap[x + y * static_cast<int>(dim.first)] = 1;
			else
				_collidersMap[x + y * static_cast<int>(dim.first)] = 0;
		}
	}
}

void IS::GameObject::Component::LuaComponent::updateDangers()
{
	std::pair<int8_t, int8_t> dim = _map->getDimension();

	std::memset(_dangersMap, 0, sizeof(_dangersMap));
	for (int x = 0; x < static_cast<int>(dim.first); x++)
	{
		for (int y = 0; y < static_cast<int>(dim.second); y++)
		{
			if (_map->getAt(_map->mapToOgre(std::pair<int8_t, int8_t>(x, y))) == 5)
			{
				_dangersMap[x + y * static_cast<int>(dim.first)] = 1;
				for (int i = 1; i < 5; i++)
				{
					if (x + i < static_cast<int>(dim.first))
					{
						if (_collidersMap[x + i + y * static_cast<int>(dim.first)] == 1)
							break;
						_dangersMap[x + i + y * static_cast<int>(dim.first)] = 1;
					}
				}
				for (int i = 1; i < 5; i++)
				{
					if (x - i >= 0)
					{
						if (_collidersMap[x - i + y * static_cast<int>(dim.first)] == 1)
							break;
						_dangersMap[x - i + y * static_cast<int>(dim.first)] = 1;
					}
				}
				for (int i = 1; i < 5; i++)
				{
					if (y + i < static_cast<int>(dim.second))
					{
						if (_collidersMap[x + (y + i) * static_cast<int>(dim.first)] == 1)
							break;
						_dangersMap[x + (y + i) * static_cast<int>(dim.first)] = 1;
					}
				}
				for (int i = 1; i < 5; i++)
				{
					if (y - i >= 0)
					{
						if (_collidersMap[x + (y + i) * static_cast<int>(dim.first)] == 1)
							break;
						_dangersMap[x + (y + i) * static_cast<int>(dim.first)] = 1;
					}
				}
			}
		}
	}
}

bool	IS::GameObject::Component::LuaComponent::isSafe(int x, int y)
{
	if ((_dangersMap[x + y * static_cast<int>(_map->getDimension().first)]) == 0)
	{
		return true;
	}
	return false;
}

void IS::GameObject::Component::LuaComponent::getOffGoal(int &x, int &y)
{
	short n = std::rand() % 4;
	char dir_x[] = { -1,0,1,0 };
	char dir_y[] = { 0,-1,0,1 };

	while (dir_x[n] + x < 0 || dir_x[n] + x >= static_cast<int>(_map->getDimension().first)
		|| dir_y[n] + y < 0 || dir_y[n] + y >= static_cast<int>(_map->getDimension().second))
	{
		n = std::rand() % 4;
	}
	if (this->isSafe(x + dir_x[n], y + dir_y[n])) {
		dropBomb();
	}
	x += dir_x[n];
	y += dir_y[n];
}