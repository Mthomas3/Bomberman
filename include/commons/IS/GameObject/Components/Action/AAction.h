#pragma once

#include <IS/GameObject/Components/AComponent.h>
#include <IS/Physics/Core/PhysicObject.h>
#include <IS/input/Input.hh>
#include <OGRE/Ogre.h>


namespace IS
{
	namespace GameObject
	{
		namespace Component
		{
			class AAction : public AComponent
			{
			public:
				enum Dir
				{
					UP,
					DOWN,
					LEFT,
					RIGHT
				};

				AAction(Ogre::SceneNode * sceneNode, Scene::Scene *, Dir, Ogre::Entity *entity);
				AAction(Ogre::SceneNode * sceneNode, Scene::Scene *, Dir, float speed, Ogre::Entity *entity);
				virtual ~AAction();

				virtual void update() = 0;

				void setSpeed(float speed);
				float getSpeed() const;

			public:
				void configureKey(int id);
				void basicMove(int id);
				void moveKey();
				void moveJoy(int id);
				bool pause();
				bool dropBomb();
				bool kickBomb();
				bool pause(int id);
				bool dropBomb(int id);
				bool kickBomb(int id);
				void aIMoveForward();
				void aIMoveDown();
				void aIMoveRight();
				void aIMoveLeft();

			protected:
				Ogre::AnimationState	*_animationState;
				Ogre::Entity			*_entity;
				struct MyKeys
				{
					InputManager::InputListener::Key up = InputManager::InputListener::Key::KEY_Z;
					InputManager::InputListener::Key down = InputManager::InputListener::Key::KEY_S;
					InputManager::InputListener::Key right = InputManager::InputListener::Key::KEY_D;
					InputManager::InputListener::Key left = InputManager::InputListener::Key::KEY_Q;
					InputManager::InputListener::Key bomb = InputManager::InputListener::Key::KEY_E;
					InputManager::InputListener::Key kick = InputManager::InputListener::Key::KEY_A;
					InputManager::InputListener::Key pauses = InputManager::InputListener::Key::KEY_ESCAPE;
				};
				struct MyKeyJoy
				{
					InputManager::InputListener::buttonJoystick bomb = InputManager::InputListener::buttonJoystick::JOYSTICK_B;
					InputManager::InputListener::buttonJoystick pauses = InputManager::InputListener::buttonJoystick::JOYSTICK_START;
				};
				float					_speed;
				MyKeys					_myKeys;
				MyKeyJoy				_myKeyJoy;
				Ogre::SceneNode			*_sceneNode;
				Physics::DynamicObject	*_physics;
				Dir						_dir;
				InputManager::InputListener *_input;
			};
		}
	}
}

