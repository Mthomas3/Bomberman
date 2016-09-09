#include <IS/GameObject/Components/Action/AAction.h>
#include <chrono>
#include <thread>
#include <IS/GameObject/Components/Mesh/MeshComponent.h>
#include <vector>
#include <IS/Utils/Time/Time.h>
IS::GameObject::Component::AAction::AAction(Ogre::SceneNode *sceneNode, IS::Scene::Scene *scene, Dir dir, Ogre::Entity *entity) : IS::GameObject::Component::AComponent(sceneNode, scene)
{
	_speed = 1.0f;
	_dir = dir;
	_sceneNode = sceneNode;
	_entity = entity;
	_physics = IS::Physics::newPhysicObject<IS::Physics::DynamicObject>(_node, scene, Collision::ECollisionType::COLLISION_ACCURATE);
	_input = InputManager::InputListener::getInstance();
	//std::cout << _scene->getGameObjectBySceneNode(_node->getParentSceneNode())->getComponent<IS::GameObject::Component::MeshComponent>()->getEntity()->getName() << std::endl;
	//	_animationState = _scene->getGameObjectBySceneNode(_node->getParentSceneNode())->getComponent<IS::GameObject::Component::MeshComponent>()->getEntity()->getAnimationState("Idle");
	//std::cout << _entity->getSkeleton()->hasBone("Jogging") << std::endl;
	/*
	const Ogre::StringVector &l = _entity->getAnimableValueNames();
	Ogre::StringVector::const_iterator i = l.begin(), e = l.end();
	int j = 1;
	for (; i != e; ++i) {
		Ogre::String name = *i;
		std::cout << "anime name" << name.c_str() << std::endl;
	}*/

//	for (int i = 0; i < animeName.size(); i++)
	//	std::cout << "anime name" << animeName[i] << std::endl;
	/*_animationState = _entity->getAnimationState("mixamorig_HeadTop_End");
	_animationState->setLoop(true);
	_animationState->setEnabled(true);*/
	//_animationState->setTimePosition(1.0f);


}

IS::GameObject::Component::AAction::AAction(Ogre::SceneNode *sceneNode, IS::Scene::Scene *scene, Dir dir, float speed, Ogre::Entity *entity) : IS::GameObject::Component::AComponent(sceneNode, scene)
{
	_speed = speed;
	_dir = dir;
	_animationState = entity->getAnimationState("breathing idle");
	_animationState->setLoop(true);
	_animationState->setEnabled(true);
	_animationState->setTimePosition(10.0f);
	_sceneNode = sceneNode;
	_input = InputManager::InputListener::getInstance();
	_physics = IS::Physics::newPhysicObject<IS::Physics::DynamicObject>(_node, scene, Collision::ECollisionType::COLLISION_ACCURATE);
}

IS::GameObject::Component::AAction::~AAction()
{
}

void IS::GameObject::Component::AAction::configureKey(int id)
{
}

void IS::GameObject::Component::AAction::basicMove(int id)
{
	Ogre::Vector4 vec4(_input->getVectorId(id));
	if (vec4.x > 0)
	{
		_physics->goBackward(vec4.x * _speed / 1);
	}
	else if (vec4.x < 0)
	{
		_physics->goForward((-1 * vec4.x) * _speed / 1);
	}
	if (vec4.y > 0)
	{
		_physics->goLeft(vec4.y * _speed / 1);
	}
	else if (vec4.y < 0)
	{
		_physics->goRight((-1 * vec4.y) * _speed / 1);
	}
}

