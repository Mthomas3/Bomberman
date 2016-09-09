#include <OGRE/Ogre.h>
#include <IS/GUI/GUI.hh>
#include <IS/Application/Application.h>
#include <IS/Audio/Playlist.h>
#include <cstdlib>

Gui::GuiMenu *Gui::GuiMenu::_instance = nullptr;
IS::GameObject::Component::Rule *Gui::GuiMenu::_rule = nullptr;
IS::Scene::Scene *Gui::GuiMenu::_scene = nullptr;
Ogre::RenderWindow *Gui::GuiMenu::_rd = nullptr;
uint8_t Gui::GuiMenu::_nbLife = 0;
uint8_t Gui::GuiMenu::_id = 0;
uint8_t	Gui::GuiMenu::_nbBomb = 0;
CEGUI::Window *Gui::GuiMenu::background = nullptr;
CEGUI::Window **Gui::GuiMenu::_ids = new CEGUI::Window*[12];
uint8_t Gui::GuiMenu::_currentPowerUp = 0;
uint8_t Gui::GuiMenu::_currentBomb = 0;
uint8_t	Gui::GuiMenu::_currentLife = 0;
std::map<int, Ogre::Vector3> Gui::GuiMenu::_statePlayers;
uint8_t Gui::GuiMenu::_currentMusic;

#pragma region GUI Singleton
Gui::GuiMenu::~GuiMenu()
{
	for (auto screen : _screen)
		clearScreen(screen.first);
}

void Gui::GuiMenu::deleteGUI()
{
	if (Gui::GuiMenu::_instance == nullptr)
		return;
	Gui::GuiMenu *instance = _instance;
	delete(instance);
}

void Gui::GuiMenu::pause()
{
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().draw();
	Gui::GuiMenu::_instance->chargeScreen("Pause");
}
#pragma endregion

#pragma region GUI Runtime
void	Gui::GuiMenu::run(void)
{
	//std::cout << "' --- S T A R T   G U I --- '" << std::endl;
	try
	{
		if (_renderCegui == nullptr)
		{
			_renderCegui = &CEGUI::OgreRenderer::bootstrapSystem();
			_wmgr = &CEGUI::WindowManager::getSingleton();
			CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
			CEGUI::Font::setDefaultResourceGroup("Fonts");
			CEGUI::Scheme::setDefaultResourceGroup("Schemes");
			CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
			CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
			CEGUI::AnimationManager::setDefaultResourceGroup("Animation");
		}

		for (int i = 0; i < 12; i++)
			_ids[i] = nullptr;

		CEGUI::Logger::getSingleton().setLoggingLevel((CEGUI::LoggingLevel)3);

		setFileScheme("TaharezLook.scheme");
		setImageToMouse("TaharezLook/CursorNormal");

		_constructors["Main"] = &GuiMenu::createMainMenu;
		_constructors["Pause"] = &GuiMenu::createPauseMenu;
		_constructors["Play"] = &GuiMenu::createPlayMenu;
		_constructors["MultiPlay"] = &GuiMenu::createMultiPlayMenu;
		_constructors["Option"] = &GuiMenu::createOptionMenu;
		_constructors["Options"] = &GuiMenu::createOptionMenus;
		_constructors["Players"] = &GuiMenu::createPlayers;
		_constructors["End"] = &GuiMenu::createEndGame;

		chargeScreen("Main");
		_currentNbPlayer = 1;
		_multiNbPlayer = 1;
		_nbLife = 1;
		_nbBomb = 0;
	}
	catch (std::exception &e)
	{
		std::cout << "Error" << ": " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Unknow error" << std::endl;
	}
	//std::cout << "' ---   E N D   G U I   --- '" << std::endl;
}
#pragma endregion

#pragma region GUI Init

void Gui::GuiMenu::displayEnd()
{
	chargeScreen("End");
}

void Gui::GuiMenu::displayPlayers()
{
	chargeScreen("Players");
}

void Gui::GuiMenu::createMainMenu()
{
	_screen["Main"] = _wmgr->createWindow("DefaultWindow", "Bomberman/MainMenu");
	propertyList properties;
	eventList events;

	properties["NormalImage"] = "TaharezLook/BigButtonNormal";
	properties["HoverImage"] = "TaharezLook/BigButtonHover";
	properties["PushedImage"] = "TaharezLook/BigButtonPushed";

	//CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(0, 0);
	events[CEGUI::PushButton::EventClicked] = &GuiMenu::play;
	_screen["Main"]->addChild(createButton(CEGUI::USize(CEGUI::UDim(0.24f, 0), CEGUI::UDim(0.05f, 0)),
		CEGUI::UVector2(CEGUI::UDim(0.0208f, 0), CEGUI::UDim(0.6534f, 0)),
		"PLAY", properties, events));

	events[CEGUI::PushButton::EventClicked] = &GuiMenu::multiplay;
	_screen["Main"]->addChild(createButton(CEGUI::USize(CEGUI::UDim(0.24f, 0), CEGUI::UDim(0.05f, 0)),
		CEGUI::UVector2(CEGUI::UDim(0.0208f, 0), CEGUI::UDim(0.7103f, 0)),
		"MULTIPLAYER", properties, events));

	events[CEGUI::PushButton::EventClicked] = &GuiMenu::options;
	_screen["Main"]->addChild(createButton(CEGUI::USize(CEGUI::UDim(0.24f, 0), CEGUI::UDim(0.05f, 0)),
		CEGUI::UVector2(CEGUI::UDim(0.0208f, 0), CEGUI::UDim(0.8241f, 0)),
		"OPTIONS", properties, events));

	events[CEGUI::PushButton::EventClicked] = &GuiMenu::exit;
	_screen["Main"]->addChild(createButton(CEGUI::USize(CEGUI::UDim(0.24f, 0), CEGUI::UDim(0.05f, 0)),
		CEGUI::UVector2(CEGUI::UDim(0.0208f, 0), CEGUI::UDim(0.881f, 0)),
		"QUIT", properties, events));
}

void Gui::GuiMenu::createPauseMenu()
{
	_screen["Pause"] = _wmgr->createWindow("DefaultWindow", "Bomberman/PauseMenu");
	propertyList properties;
	eventList events;

	properties["NormalImage"] = "TaharezLook/BigButtonNormal";
	properties["HoverImage"] = "TaharezLook/BigButtonHover";
	properties["PushedImage"] = "TaharezLook/BigButtonPushed";

	events[CEGUI::PushButton::EventClicked] = &GuiMenu::resumePause;
	_screen["Pause"]->addChild(createButton(CEGUI::USize(CEGUI::UDim(0, 307), CEGUI::UDim(0, 35)),
		CEGUI::UVector2(CEGUI::UDim(0.5f, -153.5f), CEGUI::UDim(0.5f, -57.5)),
		"RESUME", properties, events));

	events[CEGUI::PushButton::EventClicked] = &GuiMenu::optionPause;
	_screen["Pause"]->addChild(createButton(CEGUI::USize(CEGUI::UDim(0, 307), CEGUI::UDim(0, 35)),
		CEGUI::UVector2(CEGUI::UDim(0.5f, -153.5f), CEGUI::UDim(0.5f, 22.5)),
		"OPTIONS", properties, events));

	events[CEGUI::PushButton::EventClicked] = &GuiMenu::deleteGame;
	_screen["Pause"]->addChild(createButton(CEGUI::USize(CEGUI::UDim(0, 307), CEGUI::UDim(0, 35)),
		CEGUI::UVector2(CEGUI::UDim(0.5f, -153.5f), CEGUI::UDim(0.5f, 62.5)),
		"QUIT", properties, events));
}


