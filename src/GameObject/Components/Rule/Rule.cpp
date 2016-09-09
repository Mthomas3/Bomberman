#include <IS/GameObject/Components/Rule/Rule.h>
#include <IS/GameObject/Components/Mesh/MeshComponent.h>
#include <IS/GameObject/Components/Player/Player.h>
#include <IS/GameObject/Components/Lua/LuaComponent.h>
#include <IS/Scene/Scene.h>
#include <IS/Scene/SceneManager.h>
#include <IS/Map/Map.hpp>
#include <IS/Map/Tile.hpp>
#include <IS/GameObject/Components/DestroyPhysic/DestroyPhysic.h>
#include <IS/Map/Config.hpp>
#include <IS/Scene/DotSceneLoader.h>
#include "IS/GameObject/Components/PowerUp/PowerUpOnCrate.h"
#include <IS/GameObject/Components/DestroyPhysic/DestroyPhysicDynamic.h>
#include <IS/Audio/Audio.h>
#include <IS/Audio/Manager.h>
#include <IS/Audio/Playlist.h>
#include <IS/GUI/GUI.hh>
#include <IS/input/Input.hh>

IS::GameObject::Component::Rule::Rule(IS::Scene::Scene *scene, IS::Scene::SceneManager *sceneManager, int nbLife, int nbPlayer) 
{
	_scene = scene;
	_sceneManager = sceneManager;
	_nbLife = nbLife;
	_victory = true;
	_nbPlayer = nbPlayer;
	_isPlaying = false;
	_done = false;
	initialisation();
}

IS::GameObject::Component::Rule::~Rule()
{

}

void IS::GameObject::Component::Rule::updateRule()
{
	if (_isPlaying)
	{
		if (InputManager::InputListener::getInstance()->isKeyDown(InputManager::InputListener::Key::KEY_ESCAPE))
		{
			IS::Utils::Time::getInstance()->setRatio(0);
			Gui::GuiMenu::pause();
		}
		if (_charaVec.size() == 1)
		{
			int rdPart = std::rand() % 1000000;
			std::string name = "part";
			name += std::to_string(rdPart);
			if (!_done)
			{
				if (_victory)
				{
					_charaVec.erase(std::remove(_charaVec.begin(), _charaVec.end(), _charaVec[0]), _charaVec.end());
					//continue victory
					Ogre::ParticleSystem* particleSystem = _scene->getSceneManager()->getSceneManager()->createParticleSystem(name, "Examples/Fireworks");
					_charaVec[0]->getSceneNode()->getParentSceneNode()->attachObject(particleSystem);
					Gui::GuiMenu::pause();
					_done = true;
				}
				else
				{
					Ogre::ParticleSystem* particleSystem = _scene->getSceneManager()->getSceneManager()->createParticleSystem(name, "Examples/FireworksRed");
					_charaVec[0]->getSceneNode()->getParentSceneNode()->attachObject(particleSystem);
					Gui::GuiMenu::pause();
					_done = true;
				}
			}
		}
		for (int i = 0; i < _charaVec.size(); i++)
		{
			if (!_charaVec[i]->getComponent<IS::GameObject::Component::Player>()->getAlive())
			{
				if (_charaVec[i]->getComponent<IS::GameObject::Component::Player>()->getNbLife() > 0)
				{
					switch (i)
					{
					case 0:
						_charaVec[i]->getSceneNode()->setPosition(Ogre::Vector3(346, 0, 282));
							if (_charaVec[i]->getComponent<IS::GameObject::Component::Player>()->getNbLife() > 1)
								_charaVec[i]->getComponent<IS::GameObject::Component::Player>()->setAlive(true);
						_charaVec[i]->getComponent<IS::GameObject::Component::Player>()->setNbLife(_charaVec[i]->getComponent<IS::GameObject::Component::Player>()->getNbLife() - 1);
						break;
					case 1:
						if (_charaVec[i]->getComponent<IS::GameObject::Component::Player>()->getNbLife() > 1)
							_charaVec[i]->getComponent<IS::GameObject::Component::Player>()->setAlive(true);
						_charaVec[i]->getSceneNode()->setPosition(Ogre::Vector3(-240, 0, 282));
						_charaVec[i]->getComponent<IS::GameObject::Component::Player>()->setNbLife(_charaVec[i]->getComponent<IS::GameObject::Component::Player>()->getNbLife() - 1);
						break;
					case 2:
						if (_charaVec[i]->getComponent<IS::GameObject::Component::Player>()->getNbLife() > 1)
							_charaVec[i]->getComponent<IS::GameObject::Component::Player>()->setAlive(true);
						_charaVec[i]->getSceneNode()->setPosition(Ogre::Vector3(-233, 0, -189));
						_charaVec[i]->getComponent<IS::GameObject::Component::Player>()->setNbLife(_charaVec[i]->getComponent<IS::GameObject::Component::Player>()->getNbLife() - 1);
						break;
					case 3:
						if (_charaVec[i]->getComponent<IS::GameObject::Component::Player>()->getNbLife() > 1)
							_charaVec[i]->getComponent<IS::GameObject::Component::Player>()->setAlive(true);
						_charaVec[i]->getSceneNode()->setPosition(Ogre::Vector3(345, 0, -189));
						_charaVec[i]->getComponent<IS::GameObject::Component::Player>()->setNbLife(_charaVec[i]->getComponent<IS::GameObject::Component::Player>()->getNbLife() - 1);
						break;
					default:
						break;
					}

				}
				else
				{
					if (i == 0)
						_victory = false;
					_charaVec[i]->getSceneNode()->setVisible(false);
					_charaVec[i]->removeComponent<IS::GameObject::Component::DestroyPhysicDynamic>();
					_charaVec[i]->removeComponent<IS::GameObject::Component::Player>();
					_charaVec.erase(std::remove(_charaVec.begin(), _charaVec.end(), _charaVec[i]), _charaVec.end());
				}
			}
		}
	}
}

