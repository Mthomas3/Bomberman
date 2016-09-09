#include <IS/Scene/Camera.h>

IS::Scene::Camera::Camera(Ogre::SceneManager * sceneManager, Ogre::RenderWindow *renderWindow, const std::string &name) : _sceneManager(sceneManager), _renderWindow(renderWindow)
{
	_camera = sceneManager->createCamera(name);
	_camera->setPosition(Ogre::Vector3(700.0, 175.0, -475.0));
	_camera->setDirection(Ogre::Vector3(-0.540779, -0.841215, 3.80278e-05));
	_camera->setOrientation(Ogre::Quaternion(0.414677, 0.00458568, 0.909931, -0.00988362));
	//_camera->lookAt(Ogre::Vector3(0, 0, 0));
	_camera->setNearClipDistance(5);
	_viewport = _renderWindow->addViewport(_camera);
	_viewport->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
	_camera->setAspectRatio(Ogre::Real(_viewport->getActualWidth()) / Ogre::Real(_viewport->getActualHeight()));
	_input = InputManager::InputListener::getInstance();
	_stopCam = true;
}

IS::Scene::Camera::~Camera()
{
	_renderWindow->removeAllViewports();
}

void IS::Scene::Camera::update()
{
/*		std::cout << "pos = " << _camera->getPosition() << std::endl;
		std::cout << "fir = " << _camera->getDirection() << std::endl;
		std::cout << "orientation = " << _camera->getOrientation() << std::endl;
	*/cameraMovement();
}

void IS::Scene::Camera::cameraMovement()
{
	if (!_stopCam)
	{
		Ogre::Real speed = 10;
		if (_input->isMouseMoving())
			_MousePos = _input->getPosMouse();
		_camera->yaw(Ogre::Degree(-_speedMouse * _MousePos.x));
		_camera->pitch(Ogre::Degree(-_speedMouse * _MousePos.y));
		if (_input->isKey(InputManager::InputListener::Key::KEY_LEFT))
			_camera->moveRelative(Ogre::Vector3(-speed, 0, 0));
		if (_input->isKey(InputManager::InputListener::Key::KEY_RIGHT))
			_camera->moveRelative(Ogre::Vector3(speed, 0, 0));
		if (_input->isKey(InputManager::InputListener::Key::KEY_UP))
			_camera->moveRelative(Ogre::Vector3(0, 0, -speed));
		if (_input->isKey(InputManager::InputListener::Key::KEY_DOWN))
			_camera->moveRelative(Ogre::Vector3(0, 0, speed));
		if (_input->isKey(InputManager::InputListener::Key::KEY_O))
			_camera->moveRelative(Ogre::Vector3(0, -speed, 0));
		if (_input->isKey(InputManager::InputListener::Key::KEY_L))
			_camera->moveRelative(Ogre::Vector3(0, speed, 0));
		_MousePos = Ogre::Vector2::ZERO;
	}
	//std::cout << "[Camera]: " << _camera->getPosition() << "/" << _camera->getRealOrientation() << std::endl;
	//std::cout << "[Camera]: " << _camera->getPosition() << "/" << _camera->l << std::endl;
}

void IS::Scene::Camera::setCam(bool stopCam)
{
	_stopCam = stopCam;
}
