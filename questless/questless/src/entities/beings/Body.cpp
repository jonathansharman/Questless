//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/Body.h"

#include <deque>
#include <algorithm>
#include <memory>

#include "entities/beings/BodyPart.h"

using namespace units;

namespace questless
{
	Body::Body(Being& owner, uptr<BodyPart> root) : _owner{owner}, _root{std::move(root)}
	{
		int x_min = 0;
		int y_min = 0;
		int x_max = 0;
		int y_max = 0;

		// Walk the parts tree to build the parts lists and compute bounds.
		std::deque<ref<BodyPart>> work_list;
		work_list.push_back(*_root);
		while (!work_list.empty()) {
			BodyPart& part = work_list.front();
			_parts.push_back(part);
			PartAttacher attacher{*this};
			part.accept(attacher);

			for (ScreenSpace::Box const& region : part.regions()) {
				x_min = std::min(x_min, left(region));
				x_max = std::max(x_max, right(region));
				y_min = std::min(y_min, top(region));
				y_max = std::max(y_max, bottom(region));
			}

			// Remove current part from work list and add its children.
			work_list.pop_front();
			for (uptr<BodyPart> const& child : part.children()) {
				work_list.push_back(*child);
			}
		}

		_bounds = ScreenSpace::Box{ScreenSpace::Point{x_min, y_min}, ScreenSpace::Vector{x_max - x_min + 1, y_max - y_min + 1}};
		_offset_to_center = ScreenSpace::Vector{-x_min, -y_min};
	}

	BodyPart* Body::find_part(Id<BodyPart> id)
	{
		for (BodyPart& part : _parts) {
			if (part.id == id) {
				return &part;
			}
		}
		return nullptr;
	}

	Head* Body::find_head(Id<BodyPart> id)
	{
		for (Head& head : _heads) {
			if (head.id == id) {
				return &head;
			}
		}
		return nullptr;
	}

	Torso* Body::find_torso(Id<BodyPart> id)
	{
		for (Torso& torso : _torsos) {
			if (torso.id == id) {
				return &torso;
			}
		}
		return nullptr;
	}

	Arm* Body::find_arm(Id<BodyPart> id)
	{
		for (Arm& arm : _arms) {
			if (arm.id == id) {
				return &arm;
			}
		}
		return nullptr;
	}

	Hand* Body::find_hand(Id<BodyPart> id)
	{
		for (Hand& hand : _hands) {
			if (hand.id == id) {
				return &hand;
			}
		}
		return nullptr;
	}

	Leg* Body::find_leg(Id<BodyPart> id)
	{
		for (Leg& leg : _legs) {
			if (leg.id == id) {
				return &leg;
			}
		}
		return nullptr;
	}

	Foot* Body::find_foot(Id<BodyPart> id)
	{
		for (Foot& foot : _feet) {
			if (foot.id == id) {
				return &foot;
			}
		}
		return nullptr;
	}

	Wing* Body::find_wing(Id<BodyPart> id)
	{
		for (Wing& wing : _wings) {
			if (wing.id == id) {
				return &wing;
			}
		}
		return nullptr;
	}

	Tail* Body::find_tail(Id<BodyPart> id)
	{
		for (Tail& tail : _tails) {
			if (tail.id == id) {
				return &tail;
			}
		}
		return nullptr;
	}

	void Body::PartAttacher::visit(Head& head)
	{
		_body._heads.push_back(head);
	}
	void Body::PartAttacher::visit(Torso& torso)
	{
		_body._torsos.push_back(torso);
	}
	void Body::PartAttacher::visit(Arm& arm)
	{
		_body._arms.push_back(arm);
	}
	void Body::PartAttacher::visit(Hand& hand)
	{
		_body._hands.push_back(hand);
	}
	void Body::PartAttacher::visit(Leg& leg)
	{
		_body._legs.push_back(leg);
	}
	void Body::PartAttacher::visit(Foot& foot)
	{
		_body._feet.push_back(foot);
	}
	void Body::PartAttacher::visit(Wing& wing)
	{
		_body._wings.push_back(wing);
	}
	void Body::PartAttacher::visit(Tail& tail)
	{
		_body._tails.push_back(tail);
	}
}
