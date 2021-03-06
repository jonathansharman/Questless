//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "particle_animation.hpp"

#include "rsrc/particle_fwd.hpp"

namespace ql {
	//! Creates drops of blood.
	struct bleeding : particle_animation {
		//! Unit of drops of blood, for use in bleeding animations.
		using drops = cancel::quantity<double, cancel::unit_t<struct bleeding_drops_tag>>;

		//! The number of drops of blood to create per second. Can be less than one.
		cancel::quotient_t<drops, sec> drop_rate;

		//! @param drop_rate The number of drops of blood to create per second. Can be less than one.
		bleeding(rsrc::particle const& resources, cancel::quotient_t<drops, sec> drop_rate)
			: drop_rate{drop_rate}
			, _rsrc{&resources} //
		{}

	private:
		rsrc::particle_ptr _rsrc;

		//! The current accumulation of drops of blood.
		drops _drops{0.0};

		auto particle_animation_subupdate(sec elapsed_time) -> void final;
	};
}