void Gui::GuiMenu::createPlayers()
{
	_screen["Players"] = _wmgr->createWindow("DefaultWindow", "Bomberman/PlayersMenu");

	if (_currentNbPlayer == 1)
	{
		_screen["Players"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0.04f, 0), CEGUI::UDim(0.10f, 0)),
			CEGUI::UVector2(CEGUI::UDim(0.001f, 0), CEGUI::UDim(0.8800f, 0)), "TaharezLook/FireUp", "Players"));
		_screen["Players"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0.05f, 0), CEGUI::UDim(0.09f, 0)),
			CEGUI::UVector2(CEGUI::UDim(0.001f, 0), CEGUI::UDim(0.7250f, 0)), "TaharezLook/HeartNormal", "Players"));
		_screen["Players"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0.05f, 0), CEGUI::UDim(0.09f, 0)),
			CEGUI::UVector2(CEGUI::UDim(0.09f, 0), CEGUI::UDim(0.8900f, 0)), "TaharezLook/BombLocked", "Players"));

	}
	else if (_currentNbPlayer == 2)
	{
		_screen["Players"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0.04f, 0), CEGUI::UDim(0.10f, 0)),
			CEGUI::UVector2(CEGUI::UDim(0.001f, 0), CEGUI::UDim(0.8800f, 0)), "TaharezLook/FireUp", "Players"));
		_screen["Players"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0.05f, 0), CEGUI::UDim(0.09f, 0)),
			CEGUI::UVector2(CEGUI::UDim(0.001f, 0), CEGUI::UDim(0.7250f, 0)), "TaharezLook/HeartNormal", "Players"));
		_screen["Players"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0.05f, 0), CEGUI::UDim(0.09f, 0)),
			CEGUI::UVector2(CEGUI::UDim(0.09f, 0), CEGUI::UDim(0.8900f, 0)), "TaharezLook/BombLocked", "Players"));


		_screen["Players"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0.04f, 0), CEGUI::UDim(0.10f, 0)),
			CEGUI::UVector2(CEGUI::UDim(0.001f, 0), CEGUI::UDim(0.0100f, 0)), "TaharezLook/FireUp", "Players"));
		_screen["Players"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0.05f, 0), CEGUI::UDim(0.09f, 0)),
			CEGUI::UVector2(CEGUI::UDim(0.001f, 0), CEGUI::UDim(0.1750f, 0)), "TaharezLook/HeartNormal", "Players"));
		_screen["Players"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0.05f, 0), CEGUI::UDim(0.09f, 0)),
			CEGUI::UVector2(CEGUI::UDim(0.09f, 0), CEGUI::UDim(0.0100f, 0)), "TaharezLook/BombLocked", "Players"));
	}
	else if (_currentNbPlayer == 3)
	{

		_screen["Players"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0.04f, 0), CEGUI::UDim(0.10f, 0)),
			CEGUI::UVector2(CEGUI::UDim(0.001f, 0), CEGUI::UDim(0.8800f, 0)), "TaharezLook/FireUp", "Players"));
		_screen["Players"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0.05f, 0), CEGUI::UDim(0.09f, 0)),
			CEGUI::UVector2(CEGUI::UDim(0.001f, 0), CEGUI::UDim(0.7250f, 0)), "TaharezLook/HeartNormal", "Players"));
		_screen["Players"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0.05f, 0), CEGUI::UDim(0.09f, 0)),
			CEGUI::UVector2(CEGUI::UDim(0.09f, 0), CEGUI::UDim(0.8900f, 0)), "TaharezLook/BombLocked", "Players"));


		_screen["Players"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0.04f, 0), CEGUI::UDim(0.10f, 0)),
			CEGUI::UVector2(CEGUI::UDim(0.001f, 0), CEGUI::UDim(0.0100f, 0)), "TaharezLook/FireUp", "Players"));
		_screen["Players"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0.05f, 0), CEGUI::UDim(0.09f, 0)),
			CEGUI::UVector2(CEGUI::UDim(0.001f, 0), CEGUI::UDim(0.1750f, 0)), "TaharezLook/HeartNormal", "Players"));
		_screen["Players"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0.05f, 0), CEGUI::UDim(0.09f, 0)),
			CEGUI::UVector2(CEGUI::UDim(0.09f, 0), CEGUI::UDim(0.0100f, 0)), "TaharezLook/BombLocked", "Players"));


		_screen["Players"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0.04f, 0), CEGUI::UDim(0.10f, 0)),
			CEGUI::UVector2(CEGUI::UDim(0.95f, 0), CEGUI::UDim(0.0100f, 0)), "TaharezLook/FireUp", "Players"));
		_screen["Players"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0.05f, 0), CEGUI::UDim(0.09f, 0)),
			CEGUI::UVector2(CEGUI::UDim(0.94f, 0), CEGUI::UDim(0.1800f, 0)), "TaharezLook/HeartNormal", "Players"));
		_screen["Players"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0.05f, 0), CEGUI::UDim(0.09f, 0)),
			CEGUI::UVector2(CEGUI::UDim(0.85f, 0), CEGUI::UDim(0.0100f, 0)), "TaharezLook/BombLocked", "Players"));
	}
	else if (_currentNbPlayer == 4)
	{
		_screen["Players"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0.04f, 0), CEGUI::UDim(0.10f, 0)),
			CEGUI::UVector2(CEGUI::UDim(0.001f, 0), CEGUI::UDim(0.8800f, 0)), "TaharezLook/FireUp", "Players"));
		_screen["Players"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0.05f, 0), CEGUI::UDim(0.09f, 0)),
			CEGUI::UVector2(CEGUI::UDim(0.001f, 0), CEGUI::UDim(0.7250f, 0)), "TaharezLook/HeartNormal", "Players"));
		_screen["Players"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0.05f, 0), CEGUI::UDim(0.09f, 0)),
			CEGUI::UVector2(CEGUI::UDim(0.09f, 0), CEGUI::UDim(0.8900f, 0)), "TaharezLook/BombLocked", "Players"));


		_screen["Players"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0.04f, 0), CEGUI::UDim(0.10f, 0)),
			CEGUI::UVector2(CEGUI::UDim(0.001f, 0), CEGUI::UDim(0.0100f, 0)), "TaharezLook/FireUp", "Players"));
		_screen["Players"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0.05f, 0), CEGUI::UDim(0.09f, 0)),
			CEGUI::UVector2(CEGUI::UDim(0.001f, 0), CEGUI::UDim(0.1750f, 0)), "TaharezLook/HeartNormal", "Players"));
		_screen["Players"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0.05f, 0), CEGUI::UDim(0.09f, 0)),
			CEGUI::UVector2(CEGUI::UDim(0.09f, 0), CEGUI::UDim(0.0100f, 0)), "TaharezLook/BombLocked", "Players"));


		_screen["Players"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0.04f, 0), CEGUI::UDim(0.10f, 0)),
			CEGUI::UVector2(CEGUI::UDim(0.95f, 0), CEGUI::UDim(0.0100f, 0)), "TaharezLook/FireUp", "Players"));
		_screen["Players"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0.05f, 0), CEGUI::UDim(0.09f, 0)),
			CEGUI::UVector2(CEGUI::UDim(0.94f, 0), CEGUI::UDim(0.1800f, 0)), "TaharezLook/HeartNormal", "Players"));
		_screen["Players"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0.05f, 0), CEGUI::UDim(0.09f, 0)),
			CEGUI::UVector2(CEGUI::UDim(0.85f, 0), CEGUI::UDim(0.0100f, 0)), "TaharezLook/BombLocked", "Players"));


		_screen["Players"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0.04f, 0), CEGUI::UDim(0.10f, 0)),
			CEGUI::UVector2(CEGUI::UDim(0.95f, 0), CEGUI::UDim(0.8850f, 0)), "TaharezLook/FireUp", "Players"));
		_screen["Players"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0.05f, 0), CEGUI::UDim(0.09f, 0)),
			CEGUI::UVector2(CEGUI::UDim(0.94f, 0), CEGUI::UDim(0.7300f, 0)), "TaharezLook/HeartNormal", "Players"));
		_screen["Players"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0.05f, 0), CEGUI::UDim(0.09f, 0)),
			CEGUI::UVector2(CEGUI::UDim(0.85f, 0), CEGUI::UDim(0.8900f, 0)), "TaharezLook/BombLocked", "Players"));
	}

}

