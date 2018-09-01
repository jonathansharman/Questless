//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <functional>
#include <optional>

#include "action.hpp"
#include "utility/complete.hpp"
#include "utility/reference.hpp"
#include "world/coordinates.hpp"

namespace ql {
	class being;
	class body_part;
	class effect;
	class item;
	class gatestone;
	namespace magic { class spell; }

	struct being_query;
	struct count_query;
	struct direction_query;
	struct item_query;
	struct magnitude_query;
	struct message;
	struct tile_query;
	struct vector_query;

	//! Facilitates control of a being. Includes AIs and the player.
	class agent {
	public:
		agent(being& being) : being{being} {}

		template <typename Derived>
		static auto make(being& being) { return umake<Derived>(being); }

		virtual ~agent() = default;

		agent& operator =(agent const&) = delete;

		being& being;

		//! Chooses and executes an action for the agent's being to perform.
		virtual complete act() = 0;

		//! Causes the agent to perceive the given effect, possibly updating its state accordingly.
		//! @param effect The effect to perceive.
		virtual void perceive(sptr<effect> const& effect) = 0;

		// Queries and messages
		
		//! @todo Update documentation here.

		virtual complete send_message
			( uptr<message> message
			, std::function<complete()> cont
			) const = 0;

		//virtual complete query_count
		//	( uptr<count_query> query
		//	, int default_value
		//	, std::optional<int> min
		//	, std::optional<int> max
		//	, std::function<complete(std::optional<int>)> cont
		//	) const = 0;

		virtual complete query_magnitude
			( uptr<magnitude_query>
			, double default_value
			, std::optional<double> min
			, std::optional<double> max
			, std::function<complete(std::optional<double>)> cont
			) const = 0;

		virtual complete query_tile
			( uptr<tile_query> query
			, std::optional<region_tile::point> origin
			, std::function<bool(region_tile::point)> predicate
			, std::function<complete(std::optional<region_tile::point>)> cont
			) const = 0;

		//virtual complete query_direction
		//	( uptr<direction_query> query
		//	, std::function<complete(std::optional<region_tile::direction>)> cont
		//	) const = 0;

		virtual complete query_vector
			( uptr<vector_query> query
			, std::optional<region_tile::point> origin
			, std::function<bool(region_tile::vector)> predicate
			, std::function<complete(std::optional<region_tile::vector>)> cont
			) const = 0;

		virtual complete query_being
			( uptr<being_query> query
			, std::function<bool(ql::being&)> predicate
			, std::function<complete(std::optional<ql::being*>)> cont
			) const = 0;

		//virtual complete query_item
		//	( uptr<item_query> query
		//	, ql::being& source
		//	, std::function<bool(ql::being&)> predicate
		//	, std::function<complete(std::optional<item*>)> cont
		//	) const = 0;

		// Quick Time Events

		virtual complete aim_missile(region_tile::point source_coords, ql::being& target_being, std::function<complete(body_part*)> cont) const;
		virtual complete get_shock_quality(region_tile::point target_coords, std::function<complete(double)> cont) const;

		virtual complete incant(gatestone& gatestone, std::function<complete(uptr<magic::spell>)> cont) const;
	protected:
		complete idle(action::cont cont);
		complete idle(double duration);
		complete turn(region_tile::direction direction, action::cont cont);
		complete walk(region_tile::direction direction, action::cont cont);
		complete fly();
	};
}
