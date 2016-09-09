#pragma once
#include <IS/GameObject/Components/AComponent.h>
#include <IS/Physics/Core/PhysicObject.h>
#include <IS/input/Input.hh>
#include <OGRE/Ogre.h>
#include <IS/GameObject/GameObject.h>
#include <IS/GameObject/Components/Mesh/MeshComponent.h>
#include <IS/Utils/Time/Time.h>

namespace IS
{
	namespace GameObject
	{
		namespace Component
		{
			class PowerUp : public AComponent
			{
			public:
				enum PowerUpEnum
				{
					NONE,
					SPEED,
					SLOW,
					TIME,
					RANGE,
					MOREBOMB
				};
			public:
				PowerUp(Ogre::SceneNode *sceneNode, IS::Scene::Scene *scene, PowerUpEnum powerUpEnum);
				virtual ~PowerUp();

				virtual void update();

				PowerUpEnum getPowerUpEnum() const;
				void setPowerUpEnum(PowerUpEnum en);

			protected:
				PowerUpEnum		_powerUpEnum;
				float			_timer;
				float			_time;
				IS::Utils::Time	*_timeInstance;
			};
		}
	}
}
