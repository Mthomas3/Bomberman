#pragma once

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>
#include <OGRE/OgreFrameListener.h>
#include <OGRE/OgreVector2.h>
#include <IS/Utils/Singleton.h>
#include <vector>
#include <map>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>


namespace InputManager
{
	class InputListener : public IS::Utils::Singleton<InputListener>, Ogre::FrameListener, OIS::MouseListener, OIS::KeyListener, OIS::JoyStickListener
	{
	public :

		enum buttonJoystick
		{
			JOYSTICK_A = 0,
			JOYSTICK_B = 1,
			JOYSTICK_X = 2,
			JOYSTICK_Y = 3,
			JOYSTICK_LB = 4,
			JOYSTICK_RB = 5,
			JOYSTICK_BACK = 6,
			JOYSTICK_START = 7,
			BUTTON_MAX
		};

		enum Joystick
		{
			JOYSTICK_NORTH = OIS::Pov::North,
			JOYSTICK_SOUTH = OIS::Pov::South,
			JOYSTICK_EAST = OIS::Pov::East,
			JOYSTICK_WEST = OIS::Pov::West,
			JOYSTICK_CENTERED = OIS::Pov::Centered,
			JOYSTICK_MAX = 255
		};

		enum Mouse
		{
			MOUSE_RIGHT = OIS::MB_Right,
			MOUSE_LEFT = OIS::MB_Left,
			MOUSE_B3 = OIS::MB_Button3,
			MOUSE_B4 = OIS::MB_Button4,
			MOUSE_B5 = OIS::MB_Button5,
			MOUSE_B6 = OIS::MB_Button6,
			MOUSE_B7 = OIS::MB_Button7,
			MOUSE_MAX = 255
		};

		enum	Key
		{
			KEY_A = OIS::KC_Q,
			KEY_Z = OIS::KC_W,
			KEY_E = OIS::KC_E,
			KEY_R = OIS::KC_R,
			KEY_T = OIS::KC_T,
			KEY_Y = OIS::KC_Y,
			KEY_U = OIS::KC_U,
			KEY_I = OIS::KC_I,
			KEY_O = OIS::KC_O,
			KEY_P = OIS::KC_P,
			KEY_Q = OIS::KC_A,
			KEY_S = OIS::KC_S,
			KEY_D = OIS::KC_D,
			KEY_F = OIS::KC_F,
			KEY_G = OIS::KC_G,
			KEY_H = OIS::KC_H,
			KEY_J = OIS::KC_J,
			KEY_K = OIS::KC_K,
			KEY_L = OIS::KC_L,
			KEY_M = OIS::KC_M,
			KEY_W = OIS::KC_W,
			KEY_X = OIS::KC_X,
			KEY_C = OIS::KC_C,
			KEY_V = OIS::KC_V,
			KEY_B = OIS::KC_B,
			KEY_N = OIS::KC_N,
			KEY_F1 = OIS::KC_F1,
			KEY_F2 = OIS::KC_F2,
			KEY_F3 = OIS::KC_F3,
			KEY_F4 = OIS::KC_F4,
			KEY_F5 = OIS::KC_F5,
			KEY_F6 = OIS::KC_F6,
			KEY_F7 = OIS::KC_F7,
			KEY_F8 = OIS::KC_F8,
			KEY_F9 = OIS::KC_F9,
			KEY_F10 = OIS::KC_F10,
			KEY_F11 = OIS::KC_F11,
			KEY_F12 = OIS::KC_F12,
			KEY_CAPITAL = OIS::KC_CAPITAL,
			KEY_COMMA = OIS::KC_COMMA,
			KEY_PERIOD = OIS::KC_PERIOD, //point "."
			KEY_LOCK = OIS::KC_NUMLOCK,
			KEY_ESCAPE = OIS::KC_ESCAPE,
			KEY_LEFT = OIS::KC_LEFT,
			KEY_RIGHT = OIS::KC_RIGHT,
			KEY_UP = OIS::KC_UP,
			KEY_DOWN = OIS::KC_DOWN,
			MAX =255
		};
		enum stateKey
		{
			KEY = 0,
			KEYDOWN = 1,
			KEYUP = 2,
			NONE
		};

