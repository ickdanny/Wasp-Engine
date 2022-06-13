#pragma once

namespace wasp::game::components {

	enum class CollisionCommands {
		death,
		takeDamage
	};

	//When creating a collision type, pass the derived type as the template parameter
	//such that the compiler will instantiate a separate Source and Target type for
	//each collision type.
	template <typename Derived>
	struct CollisionType {
		struct Source {
			CollisionCommands command;
		};
		struct Target {
			CollisionCommands command;
		};
	};
}