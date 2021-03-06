//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#include "bleeding.hpp"

#include "particles/blood_particle.hpp"

namespace ql {
	auto bleeding::particle_animation_subupdate(sec elapsed_time) -> void {
		_drops += drop_rate * elapsed_time;
		while (_drops > drops{1.0}) {
			particles.push_front(umake<blood_particle>(*_rsrc));
			_drops -= drops{1.0};
		}
	}
}
