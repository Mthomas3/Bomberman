	#include <IS/GameObject/Components/Player/Player.h>
#include <IS/GameObject/Components/Bomb/BombManager.h>
#include <IS/GUI/GUI.hh>

IS::GameObject::Component::Player::Player(Ogre::SceneNode *sceneNode, IS::Scene::Scene *scene, int id, Dir dir, Ogre::Entity *entity) : IS::GameObject::Component::APlayer(sceneNode, scene, id, dir, entity)
{
	//_id = id;
//	_bombManager = new IS::GameObject::Component::BombManager(sceneNode, scene);
	//_powerUp = new IS::GameObject::Component::PowerUpOnPlayer(sceneNode, scene, this);
	//entity->getAnimationState("mixamorig:RightUpLeg");
//	_scene->getGameObjectBySceneNode(_node->getParentSceneNode())->getComponent<IS::GameObject::Component::MeshComponent>();

}

IS::GameObject::Component::Player::Player(Ogre::SceneNode *sceneNode, IS::Scene::Scene *scene, int id, float speed, Dir dir, Ogre::Entity *entity) : IS::GameObject::Component::APlayer(sceneNode, scene, id, speed ,dir, entity)
{
//	_id = id;
	//_bombManager = new IS::GameObject::Component::BombManager(sceneNode, scene);
	//_powerUp = new IS::GameObject::Component::PowerUpOnPlayer(sceneNode, scene, this);
}

IS::GameObject::Component::Player::~Player()
{
	//death animation
}

void IS::GameObject::Component::Player::update()
{
	if (_id == -2)
		return;
	if (IS::Utils::Time::getInstance()->getRatio() != 0)
	{
		if (_id == -1)
		{
			Gui::GuiMenu::setBomb(_bombManager->getNbBombDispo(), 0);
			Gui::GuiMenu::setLife(_nbLife, 0);
			Gui::GuiMenu::setPowerUp(_bombManager->getRange() / 50.0f, 0);
		}
		else if (_id == 0 || _id == -3)
		{
			Gui::GuiMenu::setBomb(_bombManager->getNbBombDispo(), 1);
			Gui::GuiMenu::setLife(_nbLife, 1);
			Gui::GuiMenu::setPowerUp(_bombManager->getRange() / 50.0f, 2);
		}
		else if (_id == 1 || _id == -4)
		{
			Gui::GuiMenu::setBomb(_bombManager->getNbBombDispo(), 2);
			Gui::GuiMenu::setLife(_nbLife, 2);
			Gui::GuiMenu::setPowerUp(_bombManager->getRange() / 50.0f, 2);
		}
		else if (_id == 2 || _id == -5)
		{
			Gui::GuiMenu::setBomb(_bombManager->getNbBombDispo(), 3);
			Gui::GuiMenu::setLife(_nbLife, 3);
			Gui::GuiMenu::setPowerUp(_bombManager->getRange() / 50.0f, 3);
		}
	}
	if (_id > -2)
	{
			_powerUp->update();
			//_animationState->addTime(IS::Utils::Time::getInstance()->getDeltaTime());
			if (_id == -1)
			{
				moveKey();
				if (dropBomb())
					_bombManager->createBomb();
				if (pause())
				{
					IS::Utils::Time::getInstance()->setRatio(0);
					Gui::GuiMenu::pause();
				}
			}
			else
			{
				moveJoy(_id);
				if (dropBomb(_id))
					_bombManager->createBomb();
				if (pause(_id))
				{
					IS::Utils::Time::getInstance()->setRatio(0);
					Gui::GuiMenu::pause();
				}
			}
	}

		if (_isSlow && _underSlow)
			_slowTimer -= IS::Utils::Time::getInstance()->getDeltaTime();
		if (_underSlow && _slowTimer < 0.0f)
		{
			IS::Utils::Time::getInstance()->setRatio(1.0f);
			setSpeed(getSpeed() - 2);
			_underSlow = false;
			_isSlow = false;
		}
		if (_isSlow && !_underSlow)
		{
			_slowTimer = 1.5f;
			IS::Utils::Time::getInstance()->setRatio(0.5f);
			setSpeed(getSpeed() + 2);
			_underSlow = true;
		}
}
/*
IS::GameObject::Component::BombManager * IS::GameObject::Component::Player::getBombManager() const
{
	return _bombManager;
}*/