void Gui::GuiMenu::createEndGame()
{
	_screen["End"] = _wmgr->createWindow("DefaultWindow", "Bomberman/EndGame");

	propertyList properties;
	eventList events;

	properties["NormalImage"] = "TaharezLook/BigButtonNormal";
	properties["HoverImage"] = "TaharezLook/BigButtonHover";
	properties["PushedImage"] = "TaharezLook/BigButtonPushed";

	events[CEGUI::PushButton::EventClicked] = &GuiMenu::main;
	_screen["End"]->addChild(createButton(CEGUI::USize(CEGUI::UDim(0.24f, 0), CEGUI::UDim(0.05f, 0)),
		CEGUI::UVector2(CEGUI::UDim(0.38f, 0), CEGUI::UDim(0.5604f, 0)),
		"RETURN", properties, events));
}


void Gui::GuiMenu::createPlayMenu()
{
	_screen["Play"] = _wmgr->createWindow("DefaultWindow", "Bomberman/Play");
	propertyList properties;
	eventList events;

	properties["NormalImage"] = "TaharezLook/HeartNormal";
	properties["HoverImage"] = "TaharezLook/HeartHover";
	properties["PushedImage"] = "TaharezLook/HeartHover";

	events[CEGUI::PushButton::EventMouseClick] = &GuiMenu::changeNbLife;
	_screen["Play"]->addChild(createButton(CEGUI::USize(CEGUI::UDim(0, 147), CEGUI::UDim(0, 122)),
		CEGUI::UVector2(CEGUI::UDim(0.50f, -74), CEGUI::UDim(0.15f, -61)),
		"", properties, events));

	events.clear();
	properties["NormalImage"] = "TaharezLook/BigButtonNormal";
	properties["HoverImage"] = "TaharezLook/BigButtonHover";
	properties["PushedImage"] = "TaharezLook/BigButtonPushed";

	events[CEGUI::PushButton::EventClicked] = &GuiMenu::launch;
	_screen["Play"]->addChild(createButton(CEGUI::USize(CEGUI::UDim(0, 307), CEGUI::UDim(0, 35)),
		CEGUI::UVector2(CEGUI::UDim(0.5f, -153.5f), CEGUI::UDim(0.75f, 22.5)),
		"PLAY", properties, events));

	events[CEGUI::PushButton::EventClicked] = &GuiMenu::resume;
	_screen["Play"]->addChild(createButton(CEGUI::USize(CEGUI::UDim(0, 307), CEGUI::UDim(0, 35)),
		CEGUI::UVector2(CEGUI::UDim(0.5f, -153.5f), CEGUI::UDim(0.75f, 62.5)),
		"RETURN", properties, events));

	for (uint8_t i = 0; i < _nbLife; i++)
		_screen["Play"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0, 88), CEGUI::UDim(0, 73)),
			CEGUI::UVector2(CEGUI::UDim(0.25f + 0.08333 * static_cast<float>((i) % 7), -44),
				CEGUI::UDim(0.35f + 0.15 * static_cast<float>((i) / 7), -36.5f)), "TaharezLook/HeartPushed"));
}


