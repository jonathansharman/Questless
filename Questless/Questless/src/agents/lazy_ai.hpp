//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "reg.hpp"
#include "utility/future.hpp"

namespace ql {
	namespace effects {
		struct effect;
	}

	//! An AI that does nothing.
	struct lazy_ai {
		ent id;

		std::future<void> act() {
			return make_ready_future();
		}

		void perceive(effects::effect const&) {}
	};
}
