#include "IS/GameObject/Components/PowerUp/PowerUp.h"

IS::GameObject::Component::PowerUp::PowerUp(Ogre::SceneNode * sceneNode, IS::Scene::Scene * scene, PowerUpEnum powerUpEnum) : IS::GameObject::Component::AComponent(sceneNode, scene)
{
	_powerUpEnum = powerUpEnum;
	_time = 0.01f;
	_timer = _time;
	_timeInstance = IS::Utils::Time::getInstance();
}

IS::GameObject::Component::PowerUp::~PowerUp()
{

}

void IS::GameObject::Component::PowerUp::update()
{
	_timer -= _timeInstance->getDeltaTime();
	if (_timer < 0.0f)
	{
		_node->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(2));
		_timer = _time;	
	}
}

IS::GameObject::Component::PowerUp::PowerUpEnum IS::GameObject::Component::PowerUp::getPowerUpEnum() const
{
	return _powerUpEnum;
}

void IS::GameObject::Component::PowerUp::setPowerUpEnum(PowerUpEnum en)
{
	_powerUpEnum = en;
}