void Gui::GuiMenu::createOptionMenu(void)
{
	_screen["Option"] = _wmgr->createWindow("DefaultWindow", "Bomberman/Option");
	propertyList properties;
	eventList events;

	properties["NormalImage"] = "TaharezLook/OptionBgNormal";
	properties["HoverImage"] = "TaharezLook/OptionBgNormalMarked";
	properties["MarkImage"] = "TaharezLook/OptionBgNormalSelected";

	events[CEGUI::ToggleButton::EventSelectStateChanged] = &GuiMenu::changeCam;

	auto checkbox = createCheckbox(CEGUI::USize(CEGUI::UDim(0, 220), CEGUI::UDim(0, 220)),
		CEGUI::UVector2(CEGUI::UDim(0.50399f, 0), CEGUI::UDim(0.3440f, 0)),
		CEGUI::Quaternion::eulerAnglesDegrees(0, 0, 45),
		properties, events);

	auto label = writeTextAt("FreeCam", CEGUI::UVector2(CEGUI::UDim(0.25399f, 0), CEGUI::UDim(0.3440f, 0)));

	label->setRotation(CEGUI::Quaternion::eulerAnglesDegrees(0, 0, -45));
	checkbox->addChild(label);
	_screen["Option"]->addChild(checkbox);

	events[CEGUI::ToggleButton::EventSelectStateChanged] = &GuiMenu::changeScreen;
	auto checkboxs = createCheckbox(CEGUI::USize(CEGUI::UDim(0, 220), CEGUI::UDim(0, 220)),
		CEGUI::UVector2(CEGUI::UDim(0.2530f, 0), CEGUI::UDim(0.3450f, 0)),
		CEGUI::Quaternion::eulerAnglesDegrees(0, 0, 45),
		properties, events);

	auto labels = writeTextAt("Fullscreen", CEGUI::UVector2(CEGUI::UDim(0.25399f, 0), CEGUI::UDim(0.4440f, 0)));
	labels->setRotation(CEGUI::Quaternion::eulerAnglesDegrees(0, 0, -45));
	checkboxs->addChild(labels);
	_screen["Option"]->addChild(checkboxs);

	properties.clear();
	events.clear();

	_screen["Option"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0, 220), CEGUI::UDim(0, 220)),
		CEGUI::UVector2(CEGUI::UDim(0.3800f, 0), CEGUI::UDim(0.1200f, 0)),
		CEGUI::Quaternion::eulerAnglesDegrees(0, 0, 45),
		"TaharezLook/OptionBgNormal"));

	_screen["Option"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0, 220), CEGUI::UDim(0, 220)),
		CEGUI::UVector2(CEGUI::UDim(0.3800f, 0), CEGUI::UDim(0.5720f, 0)),
		CEGUI::Quaternion::eulerAnglesDegrees(0, 0, 45),
		"TaharezLook/OptionBgNormal"));

	_screen["Option"]->addChild(writeTextAt("Sound", CEGUI::UVector2(CEGUI::UDim(0.4250f, 0), CEGUI::UDim(0.1438f, 0))));
	_screen["Option"]->addChild(writeTextAt("Music", CEGUI::UVector2(CEGUI::UDim(0.4250f, 0), CEGUI::UDim(0.6000f, 0))));

	events[CEGUI::Slider::EventValueChanged] = &Gui::GuiMenu::changeMusic;
	events[CEGUI::Slider::EventThumbTrackEnded] = &Gui::GuiMenu::changeMusic;

	_screen["Option"]->addChild(createSlider(CEGUI::USize(CEGUI::UDim(0, 150), CEGUI::UDim(0, 17)),
		CEGUI::UVector2(CEGUI::UDim(0.4050f, 0), CEGUI::UDim(0.7000f, 0)), events));

	events[CEGUI::Slider::EventValueChanged] = &Gui::GuiMenu::changeSound;
	events[CEGUI::Slider::EventThumbTrackEnded] = &Gui::GuiMenu::changeSound;

	_screen["Option"]->addChild(createSlider(CEGUI::USize(CEGUI::UDim(0, 140), CEGUI::UDim(0, 17)),
		CEGUI::UVector2(CEGUI::UDim(0.4050f, 0), CEGUI::UDim(0.2500f, 0)), events));

	properties.clear();
	events.clear();

	properties["NormalImage"] = "TaharezLook/MediaPrev";
	properties["HoverImage"] = "TaharezLook/MediaPrev";
	properties["PushedImage"] = "TaharezLook/MediaPrev";

	events[CEGUI::PushButton::EventClicked] = &Gui::GuiMenu::mediaPrev;
	_screen["Option"]->addChild(createButton(CEGUI::USize(CEGUI::UDim(0.03f, 0), CEGUI::UDim(0.03f, 0)),
		CEGUI::UVector2(CEGUI::UDim(0.3772f, 0), CEGUI::UDim(0.6955f, 0)), "", properties, events));

	properties.clear();
	events.clear();

	properties["NormalImage"] = "TaharezLook/MediaNext";
	properties["HoverImage"] = "TaharezLook/MediaNext";
	properties["PushedImage"] = "TaharezLook/MediaNext";

	events[CEGUI::PushButton::EventClicked] = &Gui::GuiMenu::mediaNext;
	_screen["Option"]->addChild(createButton(CEGUI::USize(CEGUI::UDim(0.03f, 0), CEGUI::UDim(0.03f, 0)),
		CEGUI::UVector2(CEGUI::UDim(0.5150f, 0), CEGUI::UDim(0.6955f, 0)), "", properties, events));

	properties.clear();
	events.clear();

	properties["NormalImage"] = "TaharezLook/BigButtonNormal";
	properties["HoverImage"] = "TaharezLook/BigButtonHover";
	properties["PushedImage"] = "TaharezLook/BigButtonPushed";

	events[CEGUI::PushButton::EventClicked] = &GuiMenu::play;
	events[CEGUI::PushButton::EventClicked] = &GuiMenu::main;
	_screen["Option"]->addChild(createButton(CEGUI::USize(CEGUI::UDim(0.24f, 0), CEGUI::UDim(0.05f, 0)),
		CEGUI::UVector2(CEGUI::UDim(0.35f, 0), CEGUI::UDim(0.9400f, 0)),
		"RETURN", properties, events));
}

void Gui::GuiMenu::createOptionMenus(void)
{
	_screen["Options"] = _wmgr->createWindow("DefaultWindow", "Bomberman/Option");
	propertyList properties;
	eventList events;

	properties["NormalImage"] = "TaharezLook/OptionBgNormal";
	properties["HoverImage"] = "TaharezLook/OptionBgNormalMarked";
	properties["MarkImage"] = "TaharezLook/OptionBgNormalSelected";

	events[CEGUI::ToggleButton::EventSelectStateChanged] = &GuiMenu::changeScreen;
	auto checkbox = createCheckbox(CEGUI::USize(CEGUI::UDim(0, 220), CEGUI::UDim(0, 220)),
		CEGUI::UVector2(CEGUI::UDim(0.5f, 54), CEGUI::UDim(0.5f, -110)),
		CEGUI::Quaternion::eulerAnglesDegrees(0, 0, 45),
		properties, events);
	auto label = writeTextAt("Fullscreen", CEGUI::UVector2(CEGUI::UDim(0.5f, -45), CEGUI::UDim(0.5, -25)));
	label->setRotation(CEGUI::Quaternion::eulerAnglesDegrees(0, 0, -45));
	checkbox->addChild(label);
	_screen["Options"]->addChild(checkbox);

	events[CEGUI::ToggleButton::EventSelectStateChanged] = &GuiMenu::changeCam;
	checkbox = createCheckbox(CEGUI::USize(CEGUI::UDim(0, 220), CEGUI::UDim(0, 220)),
		CEGUI::UVector2(CEGUI::UDim(0.5f, -277.126983f), CEGUI::UDim(0.5f, -110)),
		CEGUI::Quaternion::eulerAnglesDegrees(0, 0, 45),
		properties, events);
	label = writeTextAt(" FreeCam  ", CEGUI::UVector2(CEGUI::UDim(0.5f, -45), CEGUI::UDim(0.5, -25)));
	label->setRotation(CEGUI::Quaternion::eulerAnglesDegrees(0, 0, -45));
	checkbox->addChild(label);
	_screen["Options"]->addChild(checkbox);

	properties.clear();
	events.clear();

	_screen["Options"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0, 220), CEGUI::UDim(0, 220)),
		CEGUI::UVector2(CEGUI::UDim(0.5f, -111), CEGUI::UDim(0.5f, 55)),
		CEGUI::Quaternion::eulerAnglesDegrees(0, 0, 45),
		"TaharezLook/OptionBgNormal"));

	_screen["Options"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0, 220), CEGUI::UDim(0, 220)),
		CEGUI::UVector2(CEGUI::UDim(0.5f, -111), CEGUI::UDim(0.5f, -275)),
		CEGUI::Quaternion::eulerAnglesDegrees(0, 0, 45),
		"TaharezLook/OptionBgNormal"));

	_screen["Options"]->addChild(writeTextAt("Sound", CEGUI::UVector2(CEGUI::UDim(0.5f, -47), CEGUI::UDim(0.5f, -250))));
	_screen["Options"]->addChild(writeTextAt("Music", CEGUI::UVector2(CEGUI::UDim(0.5f, -47), CEGUI::UDim(0.5f, 75))));

	events[CEGUI::Slider::EventValueChanged] = &Gui::GuiMenu::changeSound;
	events[CEGUI::Slider::EventThumbTrackEnded] = &Gui::GuiMenu::changeSound;
	_screen["Options"]->addChild(createSlider(CEGUI::USize(CEGUI::UDim(0, 150), CEGUI::UDim(0, 17)),
		CEGUI::UVector2(CEGUI::UDim(0.5f, -75), CEGUI::UDim(0.5f, -160)), events));

	_screen["Options"]->addChild(createSlider(CEGUI::USize(CEGUI::UDim(0, 140), CEGUI::UDim(0, 17)),
		CEGUI::UVector2(CEGUI::UDim(0.5f, -75), CEGUI::UDim(0.5f, 170)), events));

	properties.clear();
	events.clear();
	properties["NormalImage"] = "TaharezLook/BigButtonNormal";
	properties["HoverImage"] = "TaharezLook/BigButtonHover";
	properties["PushedImage"] = "TaharezLook/BigButtonPushed";

	
	events[CEGUI::PushButton::EventClicked] = &GuiMenu::configPause;
	_screen["Options"]->addChild(createButton(CEGUI::USize(CEGUI::UDim(0, 307), CEGUI::UDim(0, 35)),
		CEGUI::UVector2(CEGUI::UDim(0.5f, -153.5f), CEGUI::UDim(1, -40)),
		"RETURN", properties, events));
}

