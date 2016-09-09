#pragma once
#include <stdexcept>
#include <limits>
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"

namespace PE
{
	struct Position : public Point3
	{
		Position() : Point3() { }
		Position(float xyz) : Point3(xyz) { }
		Position(float x, float y, float z) : Point3(x, y, z) { }
		Position(Point3 const &point) : Point3(point) { }
		Position(Position const &position) : Point3(position) { }
	};
	struct Angle : public Point3
	{
		Angle() : Point3() { }
		Angle(float xyz) : Point3(xyz) { }
		Angle(float x, float y, float z) : Point3(x, y, z) { }
		Angle(Point3 const &point) : Point3(point) { }
		Angle(Position const &angle) : Point3(angle) { }
	};
}