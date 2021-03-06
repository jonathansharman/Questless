//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "arrow_attack.hpp"
#include "injury.hpp"
#include "lightning_bolt.hpp"
#include "telescope.hpp"

#include "world/coordinates.hpp"

#include <variant>

namespace ql::effects {
	//! An in-game effect perceivable by beings, such as light and sound stimuli.
	struct effect {
		std::variant<arrow_attack, injury, lightning_bolt, telescope> value;

		//! The coordinates from which @p effect originates.
		auto origin() const -> tile_hex_point;

		//! The maximum distance from the origin at which @p effect may be perceived.
		auto range() const -> pace;
	};

	//! @todo Should effects have an origin? If so, find a good way to factor it out so it's not duplicated accross
	//! subtypes.
}