void Gui::GuiMenu::configPause(const CEGUI::EventArgs &e)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().draw();
	chargeScreen("Pause");
}

void Gui::GuiMenu::createMultiPlayMenu()
{
	_screen["MultiPlay"] = _wmgr->createWindow("DefaultWindow", "Bomberman/MultiPlay");
	propertyList properties;
	eventList events;

	_screen["MultiPlay"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0, 97), CEGUI::UDim(0, 109)),
		CEGUI::UVector2(CEGUI::UDim(0.1445f, 0), CEGUI::UDim(0.2681f, 0)), "TaharezLook/BombLocked"));

	properties["NormalImage"] = "TaharezLook/BombUnlocked";
	properties["HoverImage"] = "TaharezLook/BombSelected";
	properties["MarkImage"] = "TaharezLook/BombLocked";
	events[CEGUI::ToggleButton::EventSelectStateChanged] = &GuiMenu::changeNbPlayer;

	_screen["MultiPlay"]->addChild(createCheckbox(CEGUI::USize(CEGUI::UDim(0, 97), CEGUI::UDim(0, 109)),
		CEGUI::UVector2(CEGUI::UDim(0.3476f, 0), CEGUI::UDim(0.2681f, 0)),
		CEGUI::Quaternion::eulerAnglesDegrees(0, 0, 0),
		properties, events));
	_screen["MultiPlay"]->addChild(createCheckbox(CEGUI::USize(CEGUI::UDim(0, 97), CEGUI::UDim(0, 109)),
		CEGUI::UVector2(CEGUI::UDim(0.5507f, 0), CEGUI::UDim(0.2681f, 0)),
		CEGUI::Quaternion::eulerAnglesDegrees(0, 0, 0),
		properties, events));
	_screen["MultiPlay"]->addChild(createCheckbox(CEGUI::USize(CEGUI::UDim(0, 97), CEGUI::UDim(0, 109)),
		CEGUI::UVector2(CEGUI::UDim(0.7539f, 0), CEGUI::UDim(0.2681f, 0)),
		CEGUI::Quaternion::eulerAnglesDegrees(0, 0, 0),
		properties, events));

	properties.clear();
	events.clear();
	properties["NormalImage"] = "TaharezLook/BigButtonNormal";
	properties["HoverImage"] = "TaharezLook/BigButtonHover";
	properties["PushedImage"] = "TaharezLook/BigButtonPushed";

	events[CEGUI::PushButton::EventClicked] = &GuiMenu::play;
	_screen["MultiPlay"]->addChild(createButton(CEGUI::USize(CEGUI::UDim(0, 307), CEGUI::UDim(0, 35)),
		CEGUI::UVector2(CEGUI::UDim(0.5f, -153.5f), CEGUI::UDim(0.75f, 22.5)),
		"PLAY", properties, events));

	events[CEGUI::PushButton::EventClicked] = &GuiMenu::main;
	_screen["MultiPlay"]->addChild(createButton(CEGUI::USize(CEGUI::UDim(0, 307), CEGUI::UDim(0, 35)),
		CEGUI::UVector2(CEGUI::UDim(0.5f, -153.5f), CEGUI::UDim(0.75f, 62.5)),
		"RETURN", properties, events));
}
#pragma endregion

#pragma region GUI Tools

/*void Gui::GuiMenu::writeAxesToPlayer(std::string const &res, const CEGUI::UVector2 &pos)
{
	std::string result(res);
	
	CEGUI::Window *players1 = writeTextAt(result, pos);

	_ids.push_back(players1);
	_screen["Players"]->addChild(players1);
}*/

