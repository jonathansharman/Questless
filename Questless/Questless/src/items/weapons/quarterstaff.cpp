//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "quarterstaff.hpp"

#include "items/breakable.hpp"
#include "items/equipment.hpp"
#include "items/item.hpp"

namespace ql {
	void quarterstaff::strike() {
		//! @todo This.

		dmg::group damage = {24_bludgeon};
		tick cooldown = 20_tick;
	}

	void quarterstaff::jab() {
		//! @todo This.

		dmg::group damage = {12_bludgeon};
		tick cooldown = 13_tick;
	}

	id make_quarterstaff(id id) {
		make_item(id, 2.5_mass);

		make_breakable(id, {500_durability, 500_durability});

		make_equipment(id,
			std::nullopt,
			{equipment::tab{body_part::tag::hand, std::nullopt}, equipment::tab{body_part::tag::hand, std::nullopt}},
			1_ap,
			1_ap);

		reg.assign<quarterstaff>(id, id);

		return id;
	}
}