void IS::GameObject::Component::AAction::moveJoy(int id)
{
	if (_input->isKeyJoystickId(InputManager::InputListener::Joystick::JOYSTICK_NORTH, id))
	{
		switch (_dir)
		{
		case IS::GameObject::Component::AAction::UP:
			_physics->goForward(_speed * IS::Utils::Time::getInstance()->getDeltaTime() * 70);
			break;
		case IS::GameObject::Component::AAction::DOWN:
			_physics->yawLeft(1.0f, 180.0f);
			_dir = UP;
			break;
		case IS::GameObject::Component::AAction::LEFT:
			_physics->yawRight(1.0f, 90.0f);
			_dir = UP;
			break;
		case IS::GameObject::Component::AAction::RIGHT:
			_physics->yawLeft(1.0f, 90.0f);
			_dir = UP;
			break;
		default:
			break;
		}
	}
	if (_input->isKeyJoystickId(InputManager::InputListener::Joystick::JOYSTICK_SOUTH, id))
	{
		switch (_dir)
		{
		case IS::GameObject::Component::AAction::UP:
			_physics->yawLeft(1.0f, 180.0f);
			_dir = DOWN;
			break;
		case IS::GameObject::Component::AAction::DOWN:
			_physics->goForward(_speed * IS::Utils::Time::getInstance()->getDeltaTime() * 70);
			break;
		case IS::GameObject::Component::AAction::LEFT:
			_physics->yawLeft(1.0f, 90.0f);
			_dir = DOWN;
			break;
		case IS::GameObject::Component::AAction::RIGHT:
			_physics->yawRight(1.0f, 90.0f);
			_dir = DOWN;
			break;
		default:
			break;
		}
	}
	if (_input->isKeyJoystickId(InputManager::InputListener::Joystick::JOYSTICK_EAST, id))
	{
		switch (_dir)
		{
		case IS::GameObject::Component::AAction::UP:
			_physics->yawRight(1.0f, 90.0f);
			_dir = RIGHT;
			break;
		case IS::GameObject::Component::AAction::DOWN:
			_physics->yawLeft(1.0f, 90.0f);
			_dir = RIGHT;
			break;
		case IS::GameObject::Component::AAction::LEFT:
			_physics->yawRight(1.0f, 180.0f);
			_dir = RIGHT;
			break;
		case IS::GameObject::Component::AAction::RIGHT:
			_physics->goForward(_speed * IS::Utils::Time::getInstance()->getDeltaTime() * 70);
			break;
		default:
			break;
		}
	}
	if (_input->isKeyJoystickId(InputManager::InputListener::Joystick::JOYSTICK_WEST, id))
	{
		switch (_dir)
		{
		case IS::GameObject::Component::AAction::UP:
			_physics->yawLeft(1.0f, 90.0f);
			_dir = LEFT;
			break;
		case IS::GameObject::Component::AAction::DOWN:
			_physics->yawRight(1.0f, 90.0f);
			_dir = LEFT;
			break;
		case IS::GameObject::Component::AAction::LEFT:
			_physics->goForward(_speed * IS::Utils::Time::getInstance()->getDeltaTime() * 70);
			break;
		case IS::GameObject::Component::AAction::RIGHT:
			_physics->yawRight(1.0f, 180.0f);
			_dir = LEFT;
			break;
		default:
			break;
		}
	}
}

void IS::GameObject::Component::AAction::moveKey()
{
/*	if (_input->isKey(_myKeys.up) || _input->isKey(_myKeys.left) || _input->isKey(_myKeys.right) || _input->isKey(_myKeys.down))
	{
		_animationState = _entity->getAnimationState("jogging");
		_animationState->setLoop(true);
		_animationState->setEnabled(true);

	}
	else
	{
		_animationState = _entity->getAnimationState("breathing idle");
		_animationState->setLoop(true);
		_animationState->setEnabled(true);

	}*/
	if (_input->isKey(_myKeys.up))
	{
		switch (_dir)
		{
		case IS::GameObject::Component::AAction::UP:

	//		_animationState->setLoop(true);
			_physics->goForward(_speed * IS::Utils::Time::getInstance()->getDeltaTime() * 70);
			break;
		case IS::GameObject::Component::AAction::DOWN:
			_physics->yawLeft(1.0f, 180.0f);
			_dir = UP;
			break;
		case IS::GameObject::Component::AAction::LEFT:
			_physics->yawRight(1.0f, 90.0f);
			_dir = UP;
			break;
		case IS::GameObject::Component::AAction::RIGHT:
			_physics->yawLeft(1.0f, 90.0f);
			_dir = UP;
			break;
		default:
			break;
		}
	}
	if (_input->isKey(_myKeys.down))
	{
		switch (_dir)
		{
		case IS::GameObject::Component::AAction::UP:
			_physics->yawLeft(1.0f, 180.0f);
			_dir = DOWN;
			break;
		case IS::GameObject::Component::AAction::DOWN:
			_physics->goForward(_speed * IS::Utils::Time::getInstance()->getDeltaTime() * 70);
			break;
		case IS::GameObject::Component::AAction::LEFT:
			_physics->yawLeft(1.0f, 90.0f);
			_dir = DOWN;
			break;
		case IS::GameObject::Component::AAction::RIGHT:
			_physics->yawRight(1.0f, 90.0f);
			_dir = DOWN;
			break;
		default:
			break;
		}
	}
	if (_input->isKey(_myKeys.right))
	{
		switch (_dir)
		{
		case IS::GameObject::Component::AAction::UP:
			_physics->yawRight(1.0f, 90.0f);
			_dir = RIGHT;
			break;
		case IS::GameObject::Component::AAction::DOWN:
			_physics->yawLeft(1.0f, 90.0f);
			_dir = RIGHT;
			break;
		case IS::GameObject::Component::AAction::LEFT:
			_physics->yawRight(1.0f, 180.0f);
			_dir = RIGHT;
			break;
		case IS::GameObject::Component::AAction::RIGHT:
			_physics->goForward(_speed * IS::Utils::Time::getInstance()->getDeltaTime() * 70);
			break;
		default:
			break;
		}
	}
	if (_input->isKey(_myKeys.left))
	{
		switch (_dir)
		{
		case IS::GameObject::Component::AAction::UP:
			_physics->yawLeft(1.0f, 90.0f);
			_dir = LEFT;
			break;
		case IS::GameObject::Component::AAction::DOWN:
			_physics->yawRight(1.0f, 90.0f);
			_dir = LEFT;
			break;
		case IS::GameObject::Component::AAction::LEFT:
			_physics->goForward(_speed * IS::Utils::Time::getInstance()->getDeltaTime() * 70);
			break;
		case IS::GameObject::Component::AAction::RIGHT:
			_physics->yawRight(1.0f, 180.0f);
			_dir = LEFT;
			break;
		default:
			break;
		}
	}
}

