//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "agents/agent.hpp"
#include "reg.hpp"
#include "world/coordinates.hpp"

namespace ql {
	id make_human(reg& reg, id human_id, location location, agent agent);
}
