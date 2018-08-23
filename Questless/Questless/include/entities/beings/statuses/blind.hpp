//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/beings/statuses/status.hpp"

namespace ql {
	//! Decreases visual acuity.
	class blind : public status {
	public:
		//! @param magnitude How powerful the status modifier is.
		//! @param duration The number of turns remaining before the status modifier expires.
		//! @param source_id The ID of the being that caused the status modifier, if any.
		blind(double magnitude, int duration, std::optional<id<being>> source_id = std::nullopt)
			: status("Blind", duration, source_id)
			, _magnitude{magnitude}
			, _modifiers{umake<visual_acuity_modifier>(-magnitude)}
		{}

		virtual status_type type() const { return status_type::bane; }

		std::vector<uptr<modifier>> const& modifiers() const final { return _modifiers; }
	private:
		double _magnitude;
		std::vector<uptr<modifier>> _modifiers;
	};
}
