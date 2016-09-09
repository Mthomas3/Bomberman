#pragma once

#include <OGRE/Ogre.h>
#include <IS/input/Input.hh>

namespace IS
{
	namespace Scene
	{
		class Camera
		{
		public:
			Camera(Ogre::SceneManager *sceneManager, Ogre::RenderWindow *renderWindow, const std::string &name = "Default Camera");
			virtual ~Camera();

			Ogre::Camera	*getCamera() const { return _camera; }
			Ogre::Viewport	*getViewport() const { return _viewport; }

			void update();
			void cameraMovement();

			void setCam(bool stopCam);
		private:
			Ogre::SceneManager				*_sceneManager;
			Ogre::RenderWindow				*_renderWindow;
			Ogre::Camera					*_camera;
			Ogre::Viewport					*_viewport;
			Ogre::Vector2					_MousePos;
			InputManager::InputListener		*_input;
			Ogre::Real						_speedMouse = 0.5;
			Ogre::Real						_speedKey = 0.01;
			bool							_stopCam;
		};
	}
}