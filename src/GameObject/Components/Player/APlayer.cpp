#include <IS/GameObject/Components/Player/APlayer.h>
#include <IS/GameObject/Components/Bomb/BombManager.h>

IS::GameObject::Component::APlayer::APlayer(Ogre::SceneNode *sceneNode, IS::Scene::Scene *scene, int id, Dir dir, Ogre::Entity *entity) : IS::GameObject::Component::AAction(sceneNode, scene, dir, entity)
{
	_id = id;
	_bombManager = new IS::GameObject::Component::BombManager(sceneNode, scene);
	_powerUp = new IS::GameObject::Component::PowerUpOnPlayer(sceneNode, scene, this);
	_isAlive = true;
	_isSlow = false;
	_underSlow = false;
	_slowTimer = 0.0f;
}

IS::GameObject::Component::APlayer::APlayer(Ogre::SceneNode *sceneNode, IS::Scene::Scene *scene, int id, float speed, Dir dir, Ogre::Entity *entity) : IS::GameObject::Component::AAction(sceneNode, scene, dir, speed, entity)
{
	_id = id;
	_bombManager = new IS::GameObject::Component::BombManager(sceneNode, scene);
	_powerUp = new IS::GameObject::Component::PowerUpOnPlayer(sceneNode, scene, this);
}

IS::GameObject::Component::APlayer::~APlayer()
{
	//death animation
}

/*void IS::GameObject::Component::Player::update()
{
	_powerUp->update();
	//_animationState->addTime(IS::Utils::Time::getInstance()->getDeltaTime());
	if (_id == -1)
		moveKey();
	else
		moveJoy(_id);
	if (dropBomb())
	{
		_bombManager->createBomb();
	}
}*/

IS::GameObject::Component::BombManager * IS::GameObject::Component::APlayer::getBombManager() const
{
	return _bombManager;
}

int IS::GameObject::Component::APlayer::getNbLife() const
{
	return _nbLife;
}

void IS::GameObject::Component::APlayer::setNbLife(int nbLife)
{
	_nbLife = nbLife;
}

bool IS::GameObject::Component::APlayer::getAlive() const
{
	return _isAlive;
}

void IS::GameObject::Component::APlayer::setAlive(bool alive)
{
	_isAlive = alive;
}

void IS::GameObject::Component::APlayer::setSlow(bool slow)
{
	_isSlow = slow;
}
