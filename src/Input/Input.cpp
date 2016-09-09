#include <IS/input/Input.hh>

#include <iomanip>
#include <algorithm>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <thread>
#include <chrono>
#include <array>

bool	InputManager::InputListener::_isInit = false;

InputManager::InputListener::InputListener(void)
{	
	_mouse = nullptr;
	_isMouveMove = false;
	_keyboard = nullptr;
	_inputManager = nullptr;
	_bools = true;

	std::array<InputManager::InputListener::stateKey,255> tmp;// = new InputManager::InputListener::stateKey[255];

	for (int i = 0; i < InputManager::InputListener::Key::MAX; i++)
		_key[i] = InputManager::InputListener::stateKey::NONE;
	for (int i = 0; i < InputManager::InputListener::Mouse::MOUSE_MAX; ++i)
		_mouseKey[i] = InputManager::InputListener::stateKey::NONE;
	for (int i = 0; i < InputManager::InputListener::Joystick::JOYSTICK_MAX; ++i)
		tmp[i] = InputManager::InputListener::stateKey::NONE;
	for (int j = 0; j < 4; j++)
	{
		_joystickKey.push_back(tmp);
	}
	for (int j = 0; j < 4; j++)
	{
		_joystickArrow[j] == InputManager::InputListener::Joystick::JOYSTICK_CENTERED;
	}
	_isCenter = true;
	for (int i = 0; i < InputManager::InputListener::buttonJoystick::BUTTON_MAX; ++i)
	{
		_joystickButton[i] = InputManager::InputListener::stateKey::NONE;
	}
}

InputManager::InputListener::~InputListener(void)
{
	if (_inputManager != nullptr)
	{
		if (_keyboard != nullptr)
		{
			try { _inputManager->destroyInputObject(_keyboard); } catch (...) { }
		}
		if (_mouse != nullptr)
		{

			try { _inputManager->destroyInputObject(_mouse); }
			catch (...) {}
		}
			if (_joystick.size() > 0)
		{
			_itJoystickStart = _joystick.begin();
			_itJoystickEnd = _joystick.end();
			for (; _itJoystickStart != _itJoystickEnd; ++_itJoystickStart)
			{
				try {_inputManager->destroyInputObject((*_itJoystickStart));} catch (...) {}
			}
			_joystick.clear();
		}
			try { OIS::InputManager::destroyInputSystem(_inputManager); } catch (...){}
		_inputManager = nullptr;
		_mouse = nullptr;
		_keyboard = nullptr;
		_joystick.clear();
		CEGUI::System::getSingleton().getDefaultGUIContext().removeAllEvents();
	}
}

size_t		InputManager::InputListener::getNumberJoystick(void) const
{
	return (_inputManager->getNumberOfDevices(OIS::OISJoyStick));
}

void		InputManager::InputListener::initOIS(Ogre::RenderWindow *o, Ogre::Root *root)
{
	OIS::ParamList		list;
	std::ostringstream 	windowHdnStr;
	size_t				windowHnd = 0;
	
	o->getCustomAttribute("WINDOW", &windowHnd);
	windowHdnStr << windowHnd;
	list.insert(std::make_pair(std::string("WINDOW"), windowHdnStr.str()));
	
	try {_inputManager = OIS::InputManager::createInputSystem(list);}
	catch (...) { /*failure init*/ }
	try { _keyboard = static_cast<OIS::Keyboard *>(_inputManager->createInputObject(OIS::OISKeyboard, true)); }
	catch (...) { /*failured _keyboard*/ }
	try { _mouse = static_cast<OIS::Mouse *>(_inputManager->createInputObject(OIS::OISMouse, true)); }
	catch (...) { /*failure _mouse*/ }
	if (_inputManager->getNumberOfDevices(OIS::OISJoyStick) > 0)
	{
		_joystick.resize(_inputManager->getNumberOfDevices(OIS::OISJoyStick));		
		_itJoystickStart = _joystick.begin();
		_itJoystickEnd = _joystick.end();
		for (; _itJoystickStart != _itJoystickEnd; ++_itJoystickStart)
		{
			(*_itJoystickStart) = static_cast<OIS::JoyStick*>(_inputManager->createInputObject(OIS::OISJoyStick, true));
			(*_itJoystickStart)->setEventCallback(this);
		}
		for (_itJoystickStart = _joystick.begin(), _itJoystickEnd = _joystick.end();
			 _itJoystickStart != _itJoystickEnd; ++_itJoystickStart)
		{
			_joys[(*_itJoystickStart)->getID()] = (*_itJoystickStart);
		}
	}
	try
	{
		o->getMetrics(_pos.width, _pos.height, _pos.depth, _pos.left, _pos.top);
	const OIS::MouseState &ms = _mouse->getMouseState();
		ms.width = _pos.width;
		ms.height = _pos.height;
	}
	catch (...) { }
	try { root->addFrameListener(this); } catch (...) {}
	try {_mouse->setEventCallback(this);}
	catch (...){ }
	try {_keyboard->setEventCallback(this);} catch (...) {}
}