void IS::GameObject::Component::Rule::initialisation()
{
	/*IS::GameObject::GameObject *Splash = new IS::GameObject::GameObject(_sceneManager);
	Ogre::ParticleSystem* particleSystem = _scene->getSceneManager()->getSceneManager()->createParticleSystem("particle3", "SplashScreen");
	Splash->getSceneNode()->getParentSceneNode()->attachObject(particleSystem);
	*/
	initMap();
}

void IS::GameObject::Component::Rule::initMap()
{
	IS::Scene::DotSceneLoader loader;

	_sceneManager->getSceneManager()->setSkyDome(true, "Examples/CloudySky", 5, 8);
	
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, -1000);
	Ogre::MeshManager::getSingleton().createPlane(
		"ground",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane,
		5000, 5000, 20, 20,
		true,
		1, 5, 5,
		Ogre::Vector3::UNIT_Z);
	Ogre::Entity* groundEntity = _scene->getSceneManager()->getSceneManager()->createEntity("ground");
	_scene->getSceneManager()->getSceneManager()->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
	groundEntity->setCastShadows(false);
	groundEntity->setMaterialName("Ocean2_HLSL_GLSL");

	/*scene*/
	IS::GameObject::GameObject *sceneAll = new IS::GameObject::GameObject(_sceneManager);
	sceneAll->getSceneNode()->setScale(Ogre::Vector3(19.5f));
	sceneAll->getSceneNode()->setPosition(sceneAll->getSceneNode()->getPosition().x - 435, sceneAll->getSceneNode()->getPosition().y, sceneAll->getSceneNode()->getPosition().z + 1050);
	sceneAll->getSceneNode()->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(0.2));
	sceneAll->getSceneNode()->setDirection(Ogre::Vector3(0, 0, 0));
	std::srand(std::time(0));
	int rd = std::rand() % 3;
	switch (rd)
	{
		case 0:
			loader.parseDotScene("scn_scene_1.scene", "Popular", _sceneManager->getSceneManager(), sceneAll->getSceneNode());
			break;
		case 1:
			loader.parseDotScene("scn_scene_2.scene", "Popular", _sceneManager->getSceneManager(), sceneAll->getSceneNode());
			break;
		case 2:
			loader.parseDotScene("scn_scene_3.scene", "Popular", _sceneManager->getSceneManager(), sceneAll->getSceneNode());
			break;
	}
	loader.getEntities()[0]->getSubEntity(2)->setMaterialName("Ocean2_HLSL_GLSL");
	_scene->addObject(sceneAll);


	IS::Map::unitPos sizeMap(13, 11);
	IS::Map::realPos offset(0.0f, 0.0f);
	float blockRate(0.3);

	/* Attention, faire une shared_ptr pour Config, sinon il s'auto-detruit à la fin de la fonction, avec les tiles*/
	std::shared_ptr<IS::Map::Config> config = std::make_shared<IS::Map::Config>(sizeMap, offset, blockRate, [](int8_t number) { return (IS::Map::posT(number * 50)); });

	/* Definition des Tiles */
	(*config) <<
		IS::Map::Tile(IS::Map::None, [](IS::Map::realPos position, IS::Map::insT *instance) { return nullptr; }, _scene) <<
		IS::Map::Tile(IS::Map::Border, [](IS::Map::realPos position, IS::Map::insT *instance)
	{
		std::stringstream ss;
		IS::GameObject::GameObject *object = new IS::GameObject::GameObject(instance->getSceneManager());

		ss << "Border |" << position.first << "|" << position.second << "|" << uint64_t(object);
		IS::GameObject::Component::MeshComponent *mesh = new IS::GameObject::Component::MeshComponent(object->getSceneNode(), instance, instance->getSceneManager()->getSceneManager(), ss.str(), "cube.mesh");
		mesh->getEntity()->setVisible(false);
		object->addComponent<IS::GameObject::Component::MeshComponent>(mesh);
		object->getSceneNode()->setScale(Ogre::Vector3(0.5));
		object->getSceneNode()->setPosition(position.first, 0, position.second - 10);
		instance->addObject(object);
		IS::GameObject::Component::DestroyPhysic *destroyPhysic = new IS::GameObject::Component::DestroyPhysic(object->getSceneNode(), instance, IS::Physics::newPhysicObject<IS::Physics::StaticObject>(object->getSceneNode(), instance, IS::Physics::CollisionType::COLLISION_ACCURATE));
		object->addComponent<IS::GameObject::Component::DestroyPhysic>(destroyPhysic);
		return (object);
	}, _scene) <<
		IS::Map::Tile(IS::Map::UnbreakBlock, [](IS::Map::realPos position, IS::Map::insT *instance)
	{
		std::stringstream ss;
		IS::GameObject::GameObject *object = new IS::GameObject::GameObject(instance->getSceneManager());

		ss << "Border |" << position.first << "|" << position.second << "|" << uint64_t(object);
		IS::GameObject::Component::MeshComponent *mesh = new IS::GameObject::Component::MeshComponent(object->getSceneNode(), instance, instance->getSceneManager()->getSceneManager(), ss.str(), "pillar_rock_new.mesh");
	//	mesh->getEntity()->getSubEntity(0)->setMaterialName("Examples/Rocky");
		//mesh->getEntity()->getSubEntity(1)->setMaterialName("Examples/Rocky");
		object->addComponent<IS::GameObject::Component::MeshComponent>(mesh);
		object->getSceneNode()->setScale(Ogre::Vector3(20.5));
		object->getSceneNode()->setPosition(position.first , -10, position.second );
		instance->addObject(object);
		IS::Physics::newPhysicObject<IS::Physics::StaticObject>(object->getSceneNode(), instance, IS::Physics::CollisionType::COLLISION_ACCURATE);
		return (object);
	}, _scene) <<
		IS::Map::Tile(IS::Map::BreakBlock, [](IS::Map::realPos position, IS::Map::insT *instance)
	{
		std::stringstream ss;
		IS::GameObject::GameObject *object = new IS::GameObject::GameObject(instance->getSceneManager());

		ss << "Block |" << position.first << "|" << position.second << "|" << uint64_t(object);
		IS::GameObject::Component::MeshComponent *mesh = new IS::GameObject::Component::MeshComponent(object->getSceneNode(), instance, instance->getSceneManager()->getSceneManager(), ss.str(), "testCaisse.mesh");
		//mesh->getEntity()->getSubEntity(0)->setMaterialName("Examples/EnvMappedRustySteel");
		object->addComponent<IS::GameObject::Component::MeshComponent>(mesh);
		object->getSceneNode()->setScale(Ogre::Vector3(30.5));
		object->getSceneNode()->setPosition(position.first, -15, position.second);
		IS::GameObject::Component::DestroyPhysic *destroyPhysic = new IS::GameObject::Component::DestroyPhysic(object->getSceneNode(), instance, IS::Physics::newPhysicObject<IS::Physics::StaticObject>(object->getSceneNode(), instance, IS::Physics::CollisionType::COLLISION_ACCURATE));
		object->addComponent<IS::GameObject::Component::DestroyPhysic>(destroyPhysic);
		IS::GameObject::Component::PowerUpOnCrate *powerUpOnCrate = new IS::GameObject::Component::PowerUpOnCrate(object->getSceneNode(), instance);
		object->addComponent<IS::GameObject::Component::PowerUpOnCrate>(powerUpOnCrate);
		instance->addObject(object);
		//			IS::Physics::newPhysicObject<IS::Physics::StaticObject>(object->getSceneNode(), instance, IS::Physics::CollisionType::COLLISION_ACCURATE);
		return (object);
	}, _scene);

	/* Definitions des spawns */
	(*config) <<
		IS::Map::unitPos(0, 0) << IS::Map::unitPos(1, 0) << IS::Map::unitPos(0, 1) <<
		IS::Map::unitPos(sizeMap.first - 2, 0) << IS::Map::unitPos(sizeMap.first - 1, 0) << IS::Map::unitPos(sizeMap.first - 1, 1) <<
		IS::Map::unitPos(0, sizeMap.second - 1) << IS::Map::unitPos(1, sizeMap.second - 1) << IS::Map::unitPos(0, sizeMap.second - 2) <<
		IS::Map::unitPos(sizeMap.first - 2, sizeMap.second - 1) << IS::Map::unitPos(sizeMap.first - 1, sizeMap.second - 1) << IS::Map::unitPos(sizeMap.first - 1, sizeMap.second - 2);

	_map = std::make_shared<IS::Map>(config, _scene);
	//std::cout << *_map << std::endl;
	_scene->setMap(_map);
}