void Gui::GuiMenu::writePlayersState(void)
{
	std::string result;
	std::stringstream convert;

	/*for (size_t i = 0; i < _ids.size(); i++)
	{
		if (_ids[i] != NULL && _ids[i] != nullptr)
		{
			_ids[i]->setText("");
		}
	}
	_ids.clear();*/

	for (auto state : _statePlayers)
	{
		switch (state.first)
		{
			case 0 :
			{
				convert << static_cast<int>(state.second.x);
				result += convert.str();
				
				if (!_ids[0])
				{
					_ids[0] = writeTextAt(result, CEGUI::UVector2(CEGUI::UDim(0.0350f, 0), CEGUI::UDim(0.9250f, 0)));
					_screen["Players"]->addChild(_ids[0]);
				}
				else
				{
					_ids[0]->setText(result);
				}
				convert.str("");
				convert << static_cast<int>(state.second.y);
				result = convert.str();
				if (!_ids[1])
				{
					_ids[1] = writeTextAt(result, CEGUI::UVector2(CEGUI::UDim(0.1200f, 0), CEGUI::UDim(0.9250f, 0)));
					_screen["Players"]->addChild(_ids[1]);
				}
				else
				{
					_ids[1]->setText(result);
				}
				convert.str("");
				convert << static_cast<int>(state.second.z);
				result = convert.str();

				if (!_ids[2])
				{
					_ids[2] = writeTextAt(result, CEGUI::UVector2(CEGUI::UDim(0.0f, 0), CEGUI::UDim(0.6700f, 0)));
					_screen["Players"]->addChild(_ids[2]);
				}
				else
				{
					_ids[2]->setText(result);
				}
				break;
			}
			case 1 :
			{
				convert << static_cast<int>(state.second.x);
				result = convert.str();
				if (!_ids[3])
				{
					_ids[3] = writeTextAt(result, CEGUI::UVector2(CEGUI::UDim(0.0350f, 0), CEGUI::UDim(0.0100f, 0)));
					_screen["Players"]->addChild(_ids[3]);
				}
				else
				{
					_ids[3]->setText(result);
				}
				convert.str("");
				result.clear();
				convert << static_cast<int>(state.second.y);
				result = convert.str();
				if (!_ids[4])
				{
					_ids[4] = writeTextAt(result, CEGUI::UVector2(CEGUI::UDim(0.1200f, 0), CEGUI::UDim(0.0200f, 0)));
					_screen["Players"]->addChild(_ids[4]);
				}
				else
				{
					_ids[4]->setText(result);
				}
				result.clear();
				convert.str("");
				convert << static_cast<int>(state.second.z);
				result = convert.str();
				if (!_ids[5])
				{
					_ids[5] = writeTextAt(result, CEGUI::UVector2(CEGUI::UDim(0.0350f, 0), CEGUI::UDim(0.2000f, 0)));
					_screen["Players"]->addChild(_ids[5]);
				}
				else
				{
					_ids[5]->setText(result);
				}
				break;
			}
			case 2 :
			{
				convert << static_cast<int>(state.second.x);
				result = convert.str();
				if (!_ids[6])
				{
					_ids[6] = writeTextAt(result, CEGUI::UVector2(CEGUI::UDim(0.9000f, 0), CEGUI::UDim(0.0200f, 0)));
					_screen["Players"]->addChild(_ids[6]);
				}
				else
				{
					_ids[6]->setText(result);
				}
				result.clear();
				convert.str("");
				convert << static_cast<int>(state.second.y);
				result = convert.str();
				if (!_ids[7])
				{
					_ids[7] = writeTextAt(result, CEGUI::UVector2(CEGUI::UDim(0.7800f, 0), CEGUI::UDim(0.0200f, 0)));
					_screen["Players"]->addChild(_ids[7]);
				}
				else
				{
					_ids[7]->setText(result);
				}
				result.clear();
				convert.str("");
				convert << static_cast<int>(state.second.z);
				result = convert.str();
				if (!_ids[8])
				{
					_ids[8] = writeTextAt(result, CEGUI::UVector2(CEGUI::UDim(0.8800f, 0), CEGUI::UDim(0.2000f, 0)));
					_screen["Players"]->addChild(_ids[8]);
				}
				else
				{
					_ids[8]->setText(result);
				}
				break;
			}
			case 3:
			{

				convert << state.second.x;
				result = convert.str();
				if (!_ids[9])
				{
					_ids[9] = writeTextAt(result, CEGUI::UVector2(CEGUI::UDim(0.9000f, 0), CEGUI::UDim(0.9200f, 0)));
					_screen["Players"]->addChild(_ids[9]);
				}
				else
				{
					_ids[9]->setText(result);
				}
				result.clear();
				convert.str("");
				convert << state.second.y;
				result = convert.str();
				if (!_ids[10])
				{
					_ids[10] = writeTextAt(result, CEGUI::UVector2(CEGUI::UDim(0.7800f, 0), CEGUI::UDim(0.9200f, 0)));
					_screen["Players"]->addChild(_ids[10]);
				}
				else
				{
					_ids[10]->setText(result);
				}
				result.clear();
				convert.str("");
				convert << state.second.z;
				result = convert.str();
				if (!_ids[11])
				{
					_ids[11] = writeTextAt(result, CEGUI::UVector2(CEGUI::UDim(0.8800f, 0), CEGUI::UDim(0.7680f, 0)));
					_screen["Players"]->addChild(_ids[11]);
				}
				else
				{
					_ids[11]->setText(result);
				}
				break;
			}
		}
	}
}

/* Axes X [Power], Axes Y [Bomb], Axes Z [Life]*/

void Gui::GuiMenu::setPowerUp(int power, int index)
{
	//std::cout << "INDEX GET [" << index << "]"  << "POWER GET IS : [" << power << "] E N D " << std::endl;
	if (_statePlayers[index].x != power)
	{
		_statePlayers[index] = Ogre::Vector3(Ogre::Real(power), Ogre::Real(_statePlayers[index].y), Ogre::Real(_statePlayers[index].z));
		_instance->writePlayersState();
	}
}

void Gui::GuiMenu::setBomb(int setBomb, int index)
{
	//std::cout << "INDEX GET [" << index << "]"  << "BOMB GET IS : [" << setBomb << "] E N D " << std::endl;
	if (_statePlayers[index].y != setBomb)
	{
		_statePlayers[index] = Ogre::Vector3(Ogre::Real(_statePlayers[index].x), Ogre::Real(setBomb), Ogre::Real(_statePlayers[index].z));
		_instance->writePlayersState();
	}
}

void Gui::GuiMenu::setLife(int life, int index)
{
	//std::cout << "INDEX GET [" << index << "]" << "Life GET IS : [" << life << "] E N D " << std::endl;
	if (_statePlayers[index].z != life)
	{
		_statePlayers[index] = Ogre::Vector3(Ogre::Real(_statePlayers[index].x), Ogre::Real(_statePlayers[index].y), Ogre::Real(life));
		_instance->writePlayersState();
	}
}

void Gui::GuiMenu::updatePlayers(int nb, int players)
{

}

void Gui::GuiMenu::optionPause(const CEGUI::EventArgs &e)
{
	chargeScreen("Options");
}

void Gui::GuiMenu::resumePause(const CEGUI::EventArgs &e)
{
	_scene->getCamera()->getCamera()->setPosition(Ogre::Vector3(66.7711, 699.157, 2.99712));
	_scene->getCamera()->getCamera()->setDirection(Ogre::Vector3(0, -0.99, 0.06));
	_scene->getCamera()->getCamera()->setOrientation(Ogre::Quaternion(-0.003, 0.003, 0.72, 0.68));
	IS::Utils::Time::getInstance()->setRatio(1.0f);
	chargeScreen("Play");
	chargeScreen("Players");
}

void Gui::GuiMenu::setFileScheme(const std::string &file)
{
	CEGUI::SchemeManager::getSingleton().createFromFile(file);
}

void Gui::GuiMenu::setImageToMouse(const std::string &skin)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage(skin);
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setImage(
		CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getDefaultImage());
}

void Gui::GuiMenu::chargeScreen(const std::string &name)
{
	CEGUI::Window *screen;

	if (_screen.find(name) == _screen.end() && _constructors.find(name) != _constructors.end())
		(this->*_constructors[name])();
	if (_screen.find(name) == _screen.end())
		return;
	screen = _screen[name];
	_currentScreen = name;
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(screen);
}

void Gui::GuiMenu::clearScreen(std::string const &name)
{
	if (_screen.find(name) == _screen.end())
		return;
	CEGUI::Window *screen = _screen[name];
	//for (size_t i = 0; i < screen->getChildCount(); i++)
	//	delete (screen->getChildAtIdx(i));
	delete (screen);
	_screen.erase(name);
}

