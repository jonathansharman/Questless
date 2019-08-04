//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "widget.hpp"

#include "utility/reference.hpp"

namespace ql {
	struct animation;

	//! Allows interaction with an entity in the world.
	struct entity_widget : widget {
		auto get_size() const -> view::vector final;

		auto update(sec elapsed_time) -> void final;

		auto set_position(view::point position) -> void final;

		auto get_position() const -> view::point final;

	private:
		uptr<animation> _ani;

		auto draw(sf::RenderTarget& target, sf::RenderStates states) const -> void final;
	};
}