void IS::GameObject::Component::Rule::initChara()
{
	IS::GameObject::GameObject *object = new IS::GameObject::GameObject(_sceneManager);
	object->getSceneNode()->setScale(Ogre::Vector3(20.0f));
	object->getSceneNode()->setPosition(Ogre::Vector3(346, 0, 282));
	object->getSceneNode()->setDirection(Ogre::Vector3(0, 0, 1));
	IS::GameObject::Component::MeshComponent *mesh = new IS::GameObject::Component::MeshComponent(object->getSceneNode(), _scene, _sceneManager->getSceneManager(), "Chara_1", "character_default_p1.mesh");
	object->addComponent<IS::GameObject::Component::MeshComponent>(mesh);
	if (_nbPlayer >= 1)
	{
		IS::GameObject::Component::Player *player = new IS::GameObject::Component::Player(object->getSceneNode(), _scene, -1, IS::GameObject::Component::AAction::Dir::DOWN, mesh->getEntity());
		player->setSpeed(3);
		player->setNbLife(_nbLife);
		object->addComponent<IS::GameObject::Component::Player>(player);

	}
	else
	{
		IS::GameObject::Component::LuaComponent *Lua = new IS::GameObject::Component::LuaComponent(object->getSceneNode(), _scene, _sceneManager->getSceneManager(), "lua/test.lua", -2, IS::GameObject::Component::AAction::Dir::UP, mesh->getEntity(), _map);
		Lua->setSpeed(3);
		Lua->setNbLife(_nbLife);
		object->addComponent<IS::GameObject::Component::LuaComponent>(Lua);
	}
	IS::GameObject::Component::DestroyPhysicDynamic *destroyPhysic = new IS::GameObject::Component::DestroyPhysicDynamic(object->getSceneNode(), _scene, IS::Physics::newPhysicObject<IS::Physics::DynamicObject>(object->getSceneNode(), _scene, IS::Physics::CollisionType::COLLISION_ACCURATE));
	object->addComponent<IS::GameObject::Component::DestroyPhysicDynamic>(destroyPhysic);
	_scene->addObject(object);
	_charaVec.push_back(object);

	IS::GameObject::GameObject *object2 = new IS::GameObject::GameObject(_sceneManager);
	object2->getSceneNode()->setScale(Ogre::Vector3(20.0f));
	object2->getSceneNode()->setPosition(Ogre::Vector3(-240, 0, 282));
	object2->getSceneNode()->setDirection(Ogre::Vector3(0, 0, -1));
	IS::GameObject::Component::MeshComponent *mesh2 = new IS::GameObject::Component::MeshComponent(object2->getSceneNode(), _scene, _sceneManager->getSceneManager(), "Chara_2", "character_default_p2.mesh");
	object2->addComponent<IS::GameObject::Component::MeshComponent>(mesh2);
	if (_nbPlayer >= 2)
	{
	IS::GameObject::Component::Player *player2 = new IS::GameObject::Component::Player(object2->getSceneNode(), _scene, 0, IS::GameObject::Component::AAction::Dir::UP, mesh2->getEntity());
	player2->setSpeed(3);
	player2->setNbLife(_nbLife);
	object2->addComponent<IS::GameObject::Component::Player>(player2);
	}
	else
	{
		IS::GameObject::Component::LuaComponent *Lua2 = new IS::GameObject::Component::LuaComponent(object2->getSceneNode(), _scene, _sceneManager->getSceneManager(), "lua/test2.lua", -3, IS::GameObject::Component::AAction::Dir::UP, mesh2->getEntity(), _map);
		Lua2->setSpeed(3);
		Lua2->setNbLife(_nbLife);
		object2->addComponent<IS::GameObject::Component::Player>(Lua2);
	}
		IS::GameObject::Component::DestroyPhysicDynamic *destroyPhysic2 = new IS::GameObject::Component::DestroyPhysicDynamic(object2->getSceneNode(), _scene, IS::Physics::newPhysicObject<IS::Physics::DynamicObject>(object2->getSceneNode(), _scene, IS::Physics::CollisionType::COLLISION_ACCURATE));
		object2->addComponent<IS::GameObject::Component::DestroyPhysicDynamic>(destroyPhysic2);
		_scene->addObject(object2);
		_charaVec.push_back(object2);
		IS::GameObject::GameObject *object3 = new IS::GameObject::GameObject(_sceneManager);
		object3->getSceneNode()->setScale(Ogre::Vector3(20.0f));
		object3->getSceneNode()->setPosition(Ogre::Vector3(-233, 0, -189));
		object3->getSceneNode()->setDirection(Ogre::Vector3(0, 0, -1));
		IS::GameObject::Component::MeshComponent *mesh3 = new IS::GameObject::Component::MeshComponent(object3->getSceneNode(), _scene, _sceneManager->getSceneManager(), "Chara_3", "character_default_p3.mesh");
		object3->addComponent<IS::GameObject::Component::MeshComponent>(mesh3);
		if (_nbPlayer >= 3)
		{
			IS::GameObject::Component::Player *player3 = new IS::GameObject::Component::Player(object3->getSceneNode(), _scene, 1, IS::GameObject::Component::AAction::Dir::UP, mesh3->getEntity());
			player3->setSpeed(3);
			player3->setNbLife(_nbLife);
			object3->addComponent<IS::GameObject::Component::Player>(player3);
		}
		else
		{
			IS::GameObject::Component::LuaComponent *Lua3 = new IS::GameObject::Component::LuaComponent(object3->getSceneNode(), _scene, _sceneManager->getSceneManager(), "lua/test3.lua", -4, IS::GameObject::Component::AAction::Dir::UP, mesh3->getEntity(), _map);
			Lua3->setSpeed(3);
			Lua3->setNbLife(_nbLife);
			object3->addComponent<IS::GameObject::Component::Player>(Lua3);
		}
		IS::GameObject::Component::DestroyPhysicDynamic *destroyPhysic3 = new IS::GameObject::Component::DestroyPhysicDynamic(object3->getSceneNode(), _scene, IS::Physics::newPhysicObject<IS::Physics::DynamicObject>(object3->getSceneNode(), _scene, IS::Physics::CollisionType::COLLISION_ACCURATE));
		object3->addComponent<IS::GameObject::Component::DestroyPhysicDynamic>(destroyPhysic3);
		_scene->addObject(object3);
		_charaVec.push_back(object3);

		IS::GameObject::GameObject *object4 = new IS::GameObject::GameObject(_sceneManager);
		object4->getSceneNode()->setScale(Ogre::Vector3(20.0f));
		object4->getSceneNode()->setPosition(Ogre::Vector3(345, 0, -189));
		object4->getSceneNode()->setDirection(Ogre::Vector3(0, 0, 1));
		IS::GameObject::Component::MeshComponent *mesh4 = new IS::GameObject::Component::MeshComponent(object4->getSceneNode(), _scene, _sceneManager->getSceneManager(), "Chara_4", "character_default_p4.mesh");
		object4->addComponent<IS::GameObject::Component::MeshComponent>(mesh4);
		if (_nbPlayer >= 4)
		{
			IS::GameObject::Component::Player *player4 = new IS::GameObject::Component::Player(object4->getSceneNode(), _scene, 2, IS::GameObject::Component::AAction::Dir::DOWN, mesh4->getEntity());
			player4->setSpeed(3);
			player4->setNbLife(_nbLife);
			object4->addComponent<IS::GameObject::Component::Player>(player4);
		}
		else
		{
			IS::GameObject::Component::LuaComponent *Lua4 = new IS::GameObject::Component::LuaComponent(object4->getSceneNode(), _scene, _sceneManager->getSceneManager(), "lua/test4.lua", -5, IS::GameObject::Component::AAction::Dir::UP, mesh4->getEntity(), _map);
			Lua4->setSpeed(3);
			Lua4->setNbLife(_nbLife);
			object4->addComponent<IS::GameObject::Component::Player>(Lua4);
		}
		IS::GameObject::Component::DestroyPhysicDynamic *destroyPhysic4 = new IS::GameObject::Component::DestroyPhysicDynamic(object4->getSceneNode(), _scene, IS::Physics::newPhysicObject<IS::Physics::DynamicObject>(object4->getSceneNode(), _scene, IS::Physics::CollisionType::COLLISION_ACCURATE));
		object4->addComponent<IS::GameObject::Component::DestroyPhysicDynamic>(destroyPhysic4);
		_scene->addObject(object4);
		_charaVec.push_back(object4);
	_isPlaying = true;
}

void IS::GameObject::Component::Rule::deleteChara()
{
	for (size_t i = 0; i < _charaVec.size(); i++)
	{
		_charaVec[i]->removeComponent<IS::GameObject::Component::DestroyPhysicDynamic>();
		_charaVec[i]->removeComponent<IS::GameObject::Component::Player>();
		_charaVec.erase(std::remove(_charaVec.begin(), _charaVec.end(), _charaVec[i]), _charaVec.end());
	}
}

void IS::GameObject::Component::Rule::setLife(int nbLife)
{
	_nbLife = nbLife;
}

int IS::GameObject::Component::Rule::getLife() const
{
	return _nbLife;
}

void IS::GameObject::Component::Rule::setPlayer(int nbPlayer)
{
	_nbPlayer = nbPlayer;
}

int IS::GameObject::Component::Rule::getPlayer() const
{
	return _nbPlayer;
}

std::shared_ptr<IS::Map> IS::GameObject::Component::Rule::getMap() const
{
	return _map;
}
