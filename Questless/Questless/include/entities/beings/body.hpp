//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <memory>
#include <vector>

#include <boost/iterator/transform_iterator.hpp>

#include "entities/beings/body_part.hpp"
#include "units/window_space.hpp"

namespace ql
{
	class being;

	//! A being's body, which is composed of a tree of body parts.
	class body
	{
	public:
		lazy_bounded<double> blood;

		//! @param owner The being that owns this body.
		//! @param root The root of the body parts tree.
		body(being& owner, uptr<body_part> root);

		body(body&&) = default;

		//! The root body part, to which all other body parts are attached.
		body_part const& root() const { return *_root; }
		//! The root body part, to which all other body parts are attached.
		body_part& root() { return *_root; }

		//! The cumulative vitality of all parts of this body.
		double total_vitality() const { return _total_vitality; }

		//! Iterator to beginning of body parts.
		auto begin() { return _parts.begin(); }
		//! Iterator to end of body parts.
		auto end() { return _parts.end(); }

		//! Const iterator to beginning of body parts.
		auto begin() const
		{
			return boost::make_transform_iterator(_parts.begin(), body_part_ref_to_cref);
		}
		//! Const iterator to end of body parts.
		auto end() const
		{
			return boost::make_transform_iterator(_parts.end(), body_part_ref_to_cref);
		}

		std::vector<ref<body_part>> const& parts() { return _parts; }
		std::vector<ref<head>> const& heads() { return _heads; }
		std::vector<ref<torso>> const& torsos() { return _torsos; }
		std::vector<ref<arm>> const& arms() { return _arms; }
		std::vector<ref<hand>> const& hands() { return _hands; }
		std::vector<ref<leg>> const& legs() { return _legs; }
		std::vector<ref<foot>> const& feet() { return _feet; }
		std::vector<ref<wing>> const& wings() { return _wings; }
		std::vector<ref<tail>> const& tails() { return _tails; }

		//! The body part on this body with the given ID or nullptr if none exists.
		body_part* find_part(id<body_part> id);
		//! The body part on this body with the given ID or nullptr if none exists.
		body_part const* find_part(id<body_part> id) const;

		//! The head on this body with the given ID or nullptr if none exists.
		head* find_head(id<body_part> id);

		//! The torso on this body with the given ID or nullptr if none exists.
		torso* find_torso(id<body_part> id);

		//! The arm on this body with the given ID or nullptr if none exists.
		arm* find_arm(id<body_part> id);

		//! The hand on this body with the given ID or nullptr if none exists.
		hand* find_hand(id<body_part> id);

		//! The leg on this body with the given ID or nullptr if none exists.
		leg* find_leg(id<body_part> id);

		//! The foot on this body with the given ID or nullptr if none exists.
		foot* find_foot(id<body_part> id);

		//! The wing on this body with the given ID or nullptr if none exists.
		wing* find_wing(id<body_part> id);

		//! The tail on this body with the given ID or nullptr if none exists.
		tail* find_tail(id<body_part> id);

		//! Advances the body and all its parts one time unit.
		void update();
	private:
		being& _owner; // Okay to store reference here. A body's lifetime is a subset of its owning being's lifetime.

		uptr<body_part> _root;

		std::vector<ref<body_part>> _parts;

		std::vector<ref<head>> _heads;
		std::vector<ref<torso>> _torsos;
		std::vector<ref<arm>> _arms;
		std::vector<ref<hand>> _hands;
		std::vector<ref<leg>> _legs;
		std::vector<ref<foot>> _feet;
		std::vector<ref<wing>> _wings;
		std::vector<ref<tail>> _tails;

		// Cumulative attributes
		double _total_vitality;

		static cref<body_part> body_part_ref_to_cref(ref<body_part> body_part)
		{
			return static_cast<cref<ql::body_part>>(body_part);
		}
	};
}
