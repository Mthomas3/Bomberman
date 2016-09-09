#pragma once

#include <cstdint>
#include <functional>
#include <iostream>
#include <utility>
#include <IS/Map/Map.hpp>

namespace IS
{
	struct Map::Tile
	{
		Tile() : _type(None), _upos(0, 0), _init(nullptr) { throw std::exception(); }
		Tile(TileType type, f_init const &init, insT *construtor)
			: _type(type), _upos(0, 0), _construtor(construtor), _init(init)
		{
			_instances = new std::vector<objT*>();
			_index = _instances->end();
		}

		TileType getType() const { return (_type); }
		objT *getInstance() { return (*_index); }

		void _new(unitPos const& upos, realPos const& rpos)
		{
			_upos = upos;
			_instances->push_back(_init(rpos, _construtor));
			_index = _instances->end();
		}
		void _delete()
		{
			delete(*_index);
			_instances->erase(_index);
			_index = _instances->begin();
		}
		objT *_release()
		{
			objT *ptr;

			ptr = *_index;
			_instances->erase(_index);
			_index = _instances->begin();
			return (ptr);
		}
		void _clear()
		{
			std::for_each(_instances->begin(), _instances->end(), [](objT *data) { delete (data); });
			_instances->clear();
		}

		std::shared_ptr<Tile> clone() { return (std::make_shared<Tile>(*this)); }

	private:
		TileType _type;
		unitPos _upos;
		std::vector<objT*> *_instances;
		std::vector<objT*>::iterator _index;
		insT *_construtor;
		f_init _init;
	};
}