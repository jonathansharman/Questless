//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "sprite_particle.hpp"

#include "rsrc/particle_fwd.hpp"

namespace ql {
	//! A particle of flame and smoke.
	struct flame_particle : sprite_particle {
		flame_particle(rsrc::particle const& resources);

	private:
		auto sprite_particle_subupdate(sec elapsed_time) -> void final;
	};
}
