//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "world/coordinates.hpp"

namespace ql {
	namespace effects {
		struct telescope {
			tile_hex_point origin;
			id const caster_id;

			constexpr auto range() const -> pace {
				return 3_pace;
			}
		};
	}
}
