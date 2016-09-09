#include <exception>
#include <IS/Application/Application.h>
#include <IS/Scene/Scenes/Menu.h>
#include <IS/input/Input.hh>
#include <IS/Audio/Manager.h>
#include <IS/GUI/GUI.hh>
#include <IS/Audio/Audio.h>
#include <IS/Audio/Playlist.h>

static IS::Scene::Scene	*_currentScene;
static bool			_isReset;

bool	IS::Application::_loop = true;
bool	IS::Application::_isReset = false;
IS::Scene::Scene	*IS::Application::_currentScene = nullptr;
Ogre::Root			*IS::Application::_root = nullptr;
Ogre::RenderWindow	*IS::Application::_window = nullptr;
IS::Physics *IS::Application::_physicEngine;

IS::Application::Application()
{
	_physicEngine = new IS::Physics;

	Ogre::LogManager * lm = new Ogre::LogManager();
	lm->createLog(".Ogre.log", true, false, false);
	_root = new Ogre::Root();
	Ogre::RenderSystem *rs;
	_root->loadPlugin("libs/RenderSystem_GL");
	_root->loadPlugin("libs/Plugin_ParticleFX");
	if (!(rs = _root->getRenderSystemByName("OpenGL Rendering Subsystem")))
		throw std::exception();
	_root->setRenderSystem(rs);
	rs->setConfigOption("Full Screen", "No");
	rs->setConfigOption("Video Mode", "1280 x 720 @ 32-bit colour");
	rs->setConfigOption("VSync", "Yes");
	_root->addFrameListener(this);
	_window = _root->initialise(true, "Pulse - Story of the bomb");
	setupResources();
	_time = IS::Utils::Time::getInstance();
	_currentScene = new IS::Scenes::Menu(_root, _window, true);
}

IS::Application::~Application()
{
	IS::Utils::Time::Destroy();
	delete _physicEngine;
}

void	IS::Application::loop()
{
		IS::Audio::PlayList::getInstance()->Play();

	while (_loop)
	{
		Ogre::WindowEventUtilities::messagePump();

		if (!_root->renderOneFrame() || _window->isClosed())
		{
			_loop = false;
			break;
		}
	}
}

bool IS::Application::frameStarted(const Ogre::FrameEvent & evt)
{
	_time->update();
	return true;
}

bool IS::Application::frameRenderingQueued(const Ogre::FrameEvent & evt)
{
	if (_currentScene && !_isReset)
		_currentScene->update();
	return true;
}

void	IS::Application::quit()
{
	_loop = false;
}

void IS::Application::setupResources()
{
	Ogre::ConfigFile configFile;
	configFile.load("resources.cfg");

	Ogre::ConfigFile::SectionIterator seci = configFile.getSectionIterator();
	Ogre::String secName, typeName, archName;

	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
		}
	}
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void IS::Application::replay()
{
	IS::Scene::Scene *sc = _currentScene;

	_isReset = true;
	_currentScene = nullptr;
	delete _physicEngine;
	_physicEngine = new IS::Physics;
	delete sc;
	IS::Utils::Time::getInstance()->setRatio(1.0f);
	_currentScene = new IS::Scenes::Menu(_root, _window, false);
	_isReset = false;
}