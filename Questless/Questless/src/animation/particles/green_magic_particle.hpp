//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "sprite_particle.hpp"

namespace ql {
	//! A particle for effects involving green magic.
	class green_magic_particle : public sprite_particle {
	public:
		green_magic_particle();
	private:
		//! True if the green magic particle is turning to the right (CW), false if turning left (CCW).
		bool _turning_right;

		void sprite_particle_subupdate(sec elapsed_time) final;
	};
}
