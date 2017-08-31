//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "agents/Agent.h"
#include "entities/beings/Being.h"
#include "world/Region.h"

namespace questless
{
	// Quick Time Events

	Complete Agent::get_lightning_bolt_quality(RegionTile::Point /*target_coords*/, std::function<Complete(double)> cont) const
	{
		return cont(1.0);
	}

	Complete Agent::incant(Gatestone& /*gatestone*/, std::function<Complete(uptr<spell::Spell>)> cont) const
	{
		return cont(nullptr);
	}

	// Actions

	Complete Agent::idle(Action::cont_t cont)
	{
		return query_magnitude(std::make_unique<MagnitudeQueryWaitTime>(), 10.0, 0.0, std::nullopt,
			[this, cont](std::optional<double> duration) {
				if (duration) {
					being.busy_time += *duration;
					return cont(Action::Result::success);
				} else {
					return cont(Action::Result::aborted);
				}
			}
		);
	}

	Complete Agent::idle(double duration)
	{
		being.busy_time += duration;
		return Complete{};
	}

	Complete Agent::turn(RegionTile::Direction direction, Action::cont_t cont)
	{
		if (being.stats.agility > 0.0) {
			// Base cost of turning any amount, as a percentage of the agility factor.
			constexpr double base_cost_factor = 0.1;
			
			// Cost per turning distance from current direction to new direction, as a percentage of the agility factor.
			constexpr double cost_factor_per_turn = 0.1;

			double const agility_factor = 100.0 / being.stats.agility;
			//! @todo Account for terrain.
			double const turn_cost_factor = cost_factor_per_turn * RegionTile::distance(being.direction, direction);
			being.busy_time += agility_factor * (base_cost_factor + turn_cost_factor);
			being.direction = direction;

			return cont(Action::Result::success);
		} else {
			return cont(Action::Result::aborted);
		}
	}

	Complete Agent::walk(RegionTile::Direction direction, Action::cont_t cont)
	{
		if (being.stats.agility > 0.0) {
			// Base cost of moving, as a percentage of the agility factor.
			constexpr double base_cost_factor = 1.0;

			// Cost per turning distance from direction faced to direction moved, as a percentage of the agility factor.
			constexpr double cost_factor_per_turn = 0.1;

			double const agility_factor = 100.0 / being.stats.agility;
			if (being.region->move(being, being.coords.neighbor(direction))) {
				double const strafe_cost_factor = cost_factor_per_turn * RegionTile::distance(being.direction, direction);
				being.busy_time += agility_factor * (base_cost_factor + strafe_cost_factor); //! @todo Account for terrain.
				
				return cont(Action::Result::success);
			}
		}
		return cont(Action::Result::aborted);
	}

	Complete Agent::fly()
	{
		//! @todo This.
		return Complete{};
	}
}
