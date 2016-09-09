#include <IS/Physics/Core/Physics.h>
#include <IS/Physics/Exception/PhysicError.h>

#pragma region Physics
IS::Physics *IS::Physics::_physicInstance = nullptr;
std::list<IS::Physics::DynamicObject *> IS::Physics::_dynamicObject;
std::list<IS::Physics::StaticObject *> IS::Physics::_staticObject;
Collision::CollisionTools *IS::Physics::_collider;
#pragma endregion

#pragma region Physics::PhysicError
const std::string IS::Physics::PhysicError::_error[] =
{
	"Memory corruption",
	"Physics already has an instance",
	"Physics doesn't have an instance",
	"Physics body was not linked"
};
#pragma endregion