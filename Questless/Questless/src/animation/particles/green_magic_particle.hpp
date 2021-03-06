//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "sprite_particle.hpp"

#include "rsrc/particle_fwd.hpp"

namespace ql {
	//! A particle for effects involving green magic.
	struct green_magic_particle : sprite_particle {
		green_magic_particle(rsrc::particle const& resources);

	private:
		//! True if the green magic particle is turning to the right (CW), false if turning left (CCW).
		bool _turning_right;

		auto sprite_particle_subupdate(sec elapsed_time) -> void final;
	};
}
