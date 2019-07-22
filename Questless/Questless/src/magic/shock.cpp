//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "shock.hpp"

#include "agents/agent.hpp"
#include "effects/effect.hpp"
#include "entities/beings/being.hpp"
#include "magic/charge_cost.hpp"
#include "utility/random.hpp"
#include "world/region.hpp"

//! @todo Body part stuff - remove if not used.
#include "entities/beings/body_part.hpp"

namespace ql::magic {
	void cast(ent caster_id, ent gatestone_id, region_tile::point target, dmg::shock damage) {
		// Check range.
		auto const caster_location = reg.get<location>(caster_id);
		if ((caster_location.coords - target).length() > 3_span) { return; }

		// Check and pay cost.
		auto& gatestone = reg.get<ql::gatestone>(gatestone_id);
		auto const mana_cost = cancel::quantity<double, struct mana_tag>{0.2} / 1.0_shock / 1.0_shock * damage * damage;
		if (!charge_cost{gatestone, mana_cost}.check_and_pay()) { return; }

		//! @todo Shock quality.
		double const quality = 1.0;

		//! Add a lightning bolt effect.
		auto& region = reg.get<ql::region>(caster_location.region_id);
		region.add_effect({effects::lightning_bolt{target}});

		if (auto target_entity_id = region.entity_id_at(target)) {
			//! @todo What about damaging non-being objects?
			if (auto target_being = reg.try_get<being>(*target_entity_id)) {
				//! @todo Experimental body part stuff here... Delete or fix.

				// Pick a random strike path from the root to a leaf part.
				body_part* part = &target_being->body.root();
				std::vector<body_part*> struck_parts{part};
				while (!part->attachments.empty()) {
					part = &*part->attachments[uniform(std::size_t{0}, part->attachments.size() - 1)]->part;
					struck_parts.push_back(part);
				}
				// Deal percentage-based damage, split across all struck parts.
				for (auto struck_part : struck_parts) {
					dmg::group shock = damage * quality * uniform(0.5, 1.5) * part->stats.vitality.value() /
						(4.0_hp * struck_parts.size());
					struck_part->take_damage(shock, caster_id);
				}
			}
		}
	}
}
