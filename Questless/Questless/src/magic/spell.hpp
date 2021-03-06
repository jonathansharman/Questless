//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "heal.hpp"
#include "shock.hpp"
#include "teleport.hpp"
#include "telescope.hpp"

#include <variant>

namespace ql::magic {
	//! A magical spell that can be cast by a being.
	struct spell {
		std::variant<heal, shock, teleport, telescope> value;
	};
}