bool	Gui::GuiMenu::onSliderValueChanged(const CEGUI::EventArgs &e)
{
	CEGUI::WindowManager &winMgr = CEGUI::WindowManager::getSingleton();
	
	return true;
}

CEGUI::Slider *Gui::GuiMenu::createSlider(const CEGUI::USize &size, const CEGUI::UVector2 &pos, const eventList &events)
{
	CEGUI::Slider *slider = static_cast<CEGUI::Slider *>(_wmgr->createWindow("TaharezLook/Slider", ""));

	slider->setPosition(pos);
	slider->setSize(size);
	slider->setMaxValue(1.0f);
	slider->setClickStep(0.1f);
	slider->setCurrentValue(0.5f);

	for (auto event : events)
		slider->subscribeEvent(event.first, CEGUI::Event::Subscriber(event.second, this));
	return (slider);
}

CEGUI::ToggleButton *Gui::GuiMenu::createCheckbox(const CEGUI::USize & size, const CEGUI::UVector2 & pos,
	const CEGUI::Quaternion &rotation,
	const propertyList &properties, const eventList &events)
{
	CEGUI::ToggleButton *checkbox =
		static_cast<CEGUI::ToggleButton*>
		(_wmgr->createWindow("TaharezLook/Checkbox", ""));

	checkbox->setSize(size);
	checkbox->setPosition(pos);
	checkbox->setRotation(rotation);

	for (auto propety : properties)
		checkbox->setProperty(propety.first, propety.second);
	for (auto event : events)
		checkbox->subscribeEvent(event.first, CEGUI::Event::Subscriber(event.second, this));
	return (checkbox);
}

CEGUI::ToggleButton *Gui::GuiMenu::createCheckbox(const CEGUI::USize & size, const CEGUI::UVector2 & pos,
	const CEGUI::Quaternion &rotation,
	const std::string &text,
	const propertyList &properties, const eventList &events)
{
	CEGUI::ToggleButton *checkbox =
		static_cast<CEGUI::ToggleButton*>
		(_wmgr->createWindow("TaharezLook/Checkbox", ""));

	checkbox->setSize(size);
	checkbox->setPosition(pos);
	checkbox->setRotation(rotation);
	checkbox->setText(text);

	for (auto propety : properties)
		checkbox->setProperty(propety.first, propety.second);
	for (auto event : events)
		checkbox->subscribeEvent(event.first, CEGUI::Event::Subscriber(event.second, this));
	return (checkbox);
}


CEGUI::PushButton *Gui::GuiMenu::createButton(const CEGUI::USize &size, const CEGUI::UVector2 &pos,
	const CEGUI::Quaternion &rotation,
	const std::string &text,
	const propertyList &properties, const eventList &events,
	const std::string &fontname)
{
	CEGUI::FontManager::getSingleton().createFromFile(fontname + ".font");
	CEGUI::PushButton *button =
		static_cast<CEGUI::PushButton*>
		(_wmgr->createWindow("TaharezLook/Button", ""));

	if (!text.empty())
	{
		button->setFont(fontname);
		button->setText(text);
	}
	button->setAlpha(5.0f);
	button->setSize(size);
	button->setPosition(pos);
	button->setRotation(rotation);

	for (auto propety : properties)
		button->setProperty(propety.first, propety.second);
	for (auto event : events)
		button->subscribeEvent(event.first, CEGUI::Event::Subscriber(event.second, this));
	return (button);
}

CEGUI::PushButton *Gui::GuiMenu::createButton(const CEGUI::USize &size, const CEGUI::UVector2 &pos,
	const std::string &text,
	const propertyList &properties, const eventList &events,
	const std::string &fontname)
{
	CEGUI::FontManager::getSingleton().createFromFile(fontname + ".font");
	CEGUI::PushButton *button =
		static_cast<CEGUI::PushButton*>
		(_wmgr->createWindow("TaharezLook/Button", ""));

	if (!text.empty())
	{
		button->setFont(fontname);
		button->setText(text);
	}
	button->setAlpha(5.0f);
	button->setSize(size);
	button->setPosition(pos);

	for (auto propety : properties)
		button->setProperty(propety.first, propety.second);
	for (auto event : events)
		button->subscribeEvent(event.first, CEGUI::Event::Subscriber(event.second, this));
	return (button);
}

CEGUI::Window	*Gui::GuiMenu::writeSo(std::string const &text, CEGUI::UVector2 const &pos)
{
	background->setText(text);
	background->setPosition(pos);
	return background;
}

CEGUI::Window	*Gui::GuiMenu::writeTextAt(std::string const &text, CEGUI::UVector2 const &pos)
{
	CEGUI::Window *background = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "");
	
	background->setPosition(pos);
	//background->setSize(CEGUI::USize(CEGUI::UDim(2.f, 0), CEGUI::UDim(2.f, 0)));
	//background->setMinSize(CEGUI::USize(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.3f, 0)));
	//background->setHeight(CEGUI::UDim(0.4f, 0));
	//background->setWidth(CEGUI::UDim(0.4f, 0));
	background->setText(text);
	return background;
}

CEGUI::Window	*Gui::GuiMenu::createBackground(const CEGUI::USize &size, const CEGUI::UVector2 &pos,
	const std::string &image, const std::string &text)
{
	CEGUI::Window *background = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "");
	std::string result; std::ostringstream convert;

	background->setSize(size);
	background->setPosition(pos);
	background->setProperty("DisabledImage", image);
	for (auto state : _statePlayers)
	{
		//std::cout << "id : " << _id << "first : " << state.first << std::endl;
		if (state.first == _id)
		{
			convert << state.second;
			result = convert.str();
			background->setText(result);
			result.clear();
		}
	}
	background->setEnabled(false);
	return (background);
}

CEGUI::Window	*Gui::GuiMenu::createBackground(const CEGUI::USize &size, const CEGUI::UVector2 &pos,
	const std::string &image)
{
	CEGUI::Window *background = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "");
	background->setSize(size);
	background->setPosition(pos);
	background->setProperty("DisabledImage", image);
	background->setEnabled(false);
	return (background);
}

CEGUI::Window	*Gui::GuiMenu::createBackground(const CEGUI::USize &size, const CEGUI::UVector2 &pos,
	const CEGUI::Quaternion &rotation,
	const std::string &image)
{
	CEGUI::Window *background = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "");

	background->setSize(size);
	background->setPosition(pos);
	background->setProperty("DisabledImage", image);
	background->setEnabled(false);
	background->setRotation(rotation);

	return (background);
}

#pragma endregion

