#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <vector>
#include <IS/GameObject/GameObject.h>

namespace IS
{
	namespace Scene
	{
		class Scene;
	}
}

namespace IS
{
	class Map
	{
	public:
		typedef IS::GameObject::GameObject objT;
		typedef IS::Scene::Scene insT;
		typedef Ogre::Real posT;

		typedef std::pair<int8_t, int8_t> unitPos;
		typedef std::pair<posT, posT> realPos;
		typedef std::function<posT(int8_t)> f_convert;
		typedef std::function<objT*(realPos, insT *)> f_init;

	public:
		enum TileType
		{
			None = 0,
			Border,
			UnbreakBlock,
			BreakBlock,
			Powerup,
			Bomb
		};

	public:
		struct Tile;
		struct Config;

	public:
		Map(std::shared_ptr<Config> const &, insT *);
		~Map();

	public:
		std::vector<std::shared_ptr<Tile>> &getArround(unitPos const&);
		std::vector<std::shared_ptr<Tile>> &getWalkable(unitPos const&);
		int getAt(realPos const &);
		void setAt(unitPos const &, TileType);
		unitPos const &getDimension() const;
		std::shared_ptr<Tile> operator[](unitPos const&);

	public:
		unitPos const ogreToMap(realPos const &);
		realPos const mapToOgre(unitPos const &);

	private:
		void createStaticBlock();
		void createBreakableBlock();
		void putTile(unitPos const &, TileType);

	private:
		std::shared_ptr<Config> _config;
		insT *_instManager;
		std::map<unitPos, std::shared_ptr<Tile>> _tiles;
		std::vector<std::shared_ptr<unitPos>> _walkable;
	};
};

std::ostream& operator<<(std::ostream&, IS::Map &);
IS::Map::unitPos operator+(IS::Map::unitPos const&, IS::Map::unitPos const&);
IS::Map::unitPos operator-(IS::Map::unitPos const&, IS::Map::unitPos const&);
IS::Map::realPos operator+(IS::Map::realPos const&, IS::Map::realPos const&);
IS::Map::realPos operator-(IS::Map::realPos const&, IS::Map::realPos const&);