//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/Equipment.h"
#include "items/Inventory.h"

namespace questless
{
	//! An item for containing projectiles, such as arrows.
	class Quiver : public EquipmentBase<Quiver>
	{
	public:
		Inventory inventory;

		Quiver(Inventory inventory, Id<Item> id = Id<Item>::make())
			: Item{id}
			, inventory{std::move(inventory)}
		{}

		std::string name() const override { return "Quiver"; }

		double weight() const override { return 0.5; }

		virtual std::vector<uptr<Action>> actions() override
		{
			std::vector<uptr<Action>> actions;
			if (equipped()) {
				actions.push_back(Unequip::make(*this));
			} else {
				actions.push_back(Equip::make(*this));
				actions.push_back(Drop::make(*this));
				actions.push_back(Throw::make(*this));
			}
			return actions;
		}

		double equip_time() const override { return 1.0; }
		double unequip_time() const override { return 1.0; }
	private:
		Requirements requirements() const override { return Requirements{Torsos{1}}; }
	};
}
