#include <sstream>
#include <memory>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <IS/Scene/Scenes/Menu.h>
#include <IS/GameObject/GameObject.h>
#include <IS/GameObject/Components/Mesh/MeshComponent.h>
#include <IS/GameObject/Components/Action/AAction.h>
#include <IS/GameObject/Components/Player/Player.h>
#include <IS/GameObject/Components/Lua/LuaComponent.h>
#include <IS/Physics/Core/PhysicObject.h>
#include <IS/Scene/DotSceneLoader.h>
#include <IS/GameObject/Components/PowerUp/PowerUp.h>
#include <IS/GameObject/Components/DestroyPhysic/DestroyPhysic.h>
#include <IS/GUI/GUI.hh>
#include <IS/Application/Application.h>

IS::Scenes::Menu::Menu(Ogre::Root *root, Ogre::RenderWindow *renderWindow, bool isSplash) : IS::Scene::Scene(root, renderWindow)
{
	good = false;
	timer = 7.0f;
	_renderWindow = renderWindow;
	_root = root;
	
	#ifdef _WIN32
	if (isSplash)
	{
	//	Splash = new IS::GameObject::GameObject(_sceneManager);
		particleSystem = _sceneManager->getSceneManager()->createParticleSystem("SplashPar", "SplashScreen");
		_sceneManager->getSceneManager()->getRootSceneNode()->attachObject(particleSystem);
		//Splash->getSceneNode()->attachObject(particleSystem);
		//addObject(Splash);
	}
	else
	{
		timer = -1;
	}
	#else
	init();
	InputManager::InputListener::getInstance()->initOIS(_renderWindow, _root);
	#endif
}

void IS::Scenes::Menu::init()
{
	Ogre::SceneNode	*node = _sceneManager->getSceneManager()->getRootSceneNode()->createChildSceneNode();
	_rule = new IS::GameObject::Component::Rule(this, _sceneManager, 4, 4);
	Gui::GuiMenu::createGUI(_root, _rule, this, _renderWindow);
	
	CEGUI::Logger::getSingleton().setLogFilename("");
	node->setScale(Ogre::Vector3(20.0f, 20.0f, 20.0f));
	_sceneManager->setAmbiantLight(.2f, .2f, .2f);
	Ogre::Light *spotLight = _sceneManager->getSceneManager()->createLight("SpotLight");
	spotLight->setDiffuseColour(1.0, 1.0, 1.0);
	spotLight->setSpecularColour(1.0, 1.0, 1.0);
	spotLight->setType(Ogre::Light::LT_DIRECTIONAL);
	spotLight->setDirection(-1, -1, -1);
	spotLight->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(50));
}

IS::Scenes::Menu::~Menu()
{
}

void IS::Scenes::Menu::update()
{
	#ifdef _WIN32
	if (!good)
	{
		timer -= IS::Utils::Time::getInstance()->getDeltaTime();
	}
	if (!good && timer < 0)
	{
//		_sceneManager->getSceneManager()->clearScene();
		//	IS::Application::replay();
//			std::cout << "delete splah" << std::endl;
	//		Splash->getSceneNode()->setPosition(100000, 100000, 1000000);

			_sceneManager->getSceneManager()->destroyAllParticleSystems();
//		Splash->getSceneNode()->setVisible(false);
//			particleSystem->setVisible(false);
		init();
		InputManager::InputListener::getInstance()->initOIS(_renderWindow, _root);
		good = true;
	}
	if (good)
	{
		IS::Scene::Scene::update();
		_rule->updateRule();
	}
	#else
	IS::Scene::Scene::update();
	_rule->updateRule();
	#endif
}