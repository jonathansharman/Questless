//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <variant>

namespace ql::queries::magnitude {
	struct heal {};
	struct shock {};
	struct wait_time {};

	using any = std::variant
		< heal
		, shock
		, wait_time
		>;
}