//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "particle_animation.hpp"
#include "particles/flame_particle.hpp"

namespace ql
{
	//! Creates sparks, smoke, and flame.
	class flame : public particle_animation
	{
	private:
		void particle_animation_subupdate() final
		{
			constexpr int n_flames = 2;
			for (int i = 0; i < n_flames; ++i) {
				push_front(umake<flame_particle>());
			}
		}
	};
}
