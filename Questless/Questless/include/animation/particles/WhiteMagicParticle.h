//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation/particles/Particle.h"
#include "utility/utility.h"

namespace questless
{
	//! A particle for effects involving white magic.
	class WhiteMagicParticle : public Particle
	{
	public:
		//! @param position The particle's starting position.
		WhiteMagicParticle(units::GamePoint position) : Particle
			{ position
			, random_displacement(80.0) / 1.0s
			, units::GameVector{0.0, 50.0} / 1.0s / 1.0s
			, random_angle()
			, uniform(-1.0, 1.0) * _dtheta_max / 1.0s
			, Scale{1.0}
			, units::GameScaleVelocity{0.0}
			, Lifetime{units::GameSeconds{uniform(2.0, 2.5)}}
			}
		{}
	private:
		static constexpr units::Hertz _vx_percent_drag_rate = 1.8_Hz;
		static constexpr units::GameRadians _dtheta_max = 2.0 * units::GameRadians::circle();

		void subupdate() final;

		sdl::Texture const& texture() const final;
	};
}
