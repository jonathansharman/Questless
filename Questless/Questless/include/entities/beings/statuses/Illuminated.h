//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/beings/statuses/Status.h"
#include "entities/beings/stats/Modifier.h"

namespace questless
{
	//! Reduces stealth and black magic.
	class Illuminated : public Status
	{
	public:
		//! @param magnitude How powerful the status modifier is.
		//! @param duration The number of turns remaining before the status modifier expires.
		//! @param source_id The ID of the being that caused the status modifier, if any.
		Illuminated(double magnitude, int duration, std::optional<Id<Being>> source_id = std::nullopt)
			: Status("Illuminated", duration, source_id)
			, _magnitude{magnitude}
			, _modifiers
				{ std::make_unique<StealthModifier>(-_magnitude)
				, std::make_unique<BlackMagicModifier>(-_magnitude)
				}
		{}

		virtual Type type() const { return Type::debuff; }

		std::vector<uptr<Modifier>> const& modifiers() const override { return _modifiers; }
	private:
		double _magnitude;
		std::vector<uptr<Modifier>> _modifiers;
	};
}
