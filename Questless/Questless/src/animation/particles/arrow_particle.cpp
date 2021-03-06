//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#include "arrow_particle.hpp"

#include "rsrc/particle.hpp"
#include "utility/random.hpp"

namespace ql {
	using namespace view::literals;

	arrow_particle::arrow_particle(rsrc::particle const& resources, view::point source, view::point target)
		: sprite_particle{0.0_s, resources.arrow}
		, _target{target} //
	{
		setPosition(to_sfml(source));
		angle = random_radians();
		auto target_vector = target - source;
		if (target_vector == view::vector::zero()) {
			lifetime = 0.0_s;
		} else {
			// Set velocity towards the target.
			auto heading = target_vector / target_vector.length();
			velocity = heading * 1'000.0_px / 1.0_s;
			// Set lifetime / time left such that the arrow will disappear when it reaches the target.
			lifetime = target_vector.length() / velocity.length();
			time_left = lifetime;
		}
	}
}