	public :
		InputListener(void);
		virtual ~InputListener(void);
		void							initOIS(Ogre::RenderWindow *, Ogre::Root *);
		bool							frameRenderingQueued(const Ogre::FrameEvent &);
		inline  OIS::Keyboard			*getKeyboard(void) { return _keyboard; }
		inline  OIS::Mouse				*getMouse(void) { return _mouse; }
	
		Ogre::Vector2					getPosMouse(void);
		virtual bool					mouseMoved(const OIS::MouseEvent &o);
		virtual bool					mousePressed(const OIS::MouseEvent &o, OIS::MouseButtonID id);
		virtual bool					mouseReleased(const OIS::MouseEvent &o, OIS::MouseButtonID id);
		virtual bool					keyPressed(const OIS::KeyEvent &o);
		virtual bool					keyReleased(const OIS::KeyEvent &o);
		bool							isKeyDown(InputManager::InputListener::Key);
		virtual bool					frameStarted(const Ogre::FrameEvent &);
		virtual bool					frameEnded(const Ogre::FrameEvent &);
		bool							isKey(InputManager::InputListener::Key);
		bool							isKey(InputManager::InputListener::Key, int);
		bool							isMouseDown(InputManager::InputListener::Mouse);
		bool							isKeyMouse(InputManager::InputListener::Mouse);
		bool							isKeyJoystickDown(InputManager::InputListener::Joystick);
		bool							isKeyJoystick(InputManager::InputListener::Joystick);

		virtual bool					buttonPressed(const OIS::JoyStickEvent &, int);
		virtual bool					buttonReleased(const OIS::JoyStickEvent &, int);
		virtual bool					axisMoved(const OIS::JoyStickEvent &, int);
		virtual bool					sliderMoved(const OIS::JoyStickEvent &, int);
		virtual bool					povMoved(const OIS::JoyStickEvent &, int);
		Ogre::Vector4					normalizeVector(Ogre::Vector4 & v);
		virtual bool					vector3Moved(const OIS::JoyStickEvent &, int);
		bool							isKeyJoystickId(InputManager::InputListener::Joystick, int);
		size_t							getNumberJoystick(void) const;
		Ogre::Vector4					getVectorId(int joystick);
		bool							isMouseMoving(void);
		bool							isButtonKeyDown(InputManager::InputListener::buttonJoystick id, int index);
		bool							isbuttonKey(InputManager::InputListener::buttonJoystick id, int index);
		CEGUI::MouseButton				convertButton(OIS::MouseButtonID buttonID);
		static void						deleteInput();


	private :
		
		OIS::InputManager				*_inputManager;
		OIS::Mouse						*_mouse;
		OIS::Keyboard					*_keyboard;
		std::vector<OIS::JoyStick*>		_joystick;
	//	OIS::JoyStick*					_t;
		std::vector<OIS::JoyStick*>::iterator _itJoystickStart;
		std::vector<OIS::JoyStick*>::iterator  _itJoystickEnd;
		OIS::JoyStick *	test;
		std::map<int, OIS::JoyStick *>	_joys;
		bool							_isCenter;
		bool							_isMouveMove;
		struct				Pos
		{
			unsigned int	width;
			unsigned int	height;
			unsigned int	depth;
			int				top;
			int				left;
		};
		struct			mousePos
		{
			int			x = 0;
			int			y = 0;
		};
		struct			joyPos
		{
			int			left_x = 0;
			int			left_y = 0;
			int			right_x = 0;
			int			right_y = 0;
			int			id = 0;
		};
	
	private :
		std::vector<std::array<InputManager::InputListener::stateKey, 255>>		_joystickKey;
		InputManager::InputListener::Joystick									_joystickArrow[4];
		InputManager::InputListener::stateKey									_mouseKey[InputManager::InputListener::Mouse::MOUSE_MAX];
		InputManager::InputListener::stateKey									_key[InputManager::InputListener::Key::MAX];
		InputManager::InputListener::stateKey									_joystickButton[InputManager::InputListener::buttonJoystick::BUTTON_MAX];
		Pos					_pos;
		mousePos			_mousePos;
		joyPos				_joyPos;
		Ogre::Vector4		_posJoy;
		bool				_bools;
		static bool			_isInit;
	};
}