#pragma once
#include <IS/Physics/Core/Physics.h>

namespace IS
{
	class Physics::PhysicError
	{
	public:
		enum ErrorType { MEMORY_CORRUPTION, PHYS_EXIST, PHYS_NOT_EXIST, PHYSOBJ_NOT_LINKED };

	public:
		inline static const std::string getError(ErrorType errType) { return (_error[static_cast<size_t>(errType)]); }

	private:
		static const std::string _error[];
	};
}