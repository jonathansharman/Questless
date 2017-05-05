//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <vector>
#include <deque>
#include <utility>
#include <functional>
#include <memory>

#include "entities/Entity.h"
#include "entities/beings/Abilities.h"
#include "entities/beings/Body.h"
#include "entities/beings/statuses/Status.h"
#include "entities/Perception.h"
#include "items/armor/Armor.h"
#include "items/Inventory.h"
#include "items/Item.h"
#include "items/weapons/Weapon.h"
#include "spell/Spell.h"
#include "stats/Stats.h"
#include "utility/Bounded.h"
#include "utility/DynamicProperty.h"
#include "utility/Event.h"
#include "utility/Id.h"
#include "utility/utility.h"

namespace questless
{
	class Agent;

	struct Health : TaggedType<double> { using TaggedType::TaggedType; };
	struct Mana : TaggedType<double> { using TaggedType::TaggedType; };
	struct Energy : TaggedType<double> { using TaggedType::TaggedType; };
	struct Satiety : TaggedType<double> { using TaggedType::TaggedType; };
	struct Alertness : TaggedType<double> { using TaggedType::TaggedType; };
	struct BusyTime : TaggedType<double> { using TaggedType::TaggedType; };
	struct Dead : TaggedType<bool> { using TaggedType::TaggedType; };

	//! An animate entity.
	class Being : public Entity
	{
	public:
		///////////
		// Types //
		///////////

		using ref_less_t = std::function<bool(Being const&, Being const&)>;
		using ptr_less_t = std::function<bool(uptr<Being> const&, uptr<Being> const&)>;

		///////////////
		// Constants //
		///////////////

		//! @todo Probably move some of these to private? (Sleeping stuff goes in Sleeping?)

		// Energy
		static constexpr double energy_rate = 1.0; //!< Energy gained per turn (awake or asleep).
		static constexpr double energy_rate_asleep = 2.0; //!< Additional energy gained per turn asleep.
		static constexpr double energy_strength_penalty = 0.5; //!< Proportion of base strength removed at zero energy.
		static constexpr double energy_endurance_penalty = 0.5; //!< Proportion of base endurance removed at zero energy.

		// Satiety
		static constexpr double min_satiety = 0.0;
		static constexpr double max_satiety = 100.0;
		static constexpr double satiety_rate = -0.05; //!< Satiety gained per turn (awake or asleep).
		static constexpr double satiety_rate_asleep = 0.025; //!< Additional satiety gained per turn asleep.
		static constexpr double satiety_health_regen_penalty = 1.0; //!< Proportion of base health regeneration removed at zero satiety.
		static constexpr double satiety_mana_regen_penalty = 1.0; //!< Proportion of base mana regeneration removed at zero satiety.

		// Alertness
		static constexpr double min_alertness = 0.0;
		static constexpr double max_alertness = 100.0;
		static constexpr double alertness_rate = -0.1; //!< Alertness gained per turn (awake or asleep).
		static constexpr double alertness_rate_asleep = 0.3; //!< Additional alertness gained per turn asleep.
		static constexpr double alertness_agility_penalty = 0.75; //!< Proportion of base agility removed at zero alertness.
		static constexpr double alertness_dexterity_penalty = 0.75; //!< Proportion of base dexterity removed at zero alertness.
		static constexpr double alertness_intellect_penalty = 0.75; //!< Proportion of base intellect removed at zero alertness.
		static constexpr double health_regen_asleep_factor = 1.0; //!< Additional health regeneration multiplier while asleep.
		static constexpr double mana_regen_asleep_factor = 1.0; //!< Additional mana regeneration multiplier while asleep.

		// Strength
		static constexpr double strength_factor = 0.01; //!< Proportion of strength by which base damage is multiplied.

		// Endurance
		static constexpr double endurance_factor = 0.01; //!< Proportion of endurance by which damage after armor is divided.

		// Intellect
		static constexpr double intellect_factor = 0.01; //!< Proportion of intelligence by which spell incantations are divided.

		// Temperature
		static constexpr double temperature_damage_factor = 1.0; //!< Burn damage taken = (temp - max) / (max - min)) * factor. Freeze damage taken = (min - temp) / (max - min)) * factor.

		/////////////////
		// Public Data //
		/////////////////

		Id<Being> const id;

		Body body;

		Inventory inventory;

		// Stats

		Stats base_stats; //!< Base stats, prior to application of status modifiers.
		Stats stats; //!< Effective stats, accounting for all modifiers from base.

		// Conditions

