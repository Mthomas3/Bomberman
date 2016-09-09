#include <IS/Physics/Core/PhysicObject.h>
#include <IS/Physics/Exception/PhysicException.h>

#pragma region DynamicObject

IS::Physics::DynamicObject::DynamicObject(Ogre::SceneNode *node, IS::Scene::Scene *scene, Physics::CollisionType type, Collision::CollisionTools *collider)
	: IS::GameObject::Component::AComponent(node, scene), _collider(collider), _radCollision(30.0f)
{
	try
	{
		_entity = nullptr;
		addDynamicObject(this);
		_entity = reinterpret_cast<Ogre::Entity*>(_node->getAttachedObject(0));
		if (_entity == nullptr)
			delete this;
		_collider->register_entity(_entity, type);
	}
	catch (PhysicException &e)
	{
		e << "Physics::DynamicObject::Constructor";
		throw;
	}
}

IS::Physics::DynamicObject::~DynamicObject()
{
	removeDynamicObject(this);
	if (_entity)
		_collider->remove_entity(_entity);
}

bool IS::Physics::DynamicObject::translate(Ogre::Vector3 &&translate, Ogre::Node::TransformSpace space)
{
	Collision::SCheckCollisionAnswer collision = _collider->check_ray_collision(_node->getPosition(), _node->getPosition() + _node->getOrientation() * translate, _radCollision, 1.0f, Ogre::SceneManager::ENTITY_TYPE_MASK, _entity, false);
	if (collision.collided == true)
		return (false);
	_node->translate(translate, space);
	return (true);
}

#pragma endregion

#pragma region StaticObject

IS::Physics::StaticObject::StaticObject(Ogre::SceneNode *node, IS::Scene::Scene *scene, Physics::CollisionType type, Collision::CollisionTools *collider)
	: IS::GameObject::Component::AComponent(node, scene), _collider(collider)
{
	try
	{
		_entity = nullptr;
		addStaticObject(this);
		_entity = reinterpret_cast<Ogre::Entity*>(_node->getAttachedObject(0));
		if (_entity == nullptr)
			delete this;
		_collider->register_static_entity(_entity, _node->getPosition(), _node->getOrientation(), _node->getScale(), type);
	}
	catch (PhysicException &e)
	{
		e << "Physics::StaticObject::Constructor";
		throw;
	}
}

IS::Physics::StaticObject::~StaticObject()
{
	removeStaticObject(this);
	if (_entity)
		_collider->remove_entity(_entity);
}

#pragma endregion