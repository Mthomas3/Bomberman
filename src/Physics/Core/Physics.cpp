#include <algorithm>
#include <IS/Physics/Core/Physics.h>
#include <IS/Physics/Core/PhysicObject.h>
#include <IS/Physics/Exception/PhysicException.h>

#pragma region Constructor/Destructor

IS::Physics::Physics()
{
	if (Physics::_physicInstance == nullptr)
		Physics::_physicInstance = this;
	else
		throw PhysicException(PhysicException::WARNING, "Physics::Constructor", PhysicError::getError(PhysicError::PHYS_EXIST));
	_collider = new Collision::CollisionTools;
}

IS::Physics::~Physics()
{
	/*for (auto dynInst : IS::Physics::_dynamicObject)
		if (dynInst != nullptr)
			delete dynInst;
	IS::Physics::_dynamicObject.clear();
	for (auto statInst : IS::Physics::_staticObject)
		if (statInst != nullptr)
			delete statInst;
	IS::Physics::_staticObject.clear();

	delete _collider;*/
	IS::Physics::_physicInstance = nullptr;
}

#pragma endregion

#pragma region Instances Tools

const bool IS::Physics::isInstantiate()
{
	return (Physics::_physicInstance != nullptr);
}

template<typename PhysicType>
void IS::Physics::addInstance(std::list<PhysicType *> &instances, PhysicType *new_inst)
{
	if (std::find(instances.begin(), instances.end(), new_inst) != instances.end())
		throw PhysicException(PhysicException::FATAL_ERROR, "Physics::addInstance<" + std::string(typeid(*new_inst).name()) + ">", PhysicError::getError(PhysicError::MEMORY_CORRUPTION));
	instances.push_back(new_inst);
}

template<typename PhysicType>
void IS::Physics::removeInstance(std::list<PhysicType *> &instances, PhysicType *del_inst)
{
	auto it_inst = std::find(instances.begin(), instances.end(), del_inst);
	if (it_inst == instances.end())
		throw PhysicException(PhysicException::FATAL_ERROR, "Physics::removeInstance<" + std::string(typeid(*del_inst).name()) + ">", PhysicError::getError(PhysicError::MEMORY_CORRUPTION));
	instances.erase(it_inst);
}

#pragma endregion

#pragma region DynamicObjects

void IS::Physics::addDynamicObject(IS::Physics::DynamicObject *instance)
{
	if (!IS::Physics::isInstantiate())
		throw PhysicException(PhysicException::WARNING, "Physic::addDynamicObject", PhysicError::getError(PhysicError::PHYS_NOT_EXIST));
	try
	{
		addInstance(IS::Physics::_dynamicObject, instance);
	}
	catch (PhysicException &ep)
	{
		throw ep << "Physic::addDynamicObject";
	}
	catch (std::exception &e)
	{
		throw PhysicException(PhysicException::STD_ERROR, "Physic::addDynamicObject", e.what());
	}
}

void IS::Physics::removeDynamicObject(IS::Physics::DynamicObject *instance)
{
	if (!IS::Physics::isInstantiate())
		throw PhysicException(PhysicException::WARNING, "Physic::removeDynamicObject", PhysicError::getError(PhysicError::PHYS_NOT_EXIST));
	try
	{
		removeInstance(IS::Physics::_dynamicObject, instance);
	}
	catch (PhysicException &ep)
	{
		throw ep << "Physic::removeDynamicObject";
	}
	catch (std::exception &e)
	{
		throw PhysicException(PhysicException::STD_ERROR, "Physic::removeDynamicObject", e.what());
	}
}

#pragma endregion

#pragma region StaticObject Tools

void IS::Physics::addStaticObject(IS::Physics::StaticObject *instance)
{
	if (!IS::Physics::isInstantiate())
		throw PhysicException(PhysicException::WARNING, "Physic::addStaticObject", PhysicError::getError(PhysicError::PHYS_NOT_EXIST));
	try
	{
		addInstance(IS::Physics::_staticObject, instance);
	}
	catch (PhysicException &ep)
	{
		throw ep << "Physic::addStaticObject";
	}
	catch (std::exception &e)
	{
		throw PhysicException(PhysicException::STD_ERROR, "Physic::addStaticObject", e.what());
	}
}

void IS::Physics::removeStaticObject(IS::Physics::StaticObject *instance)
{
	if (!IS::Physics::isInstantiate())
		throw PhysicException(PhysicException::WARNING, "Physic::removeStaticObject", PhysicError::getError(PhysicError::PHYS_NOT_EXIST));
	try
	{
		removeInstance(IS::Physics::_staticObject, instance);
	}
	catch (PhysicException &ep)
	{
		throw ep << "Physic::removeStaticObject";
	}
	catch (std::exception &e)
	{
		throw PhysicException(PhysicException::STD_ERROR, "Physic::removeStaticObject", e.what());
	}
}

#pragma endregion

#pragma region Output Stream

std::ostream &operator<<(std::ostream &os, IS::Physics::PhysicException::StackTrace const &st)
{
	for (auto component : st)
		os << "\tin " << component.c_str() << std::endl;
	return (os);
}

std::ostream &operator<<(std::ostream &os, IS::Physics::PhysicException const &e)
{
	std::string type[4] = { "Warning", "Internal Error", "STD Exception", "Fatal Error" };
	return (os << type[static_cast<int>(e.getErrorLevel())].c_str() << ": " << e.what() << " at " << e.getComponent() << std::endl << e.getStackTrace());
}

#pragma endregion