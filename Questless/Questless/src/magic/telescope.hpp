//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "color.hpp"

#include "quantities/game_time.hpp"
#include "reg.hpp"

namespace ql::magic {
	//! Temporarily increases the caster's visual acuity.
	struct telescope {
		static constexpr magic::color color = magic::color::green;
		static constexpr tick cooldown = 5_tick;

		void cast(ent caster_id, ent gatestone_id);
	};
}
