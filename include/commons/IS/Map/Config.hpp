#pragma once

#include <cstdint>
#include <map>
#include <set>
#include <utility>
#include <IS/Map/Map.hpp>
#include <IS/Map/Tile.hpp>

namespace IS
{
	struct Map::Config
	{
		Config(unitPos const &dimension, realPos const &offset,
			float blockRate, f_convert const &converter)
			: _offset(offset), _blockrate(blockRate), _converter(converter)
		{
			_dimension = unitPos(dimension.first % 2 ? dimension.first : dimension.first + 1, dimension.second % 2 ? dimension.second : dimension.second + 1);
		}
		~Config() { std::for_each(_tiles.begin(), _tiles.end(), [](std::pair<TileType, Tile> _tiles) { _tiles.second._clear(); }); }

		Config &operator<<(unitPos const &position) { _spawn.insert(position); return (*this); }
		Config &operator<<(Tile const &tile) { _tiles.insert(std::pair<TileType, Tile>(tile.getType(), tile)); return (*this); }

		unitPos const &getDimension() const { return (_dimension); }
		realPos const &getOffset() const { return (_offset); }
		float const getBlockRate() const { return (_blockrate); }
		std::set<unitPos> const &getSpawns() const { return (_spawn); }
		std::shared_ptr<Tile> getTile(TileType type) { return (std::make_shared<Tile>(_tiles[type])); }
		f_convert const &getConverter() const { return (_converter); }

	private:
		unitPos _dimension;
		realPos _offset;
		float _blockrate;
		f_convert _converter;
		std::set<unitPos> _spawn;
		std::map<TileType, Tile> _tiles;
	};
}