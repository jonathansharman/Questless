//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "sprite_particle.hpp"

#include "rsrc/particle_fwd.hpp"

namespace ql {
	//! A particle for effects involving blue magic.
	struct blue_magic_particle : sprite_particle {
		blue_magic_particle(rsrc::particle const& resources);

	private:
		auto sprite_particle_subupdate(sec) -> void final {}
	};
}