bool IS::GameObject::Component::AAction::pause()
{
	if (_input->isKey(_myKeys.pauses))
		return true;
	return false;
}

bool IS::GameObject::Component::AAction::dropBomb()
{
	if (_input->isKeyDown(_myKeys.bomb))
		return true;
	return false;
}

bool IS::GameObject::Component::AAction::kickBomb()
{
	if (_input->isKey(_myKeys.kick))
		return true;
	return false;
}

bool IS::GameObject::Component::AAction::pause(int id)
{
	if (_input->isButtonKeyDown(_myKeyJoy.pauses, id))
		return true;
	return false;
}

bool IS::GameObject::Component::AAction::dropBomb(int id)
{
	if (_input->isButtonKeyDown(_myKeyJoy.bomb, id))
		return true;
	return false;
}

bool IS::GameObject::Component::AAction::kickBomb(int id)
{
	if (_input->isKey(_myKeys.bomb, id))
		return true;
	return false;
}

void IS::GameObject::Component::AAction::aIMoveForward()
{
	switch (_dir)
	{
	case IS::GameObject::Component::AAction::UP:
		_physics->goForward(_speed * IS::Utils::Time::getInstance()->getDeltaTime() * 70);
		break;
	case IS::GameObject::Component::AAction::DOWN:
		_physics->yawLeft(1.0f, 180.0f);
		_dir = UP;
		break;
	case IS::GameObject::Component::AAction::LEFT:
		_physics->yawRight(1.0f, 90.0f);
		_dir = UP;
		break;
	case IS::GameObject::Component::AAction::RIGHT:
		_physics->yawLeft(1.0f, 90.0f);
		_dir = UP;
		break;
	default:
		break;
	}
}

void IS::GameObject::Component::AAction::aIMoveDown()
{
	switch (_dir)
	{
	case IS::GameObject::Component::AAction::UP:
		_physics->yawLeft(1.0f, 180.0f);
		_dir = DOWN;
		break;
	case IS::GameObject::Component::AAction::DOWN:
		_physics->goForward(_speed * IS::Utils::Time::getInstance()->getDeltaTime() * 70);
		break;
	case IS::GameObject::Component::AAction::LEFT:
		_physics->yawLeft(1.0f, 90.0f);
		_dir = DOWN;
		break;
	case IS::GameObject::Component::AAction::RIGHT:
		_physics->yawRight(1.0f, 90.0f);
		_dir = DOWN;
		break;
	default:
		break;
	}
}

void IS::GameObject::Component::AAction::aIMoveRight()
{
	switch (_dir)
	{
	case IS::GameObject::Component::AAction::UP:
		_physics->yawRight(1.0f, 90.0f);
		_dir = RIGHT;
		break;
	case IS::GameObject::Component::AAction::DOWN:
		_physics->yawLeft(1.0f, 90.0f);
		_dir = RIGHT;
		break;
	case IS::GameObject::Component::AAction::LEFT:
		_physics->yawRight(1.0f, 180.0f);
		_dir = RIGHT;
		break;
	case IS::GameObject::Component::AAction::RIGHT:
		_physics->goForward(_speed * IS::Utils::Time::getInstance()->getDeltaTime() * 70);
		break;
	default:
		break;
	}
}

void IS::GameObject::Component::AAction::aIMoveLeft()
{
	switch (_dir)
	{
	case IS::GameObject::Component::AAction::UP:
		_physics->yawLeft(1.0f, 90.0f);
		_dir = LEFT;
		break;
	case IS::GameObject::Component::AAction::DOWN:
		_physics->yawRight(1.0f, 90.0f);
		_dir = LEFT;
		break;
	case IS::GameObject::Component::AAction::LEFT:
		_physics->goForward(_speed * IS::Utils::Time::getInstance()->getDeltaTime() * 70);
		break;
	case IS::GameObject::Component::AAction::RIGHT:
		_physics->yawRight(1.0f, 180.0f);
		_dir = LEFT;
		break;
	default:
		break;
	}
}

void IS::GameObject::Component::AAction::setSpeed(float speed)
{
	if (speed >= 5)
		_speed = 5;
	else if (speed <= 0)
	{
		_speed = 0.5f;
	}
	else
	{
		_speed = speed;
	}
}

float IS::GameObject::Component::AAction::getSpeed() const
{
	return _speed;
}