bool	InputManager::InputListener::frameStarted(const Ogre::FrameEvent &)
{
	_isMouveMove = false;
	if (_keyboard != nullptr)
		_keyboard->capture();
	if (_mouse != nullptr)
	_mouse->capture();
	if (_joystick.size() > 0)
	{
		_itJoystickStart = _joystick.begin();
		_itJoystickEnd = _joystick.end();
		for (; _itJoystickStart != _itJoystickEnd; ++_itJoystickStart)
		{
			(*_itJoystickStart)->capture();
		}
	}
	return true;
}

bool	InputManager::InputListener::frameEnded(const Ogre::FrameEvent &)
{
	for (int i = 0; i < InputManager::InputListener::Key::MAX; i++)
		if (_key[i] == InputManager::InputListener::stateKey::KEYDOWN)
			_key[i] = InputManager::InputListener::stateKey::KEY;
		else if (_key[i] == InputManager::InputListener::stateKey::KEYUP)
			_key[i] = InputManager::InputListener::stateKey::NONE;

	for (int i = 0; i < InputManager::InputListener::Mouse::MOUSE_MAX; ++i)
		if (_mouseKey[i] == InputManager::InputListener::stateKey::KEYDOWN)
			_mouseKey[i] = InputManager::InputListener::stateKey::KEY;
		else if (_mouseKey[i] == InputManager::InputListener::stateKey::KEYUP)
			_mouseKey[i] = InputManager::InputListener::stateKey::NONE;
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < InputManager::InputListener::Joystick::JOYSTICK_MAX; ++i)
			if (_joystickKey[j][i] == InputManager::InputListener::stateKey::KEYDOWN)
				_joystickKey[j][i] = InputManager::InputListener::InputListener::stateKey::KEY;
			else if (_joystickKey[j][i] == InputManager::InputListener::stateKey::KEYUP)
				_joystickKey[j][i] = InputManager::InputListener::stateKey::NONE;
	}

	for (int i = 0; i < InputManager::InputListener::buttonJoystick::BUTTON_MAX; ++i)
	{
		if (_joystickButton[i] == InputManager::InputListener::stateKey::KEYDOWN)
			_joystickButton[i] = InputManager::InputListener::stateKey::KEY;
		else if (_joystickButton[i] == InputManager::InputListener::stateKey::KEYUP)
			_joystickButton[i] = InputManager::InputListener::stateKey::NONE;
	}
		return true;
}

Ogre::Vector2		InputManager::InputListener::getPosMouse(void)
{
	return (Ogre::Vector2(_mousePos.x, _mousePos.y));
}

bool	InputManager::InputListener::frameRenderingQueued(const Ogre::FrameEvent &)
{
	return true;
}

bool InputManager::InputListener::mouseMoved(const OIS::MouseEvent &o)
{
	_isMouveMove = true;
	_mousePos.x = o.state.X.rel;
	_mousePos.y = o.state.Y.rel;

try { CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(o.state.X.rel, o.state.Y.rel); }
	catch (...){}
	return true;
}

bool InputManager::InputListener::mousePressed(const OIS::MouseEvent &o, OIS::MouseButtonID id)
{	
	_mouseKey[id] = InputManager::InputListener::stateKey::KEYDOWN;
	CEGUI::GUIContext&context = CEGUI::System::getSingleton().getDefaultGUIContext();
	context.injectMouseButtonDown(convertButton(id));
	context.injectChar(convertButton(id));
	return true;
}

