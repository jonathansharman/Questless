//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "widget.hpp"

#include "reg.hpp"
#include "rsrc/fonts_fwd.hpp"
#include "utility/reference.hpp"

namespace ql {
	struct hud;

	//! The scene for the main menu.
	struct main_menu : widget {
		main_menu(reg& reg, uptr<widget>& root, rsrc::fonts const& fonts);

		~main_menu();

		auto get_size() const -> view::vector final;

		auto update(sec elapsed_time) -> void final;

		auto set_position(view::point position) -> void final;

		auto get_position() const -> view::point final;

		auto on_parent_resize(view::vector parent_size) -> void final;

		auto on_request_quit() -> event_handled final;

	private:
		reg_ptr _reg;

		uptr<widget>& _root;
		uptr<hud> _hud;

		view::point _position;
		view::vector _size;

		auto draw(sf::RenderTarget& target, sf::RenderStates states) const -> void final;
	};
}
