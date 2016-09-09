#pragma once
#include <IS/GameObject/Components/AComponent.h>
#include <IS/Physics/Core/Physics.h>
#include <IS/GameObject/Components/Mesh/MeshComponent.h>
namespace IS
{
	class Physics::DynamicObject : public GameObject::Component::AComponent
	{
	public:
		DynamicObject(Ogre::SceneNode *, IS::Scene::Scene *, Physics::CollisionType, Collision::CollisionTools *);
		~DynamicObject();

	public:
		inline void setRadiusCollision(const float radCollision) { _radCollision = radCollision; }
		inline const float getRadiusCollision() { return (_radCollision); }

	public:
		inline bool goForward(const float dtime, const float speed = 1) {  return (translate(Ogre::Vector3::UNIT_Z * dtime * speed, Ogre::Node::TransformSpace::TS_LOCAL)); }
		inline bool goBackward(const float dtime, const float speed = 1) { return (translate(Ogre::Vector3::NEGATIVE_UNIT_Z * dtime * speed, Ogre::Node::TransformSpace::TS_LOCAL)); }
		inline bool goRight(const float dtime, const float speed = 1) { return (translate(Ogre::Vector3::UNIT_X * dtime * speed, Ogre::Node::TransformSpace::TS_LOCAL)); }
		inline bool goLeft(const float dtime, const float speed = 1) { return (translate(Ogre::Vector3::NEGATIVE_UNIT_X * dtime * speed, Ogre::Node::TransformSpace::TS_LOCAL)); }
		inline bool goUp(const float dtime, const float speed = 1) { return (translate(Ogre::Vector3::UNIT_Y * dtime * speed, Ogre::Node::TransformSpace::TS_LOCAL)); }
		inline bool goDown(const float dtime, const float speed = 1) { return (translate(Ogre::Vector3::NEGATIVE_UNIT_Y * dtime * speed, Ogre::Node::TransformSpace::TS_LOCAL)); }

	public:
		inline void yawLeft(const float dtime, const float angularSpeed = 90) { _node->yaw(Ogre::Degree(angularSpeed) * dtime, Ogre::Node::TransformSpace::TS_LOCAL); }
		inline void yawRight(const float dtime, const float angularSpeed = 90) { _node->yaw(Ogre::Degree(-angularSpeed) * dtime, Ogre::Node::TransformSpace::TS_LOCAL); }
		inline void rollRight(const float dtime, const float angularSpeed = 90) { _node->roll(Ogre::Degree(angularSpeed) * dtime, Ogre::Node::TransformSpace::TS_LOCAL); }
		inline void rollLeft(const float dtime, const float angularSpeed = 90) { _node->roll(Ogre::Degree(-angularSpeed) * dtime, Ogre::Node::TransformSpace::TS_LOCAL); }
		inline void pitchUp(const float dtime, const float angularSpeed = 90) { _node->pitch(Ogre::Degree(angularSpeed) * dtime, Ogre::Node::TransformSpace::TS_LOCAL); }
		inline void pitchDown(const float dtime, const float angularSpeed = 90) { _node->pitch(Ogre::Degree(-angularSpeed) * dtime, Ogre::Node::TransformSpace::TS_LOCAL); }

	private:
		bool translate(Ogre::Vector3&&, Ogre::Node::TransformSpace = Ogre::Node::TransformSpace::TS_LOCAL);

	public:
		void update() { }

	private:
		Collision::CollisionTools *_collider;
		Ogre::Entity *_entity;
		float _radCollision;
	};

	class Physics::StaticObject : public GameObject::Component::AComponent
	{
	public:
		StaticObject(Ogre::SceneNode *, IS::Scene::Scene *, Physics::CollisionType, Collision::CollisionTools *);
		~StaticObject();

	public:
		void update() { }

	private:
		Collision::CollisionTools *_collider;
		Ogre::Entity *_entity;
	};
}