bool InputManager::InputListener::mouseReleased(const OIS::MouseEvent &o, OIS::MouseButtonID id)
{
	_mouseKey[id] = InputManager::InputListener::stateKey::KEYUP;
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id));
	return true;
}

bool InputManager::InputListener::keyPressed(const OIS::KeyEvent &o)
{
	_key[o.key] = InputManager::InputListener::stateKey::KEYDOWN;

	CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	context.injectKeyDown((CEGUI::Key::Scan)o.key);
	context.injectChar((CEGUI::Key::Scan)o.text);
		
	return true;
}

bool InputManager::InputListener::keyReleased(const OIS::KeyEvent &o)
{
	_key[o.key] = InputManager::InputListener::stateKey::KEYUP;
	CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)o.key);
	return true;
}

bool InputManager::InputListener::isMouseDown(InputManager::InputListener::Mouse o)
{
	if (_mouseKey[o] == InputManager::InputListener::stateKey::KEYDOWN)
		return true;
	return false;
}

bool InputManager::InputListener::isKeyJoystickId(InputManager::InputListener::Joystick o, int id)
{
	for (int i = 0; i < 4; i++)
	{
		if (i == id)
		{
			if (_joystickArrow[i] == o)
				return true;
		}
	}
	return false;
}

bool InputManager::InputListener::isKeyJoystick(InputManager::InputListener::Joystick o)
{
	if (_joystickKey[0][o] == InputManager::InputListener::stateKey::KEY ||
		_joystickKey[0][o] == InputManager::InputListener::stateKey::KEYDOWN)
		return true;
	return false;
}

bool InputManager::InputListener::isKeyJoystickDown(InputManager::InputListener::Joystick o)
{
	if (_joystickKey[0][o] == InputManager::InputListener::KEYDOWN)
		return true;
	return false;
}

bool InputManager::InputListener::isKeyDown(InputManager::InputListener::Key o)
{
	if (_key[o] == InputManager::InputListener::stateKey::KEYDOWN)
		return true;
	return false;
}

bool	InputManager::InputListener::isKeyMouse(InputManager::InputListener::Mouse o)
{
	if (_mouseKey[o] == InputManager::InputListener::stateKey::KEY ||
		_mouseKey[o] == InputManager::InputListener::stateKey::KEYDOWN)
		return true;
	return false;
}

bool	InputManager::InputListener::isKey(InputManager::InputListener::Key o)
{
	if (_key[o] == InputManager::InputListener::stateKey::KEY ||
		_key[o] == InputManager::InputListener::stateKey::KEYDOWN)
		return true;
	return false;
}

bool	InputManager::InputListener::isKey(InputManager::InputListener::Key o, int id)
{
	if (id == _joyPos.id)
	{
		if (_key[o] == InputManager::InputListener::stateKey::KEY ||
			_key[o] == InputManager::InputListener::stateKey::KEYDOWN)
			return true;
	}
	return false;
}

bool	InputManager::InputListener::isbuttonKey(InputManager::InputListener::buttonJoystick id, int index)
{
	std::map<int, OIS::JoyStick*>::iterator p;

	for (p = _joys.begin(); p != _joys.end(); ++p)
	{
		if (p->first == index)
		{
			if (_joystickButton[id] == InputManager::InputListener::stateKey::KEYDOWN ||
				_joystickButton[id] == InputManager::InputListener::stateKey::KEY)
			{
				return true;
			}
			break;
		}
	}
	return false;
}

CEGUI::MouseButton InputManager::InputListener::convertButton(OIS::MouseButtonID buttonID)
{
	switch (buttonID)
	{
	case OIS::MB_Left:
		return CEGUI::LeftButton;

	case OIS::MB_Right:
		return CEGUI::RightButton;

	case OIS::MB_Middle:
		return CEGUI::MiddleButton;

	default:
		return CEGUI::LeftButton;
	}
}

bool	InputManager::InputListener::isButtonKeyDown(InputManager::InputListener::buttonJoystick id, int index)
{
			if (_joystickKey[index][id] == InputManager::InputListener::stateKey::KEYDOWN)
			{
				return true;
			}
		
	return false;
}

