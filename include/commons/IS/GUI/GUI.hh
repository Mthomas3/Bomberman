#pragma once

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <IS/Utils/Singleton.h>
#include <iostream>
#include <map>
#include <functional>
#include <IS/GameObject/GameObject.h>
#include <IS/GameObject/Components/Rule/Rule.h>
#include <IS/Scene/Scene.h>

namespace Gui
{
	class GuiMenu : public CEGUI::Event::Connection
	{
		typedef std::map<std::string, std::string> propertyList;
		typedef std::map<const CEGUI::String, void(GuiMenu::*)(const CEGUI::EventArgs&)> eventList;
	public:
		static void createGUI(Ogre::Root *root, IS::GameObject::Component::Rule *rule, IS::Scene::Scene *scene, Ogre::RenderWindow *rd)
		{
			if (_instance != nullptr)
			{
				_scene = scene;
				_rule = rule;
				_rd = rd;
				_instance->run();
				return;
			}
			_instance = new GuiMenu(root);
			_instance->run();
			_rule = rule;
			_scene = scene;
			_rd = rd;
		}
		static void deleteGUI();
		static void pause();
		static void cleanScreen() { Gui::GuiMenu::_instance->_screen[Gui::GuiMenu::_instance->_currentScreen]->setVisible(false); }
		static void displayScreen() { Gui::GuiMenu::_instance->_screen[Gui::GuiMenu::_instance->_currentScreen]->setVisible(true); }
		static void	updatePlayers(int, int);
		static void	setBomb(int, int);
		static void	setPowerUp(int, int);
		static void	setLife(int, int);
		static uint8_t	getMusicValue(void) { return (_currentMusic); };

	private:
		GuiMenu(Ogre::Root *root) : _root(root) { }
		~GuiMenu();
		void	run(void);

	private:
		CEGUI::ToggleButton *createCheckbox(const CEGUI::USize & size, const CEGUI::UVector2 & pos,
			const CEGUI::Quaternion &rotation,
			const propertyList &properties, const eventList &events);
		CEGUI::ToggleButton *createCheckbox(const CEGUI::USize & size, const CEGUI::UVector2 & pos,
			const CEGUI::Quaternion &rotation,
			const std::string &text,
			const propertyList &properties, const eventList &events);
		CEGUI::PushButton *createButton(const CEGUI::USize &size, const CEGUI::UVector2 &pos,
			const CEGUI::Quaternion &rotation,
			const std::string &text,
			const propertyList &properties, const eventList &events,
			const std::string &fontname = "DejaVuSans-10");
		CEGUI::PushButton *createButton(const CEGUI::USize &size, const CEGUI::UVector2 &pos,
			const std::string &text,
			const propertyList &properties, const eventList &events,
			const std::string &fontname = "DejaVuSans-10");
		CEGUI::Window *createBackground(const CEGUI::USize &size, const CEGUI::UVector2 &pos,
			const CEGUI::Quaternion &rotation,
			const std::string &image);
		CEGUI::Window *createBackground(const CEGUI::USize &size, const CEGUI::UVector2 &pos,
			const std::string &image);
		CEGUI::Window *createBackground(const CEGUI::USize &size, const CEGUI::UVector2 &pos,
			const std::string &image, const std::string &text);
		CEGUI::Window	*writeTextAt(std::string const &text, CEGUI::UVector2 const &pos);
		CEGUI::Slider *createSlider(const CEGUI::USize &size, const CEGUI::UVector2 &pos, const eventList &events);
		CEGUI::Window	*writeSo(std::string const &text, CEGUI::UVector2 const &pos);

		bool	onSliderValueChanged(const CEGUI::EventArgs &);

		void	setFileScheme(const std::string &file);
		void	setImageToMouse(const std::string &skin);
		void	chargeScreen(const std::string &name);
		void	clearScreen(std::string const &name);
		void	writePlayersState(void);
		void    writeAxesToPlayer(std::string const &result, const CEGUI::UVector2 &pos);
		void	animation();
		void	play(const CEGUI::EventArgs &ev);
		void	launch(const CEGUI::EventArgs &e);
		void	multiplay(const CEGUI::EventArgs &e);
		void	resume(const CEGUI::EventArgs &e);
		void	replay(const CEGUI::EventArgs &e);
		void	main(const CEGUI::EventArgs &e);
		void	exit(const CEGUI::EventArgs &e);
		void	options(const CEGUI::EventArgs &e);
		void	keybind(const CEGUI::EventArgs &e);
		void	resumePause(const CEGUI::EventArgs &e);
		void	optionPause(const CEGUI::EventArgs &e);
		void	configPause(const CEGUI::EventArgs &e);
		void	deleteGame(const CEGUI::EventArgs &e);
		void	mediaNext(const CEGUI::EventArgs &e);
		void	mediaPrev(const CEGUI::EventArgs &e);

		void	changeMusic(const CEGUI::EventArgs &e);
		void	changeCam(const CEGUI::EventArgs &e);
		void	changeScreen(const CEGUI::EventArgs &e);
		void	changeSound(const CEGUI::EventArgs &e);
		void	changeNbPlayer(const CEGUI::EventArgs &e);
		void	changeNbLife(const CEGUI::EventArgs &e);
		void	displayPlayers();
		void	writeNewValue();
		void	displayEnd();
		void	createMainMenu(void);
		void	createPauseMenu(void);
		void	createPlayMenu(void);
		void	createMultiPlayMenu(void);
		void	createOptionMenu(void);
		void	createOptionMenus(void);
		void	createPlayers(void);
		void	createEndGame(void);

	private:
		static GuiMenu *_instance;
		static IS::GameObject::Component::Rule *_rule;
		static Ogre::RenderWindow *_rd;
		static IS::Scene::Scene *_scene;
		std::map<std::string, void(GuiMenu::*)()> _constructors;
		std::map<std::string, CEGUI::Window *> _screen;
		CEGUI::WindowManager *_wmgr;
		CEGUI::OgreRenderer *_renderCegui;
		Ogre::Root *_root;
		std::string _currentScreen;

		uint8_t	_multiNbPlayer;
		uint8_t	_currentNbPlayer;

		static uint8_t _currentPowerUp;
		static uint8_t _currentBomb;
		static uint8_t	_currentLife;
		static std::map<int, Ogre::Vector3> _statePlayers;
		static uint8_t _nbLife;
		static uint8_t _id;
		static uint8_t	_nbBomb;
		static CEGUI::Window *background;
		static CEGUI::Window **_ids;
		static uint8_t _currentMusic;
		static CEGUI::Window *_players;

	};
}