#pragma region GUI Useless
void	Gui::GuiMenu::animation()
{
	CEGUI::Animation* anim = CEGUI::AnimationManager::getSingleton().createAnimation("Testing");
	anim->setDuration(0.3f);
	anim->setReplayMode(CEGUI::Animation::RM_Once);
	{
		CEGUI::Affector* affector = anim->createAffector("YRotation", "float");
		affector->createKeyFrame(0.0f, "0.0");
		affector->createKeyFrame(0.3f, "10.0", CEGUI::KeyFrame::P_QuadraticAccelerating);
	}
	{
		CEGUI::Affector* affector = anim->createAffector("Alpha", "float");
		affector->createKeyFrame(0.0f, "1.0");
		affector->createKeyFrame(0.3f, "0.5", CEGUI::KeyFrame::P_QuadraticDecelerating);
	}
	CEGUI::AnimationInstance* instance = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim);
	instance->setTargetWindow(nullptr);
	instance->start();
}
#pragma endregion

#pragma region GUI Menu Actions

void	Gui::GuiMenu::play(const CEGUI::EventArgs &e)
{
	chargeScreen("Play");
}

void	Gui::GuiMenu::launch(const CEGUI::EventArgs &e)
{
	_rule->setLife(_nbLife);
	_rule->setPlayer(_currentNbPlayer);
	_rule->initChara();
	std::cout << "Launch game with "
		<< (int)_currentNbPlayer << " player(s) with "
		<< (int)_nbLife << " life." << std::endl;
	GuiMenu::cleanScreen();
	displayPlayers();
	//change camera
	//CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
	_scene->getCamera()->getCamera()->setPosition(Ogre::Vector3(66.7711, 699.157, 2.99712));
	_scene->getCamera()->getCamera()->setDirection(Ogre::Vector3(0, -0.99, 0.06));
	_scene->getCamera()->getCamera()->setOrientation(Ogre::Quaternion(-0.003, 0.003, 0.72, 0.68));
//	_scene->getCamera()->getCamera()->lookAt(0, 0, 0);
}

void	Gui::GuiMenu::multiplay(const CEGUI::EventArgs &e)
{
	chargeScreen("MultiPlay");
}

void	Gui::GuiMenu::deleteGame(const CEGUI::EventArgs &e)
{
	//std::cout << "DELETE G A M E" << std::endl;
	//_rule->getMap();
	//_rule->deleteChara();
	//	_scene->ResetAll();
	//_screen.clear();
	//chargeScreen("Main");
	#ifdef _WIN32
	_statePlayers.clear();
	IS::Application::replay();
	main(e);
	chargeScreen("Main");
	#else
	std::cerr << "LOOOLLOLL" << std::endl;
	//S::Audio::PlayList::getInstance()->Stop();
	::exit(0);
	#endif
}

void	Gui::GuiMenu::resume(const CEGUI::EventArgs &e)
{
	//IS::Utils::Time::getInstance()->setRatio(1.0f);
		chargeScreen("Main");
}

void	Gui::GuiMenu::replay(const CEGUI::EventArgs &e)
{
}

void	Gui::GuiMenu::main(const CEGUI::EventArgs &e)
{
	_screen.clear();
	std::cout << "C L E A R M A P " << std::endl;

	_currentNbPlayer = 1;
	chargeScreen("Main");
}

void	Gui::GuiMenu::exit(const CEGUI::EventArgs &e)
{
	_statePlayers.clear();
	IS::Application::quit();
}

void	Gui::GuiMenu::options(const CEGUI::EventArgs &e)
{
	chargeScreen("Option");
}

void	Gui::GuiMenu::keybind(const CEGUI::EventArgs &e)
{
	chargeScreen("Players");
}

void	Gui::GuiMenu::changeCam(const CEGUI::EventArgs &e)
{
	const CEGUI::WindowEventArgs *args = static_cast<const CEGUI::WindowEventArgs*>(&e);
	CEGUI::ToggleButton *target = static_cast<CEGUI::ToggleButton *>(args->window);

	_scene->getCamera()->setCam(!target->isSelected());
}

void	Gui::GuiMenu::changeScreen(const CEGUI::EventArgs &e)
{
	const CEGUI::WindowEventArgs *args = static_cast<const CEGUI::WindowEventArgs*>(&e);
	CEGUI::ToggleButton *target = static_cast<CEGUI::ToggleButton *>(args->window);

	bool fullScreen = target->isSelected();
	CEGUI::Sizef size(fullScreen ? 1920 : 1280, fullScreen ? 1080 : 720);

	_rd->setFullscreen(fullScreen, size.d_width, size.d_height);
	_renderCegui->setDisplaySize(size);
	chargeScreen(_currentScreen);
}

void	Gui::GuiMenu::changeNbPlayer(const CEGUI::EventArgs &e)
{
	const CEGUI::WindowEventArgs *args = static_cast<const CEGUI::WindowEventArgs*>(&e);
	CEGUI::ToggleButton *target = static_cast<CEGUI::ToggleButton *>(args->window);

	if (target->isSelected())
		_multiNbPlayer++;
	else
		_multiNbPlayer--;
	_currentNbPlayer = _multiNbPlayer;
}

void	Gui::GuiMenu::changeNbLife(const CEGUI::EventArgs &e)
{
	const CEGUI::MouseEventArgs *args = static_cast<const CEGUI::MouseEventArgs*>(&e);

	if (args->button == CEGUI::MouseButton::LeftButton && _nbLife < 14)
	{
		_nbLife++;
		_screen["Play"]->addChild(createBackground(CEGUI::USize(CEGUI::UDim(0, 88), CEGUI::UDim(0, 73)),
			CEGUI::UVector2(CEGUI::UDim(0.25f + 0.08333 * static_cast<float>((_nbLife - 1) % 7), -44),
				CEGUI::UDim(0.35f + 0.15 * static_cast<float>((_nbLife - 1) / 7), -36.5f)), "TaharezLook/HeartPushed"));
	}
	else if (args->button == CEGUI::MouseButton::RightButton && _nbLife > 1)
	{
		_screen["Play"]->removeChild(_screen["Play"]->getChildAtIdx(_screen["Play"]->getChildCount() - 1));
		_nbLife--;
	}
}

void	Gui::GuiMenu::mediaNext(const CEGUI::EventArgs &e)
{
	IS::Audio::PlayList::getInstance()->Next(true);
	std::cout << "next media " << std::endl;
}

void	Gui::GuiMenu::mediaPrev(const CEGUI::EventArgs &e)
{
	IS::Audio::PlayList::getInstance()->Prev(true);
	std::cout << "prev media " << std::endl;
}

void	Gui::GuiMenu::changeMusic(const CEGUI::EventArgs &e)
{
	const CEGUI::WindowEventArgs *args = static_cast<const CEGUI::WindowEventArgs*>(&e);
	CEGUI::Slider *target = static_cast<CEGUI::Slider *>(args->window);

	_currentMusic = target->getCurrentValue();
}

void	Gui::GuiMenu::changeSound(const CEGUI::EventArgs &e)
{
	const CEGUI::WindowEventArgs *args = static_cast<const CEGUI::WindowEventArgs*>(&e);
	CEGUI::Slider *target = static_cast<CEGUI::Slider *>(args->window);

	IS::Audio::PlayList::getInstance()->setVolume(target->getCurrentValue());
	std::cout << "Sound value: " << target->getCurrentValue() << std::endl;
}
#pragma endregion