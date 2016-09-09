#pragma once
#include <list>
#include <IS/Physics/MOC/NewMOC.h>
#define NOT_IMPLEMENTED(fncName) throw std::exception(#fncName" not currently implemented");

namespace IS
{
	namespace Scene
	{
		class Scene;
	}
}

namespace IS
{
	class Physics
	{
	public:
		typedef Collision::ECollisionType CollisionType;

	public:
		class DynamicObject;
		class StaticObject;
		class PhysicException;

	protected:
		class PhysicError;

	public:
		Physics();
		~Physics();

	public:
		static const bool isInstantiate();

	public:
		template<typename PhysicType>
		static PhysicType *newPhysicObject(Ogre::SceneNode *node, IS::Scene::Scene *scene, CollisionType type) { return (new PhysicType(node, scene, type, _collider)); }

	private:
		static void addDynamicObject(DynamicObject *);
		static void removeDynamicObject(DynamicObject *);
		static void addStaticObject(StaticObject *);
		static void removeStaticObject(StaticObject *);

	private:
		template<typename PhysicType>
		static void addInstance(std::list<PhysicType *> &, PhysicType *);
		template<typename PhysicType>
		static void removeInstance(std::list<PhysicType *> &, PhysicType *);

	private:
		static Collision::CollisionTools *_collider;
		static Physics *_physicInstance;
		static std::list<DynamicObject *> _dynamicObject;
		static std::list<StaticObject *> _staticObject;
	};
}