#pragma once

namespace IS
{
	namespace GameObject
	{
		namespace Component
		{
			class IComponent
			{
			public:
				virtual ~IComponent() { }

				virtual void update() = 0;
			};
		}
	}
}