bool	InputManager::InputListener::buttonPressed(const OIS::JoyStickEvent &o, int index)
{
	std::map<int, OIS::JoyStick*>::iterator p;
	std::cout << index << std::endl;
	for (p = _joys.begin(); p != _joys.end(); ++p)
	{
		if (p->first == o.device->getID())
		{
		//	std::cout << index << std::endl;
			_joystickButton[index] = InputManager::InputListener::stateKey::KEYDOWN;

			_joystickKey[o.device->getID()][index] = InputManager::InputListener::stateKey::KEYDOWN;
			break;
		}
	}
	return true;
}

bool	InputManager::InputListener::buttonReleased(const OIS::JoyStickEvent &o, int index)
{
	std::map<int, OIS::JoyStick*>::iterator p;

	for (p = _joys.begin(); p != _joys.end(); ++p)
	{
		if (p->first == o.device->getID())
		{
			//_joystickKey[o.device->getID()][index] = InputManager::InputListener::stateKey::KEYUP;
			_joystickButton[index] = InputManager::InputListener::stateKey::KEYUP;
			break;
		}
	}
	return true;
}

bool	InputManager::InputListener::axisMoved(const OIS::JoyStickEvent &o, int index)
{
	std::map<int, OIS::JoyStick*>::iterator p;

	for (p = _joys.begin(); p != _joys.end(); ++p)
	{
		if (p->first == o.device->getID())
		{
			for (int i = 0; i < o.state.mAxes.size(); ++i)
			{
				_joyPos.right_x = o.state.mAxes[0].abs;
				_joyPos.right_y = o.state.mAxes[1].abs;
				_joyPos.left_x = o.state.mAxes[2].abs;
				_joyPos.left_y = o.state.mAxes[3].abs;
				_joyPos.id = o.device->getID();
			}

			break;
		}
	}
	return true;
}

bool	InputManager::InputListener::sliderMoved(const OIS::JoyStickEvent &o, int index)
{
	std::map<int, OIS::JoyStick*>::iterator p;

	for (p = _joys.begin(); p != _joys.end(); ++p)
	{
		if (p->first == o.device->getID())
		{
			break;
		}
	}
	return true;
}

bool	InputManager::InputListener::povMoved(const OIS::JoyStickEvent &o, int index)
{
	switch (o.state.mPOV[index].direction)
	{
	case OIS::Pov::North:
		_joystickArrow[o.device->getID()] = InputManager::InputListener::Joystick::JOYSTICK_NORTH;
		break;
	case OIS::Pov::South:
		_joystickArrow[o.device->getID()] = InputManager::InputListener::Joystick::JOYSTICK_SOUTH;
		break;
	case OIS::Pov::East:
		_joystickArrow[o.device->getID()] = InputManager::InputListener::Joystick::JOYSTICK_EAST;
		break;
	case OIS::Pov::West:
		_joystickArrow[o.device->getID()] = InputManager::InputListener::Joystick::JOYSTICK_WEST;
		break;
	case OIS::Pov::Centered :
		_joystickArrow[o.device->getID()] = InputManager::InputListener::Joystick::JOYSTICK_CENTERED;
		break;
	default:
		break;
	}
	return true;
}

Ogre::Vector4 InputManager::InputListener::normalizeVector(Ogre::Vector4 &v)
{
	float f = 1.0f / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
	return v * f;
}

Ogre::Vector4		InputManager::InputListener::getVectorId(int joystick)
{
	Ogre::Vector4	_tmp(0.0, 0.0, 0.0, 0.0);

	if (_joyPos.id == joystick)
	{
		_tmp[0] = _joyPos.right_x;
		_tmp[1] = _joyPos.right_y;
		_tmp[2] = _joyPos.left_x;
		_tmp[3] = _joyPos.left_y;
		_tmp = normalizeVector(_tmp);
	}
	return _tmp;
}

bool InputManager::InputListener::isMouseMoving(void)
{
	return _isMouveMove;
}

bool	InputManager::InputListener::vector3Moved(const OIS::JoyStickEvent &o, int index)
{
	std::map<int, OIS::JoyStick*>::iterator p;

	for (p = _joys.begin(); p != _joys.end(); ++p)
	{
		if (p->first == o.device->getID())
		{
			break;
		}
	}
	return true;
}