		DynamicProperty<double> health;
		DynamicProperty<double> mana;
		DynamicProperty<double> energy;
		Bounded<double, min_satiety, max_satiety> satiety;
		Bounded<double, min_alertness, max_alertness> alertness;

		DynamicProperty<double> busy_time;
		bool dead;
		RegionTileCoords::Direction direction;

		// Abilities

		Abilities abilities;

		// Event Handlers

		Event<Damage&, BodyPart*, std::optional<Id<Being>>> before_take_damage;
		Event<Damage&, BodyPart*, std::optional<Id<Being>>> after_take_damage;

		Event<Damage&, BodyPart*, Id<Being>> before_deal_damage;
		Event<Damage&, BodyPart*, Id<Being>> after_deal_damage;

		Event<double&, BodyPart*, std::optional<Id<Being>>> before_receive_heal;
		Event<double&, BodyPart*, std::optional<Id<Being>>> after_receive_heal;

		Event<double&, BodyPart*, Id<Being>> after_give_heal;
		Event<double&, BodyPart*, Id<Being>> before_give_heal;

		Event<std::optional<Id<Being>>> before_die;
		Event<std::optional<Id<Being>>> after_die;

		Event<Id<Being>> before_kill;
		Event<Id<Being>> after_kill;

		////////////////////
		// Public Methods //
		////////////////////

		virtual ~Being();

		//! @param out A stream object into which the serialized being is inserted.
		void serialize(std::ostream& out) const override;

		//! The agent responsible for this being.
		Agent& agent() { return *_agent; }

		//! Whether the being is corporeal.
		virtual bool corporeal() const = 0;

		//! This being's perception of the tile at @p region_tile_coords in its region.
		Perception perception_of(RegionTileCoords region_tile_coords) const;

		//! Causes the being to perform an action.
		void act();

		//! Adds the given delayed action to the being's delayed actions queue.
		//! @param delay The delay before the action is performed.
		//! @param cont The continuation function to call once the action completes.
		//! @param action The action to perform after the delay.
		void add_delayed_action(double delay, Action::cont_t cont, uptr<Action> action);

		//! Clears the being's delayed actions queue.
		void clear_delayed_actions();

		//! The being's equipped shields.
		std::vector<std::unique_ptr<Armor>> const& shields() { return _shields; }

		//! Recalculates this being's stats. Called automatically with each update.
		void refresh_stats() { stats = get_stats(); }

		//! Advances the being one time unit.
		void update() final;

		//! Causes the being to take damage from the specified source being.
		//! @param damage Damage to be applied to this being.
		//! @param part The body part to damage, or nullptr if the damage should be applied directly to the being.
		//! @param opt_source_id The ID of the being which caused the damage, if any.
		void take_damage(Damage& damage, BodyPart* part, std::optional<Id<Being>> opt_source_id);

		//! Causes the being to be healed by the specified source being.
		//! @param amount Health to be restored to this being.
		//! @param part The body part to heal, or nullopt if the damage should be applied directly to the being.
		//! @param opt_source_id The ID of the being which caused the healing, if any.
		void heal(double amount, BodyPart* part, std::optional<Id<Being>> opt_source_id);

		void add_status(std::unique_ptr<Status> status);
	protected:
		Being(const std::function<std::unique_ptr<Agent>(Being&)>& make_agent, Id<Being> id, Body body, const std::function<Stats()>& make_base_stats);
		Being(std::istream& in, Body body);

		virtual Body make_body() = 0;
	private:
		std::unique_ptr<Agent> _agent; //!< The agent responsible for this being.

		//! @todo Maybe use a std::pair of actions and conts, and then use the auto [action, cont] = ... syntax in act() once supported?
		std::deque<double> _action_delays; //!< The queue of delays before the next action in the delayed actions queue should begin.
		std::deque<uptr<Action>> _delayed_actions; //!< The queue of delayed actions to occur when this being is not busy.
		std::deque<Action::cont_t> _delayed_action_conts; //!< The queue of delayed action continuations.

		// Statuses

		std::vector<std::unique_ptr<Status>> _statuses;

		// Items

		std::vector<std::unique_ptr<Weapon>> _equipped_weapons;
		std::vector<std::unique_ptr<Armor>> _shields;

		// Methods

		Stats get_base_stats_plus_body_stats();
		Stats get_stats();

		std::function<void(double&, double const&)> health_mutator();
		std::function<void(double&, double const&)> mana_mutator();
		std::function<void(double&, double const&)> energy_mutator();
		std::function<void(double&, double const&)> busy_time_mutator();
	};

	DEFINE_ELEMENT_BASE(Being, Entity)
}
