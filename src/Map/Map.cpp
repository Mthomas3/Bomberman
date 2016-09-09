#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <IS/Map/Map.hpp>
#include <IS/Map/Config.hpp>
#include <IS/Scene/Scene.h>

#pragma region Constructor/Destructor
IS::Map::Map(std::shared_ptr<IS::Map::Config> const &config, insT *instManager)
	: _config(config), _instManager(instManager)
{
	std::srand(unsigned(std::time(0)));
	createStaticBlock();
	createBreakableBlock();
}

IS::Map::~Map()
{
	_tiles.clear();
	_walkable.clear();
}
#pragma endregion

#pragma region User map infos
std::vector<std::shared_ptr<IS::Map::Tile>> &IS::Map::getArround(unitPos const &p)
{
	std::vector<std::shared_ptr<IS::Map::Tile>> _npos;

	_npos.push_back(p.second > 0 ? _tiles[p + unitPos(0, -1)] : nullptr);
	_npos.push_back(p.first > _config->getDimension().first - 1 ? _tiles[p + unitPos(1, 0)] : nullptr);
	_npos.push_back(p.second < _config->getDimension().second - 1 ? _tiles[p + unitPos(0, 1)] : nullptr);
	_npos.push_back(p.first > 0 ? _tiles[p + unitPos(-1, 0)] : nullptr);
	return (_npos);
}

std::vector<std::shared_ptr<IS::Map::Tile>> &IS::Map::getWalkable(unitPos const &p)
{
	std::vector<std::shared_ptr<IS::Map::Tile>> _npos;

	_npos = getArround(p);
	for (auto&& pos : _npos)
		if (pos->getType() != None)
			pos = nullptr;
	return (_npos);
}

int IS::Map::getAt(realPos const &p)
{
	unitPos uPos;

	uPos = ogreToMap(p);
	if (uPos == unitPos(-1, -1))
		return (-1);
	try
	{
		return (static_cast<int>(_tiles[uPos]->getType()));
	}
	catch (...)
	{
		return (-1);
	}
}
void IS::Map::setAt(unitPos const &p, TileType type)
{
	if (!_config)
		return;
	if (_tiles.find(p) == _tiles.end() || _config->getTile(type) == nullptr)
		return;
	putTile(p, type);
}
IS::Map::unitPos const &IS::Map::getDimension() const
{
	return (_config->getDimension());
}

std::shared_ptr<IS::Map::Tile> IS::Map::operator[](unitPos const &p)
{
	if (p.first > _config->getDimension().first + 1 || p.second > _config->getDimension().second + 1)
		return (nullptr);
	return (_tiles[p]);
}
#pragma endregion

#pragma region Map tools
void IS::Map::putTile(unitPos const &upos, TileType type)
{
	realPos rpos;
	std::shared_ptr<Tile> tile;

	tile = _config->getTile(type);
	rpos = _config->getOffset() + realPos(_config->getConverter()(upos.first - _config->getDimension().first / 2),
		_config->getConverter()(upos.second - _config->getDimension().second / 2));
	tile->_new(upos, rpos);
	_tiles[upos] = tile->clone();
}
IS::Map::unitPos const IS::Map::ogreToMap(realPos const &pos)
{
	realPos start;
	unitPos uPos;

	start.first = _config->getOffset().first - _config->getConverter()(_config->getDimension().first / 2);
	start.second = _config->getOffset().second - _config->getConverter()(_config->getDimension().second / 2);
	if (!_config->getConverter()(1))
		return (unitPos(-1, -1));
	try
	{
		uPos.first = static_cast<int8_t>((pos.first - start.first + (_config->getConverter()(1) / 2)) / _config->getConverter()(1)) + 1;
		uPos.second = static_cast<int8_t>((pos.second - start.second - (_config->getConverter()(1) / 2)) / _config->getConverter()(1)) + 1;
		return (uPos);
	}
	catch (...)
	{
		return (unitPos(-1, -1));
	}
}
IS::Map::realPos const IS::Map::mapToOgre(unitPos const &pos)
{
	unitPos mid(_config->getDimension().first / 2 + 1, _config->getDimension().second / 2);
	unitPos target(pos - mid);

	return (realPos(_config->getConverter()(target.first) - _config->getOffset().first,
		_config->getConverter()(target.second) - _config->getOffset().second));
}
#pragma endregion

#pragma region Map generation tools
void IS::Map::createStaticBlock()
{
	for (uint16_t i = 0; i < (_config->getDimension().first + 2) * (_config->getDimension().second + 2); i++)
	{
		unitPos pos(i % (_config->getDimension().first + 2), i / (_config->getDimension().first + 2));
		if (!pos.first || pos.first == _config->getDimension().first + 1 ||
			!pos.second || pos.second == _config->getDimension().second + 1)
			putTile(pos, Border);
		else if (!(pos.first % 2) && !(pos.second % 2))
			putTile(pos, UnbreakBlock);
		else
		{
			putTile(pos, None);
			_walkable.push_back(std::make_shared<unitPos>(pos));
		}
	}
}

void IS::Map::createBreakableBlock()
{
	int16_t i;

	std::random_shuffle(_walkable.begin(), _walkable.end());
	i = 0;
	for (auto empty : _walkable)
	{
		i++;
		if (std::find(_config->getSpawns().begin(), _config->getSpawns().end(), *empty - unitPos(1, 1)) == _config->getSpawns().end())
			putTile(*empty, BreakBlock);
		else
			i--;
		if (i > (_walkable.size() * _config->getBlockRate()))
			break;
	}
	_walkable.clear();
}
#pragma endregion

#pragma region Operator Overload
std::ostream &operator<<(std::ostream &os, IS::Map &map)
{
	std::string icon = { ' ', 'X', 'U', 'B', 'P', '0' };

	os << "[S T A R T   M A P   D E B U G]" << std::endl << std::endl;
	for (uint16_t i = 0; i < (map.getDimension().first + 2) * (map.getDimension().second + 2); i++)
	{
		IS::Map::unitPos pos(i % (map.getDimension().first + 2), i / (map.getDimension().first + 2));
		auto tile_ptr = map[pos];
		if (tile_ptr)
			os << icon[tile_ptr->getType()];
		else
			os << '.';
		if (pos.first < map.getDimension().first + 1)
			os << ' ';
		else
			os << std::endl;
	}
	os << std::endl << "[  E N D   M A P   D E B U G  ]" << std::endl;
	return (os);
}

IS::Map::unitPos operator+(IS::Map::unitPos const& a, IS::Map::unitPos const& b)
{
	return IS::Map::unitPos(a.first + b.first, a.second + b.second);
}

IS::Map::unitPos operator-(IS::Map::unitPos const& a, IS::Map::unitPos const& b)
{
	return IS::Map::unitPos(a.first - b.first, a.second - b.second);
}

IS::Map::realPos operator+(IS::Map::realPos const& a, IS::Map::realPos const& b)
{
	return IS::Map::realPos(a.first + b.first, a.second + b.second);
}

IS::Map::realPos operator-(IS::Map::realPos const& a, IS::Map::realPos const& b)
{
	return IS::Map::realPos(a.first - b.first, a.second - b.second);
}
#pragma endregion