//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "tile_widget.hpp"

#include "animation/still_image.hpp"
#include "rsrc/tile.hpp"
#include "utility/unreachable.hpp"
#include "world/terrain.hpp"

namespace ql {
	tile_widget::tile_widget(rsrc::tile const& resources, world_view::tile_view tile_view)
		: _rsrc{resources}
		, _tv{tile_view} //
	{
		switch (reg.get<terrain>(_tv.id)) {
			case terrain::dirt:
				_ani = umake<still_image>(_rsrc.txtr.dirt);
				break;
			case terrain::edge:
				_ani = nullptr;
				break;
			case terrain::grass:
				_ani = umake<still_image>(_rsrc.txtr.grass);
				break;
			case terrain::sand:
				_ani = umake<still_image>(_rsrc.txtr.sand);
				break;
			case terrain::snow:
				_ani = umake<still_image>(_rsrc.txtr.snow);
				break;
			case terrain::stone:
				_ani = umake<still_image>(_rsrc.txtr.stone);
				break;
			case terrain::water:
				_ani = umake<still_image>(_rsrc.txtr.water);
				break;
			default:
				UNREACHABLE;
		}
	}

	auto tile_widget::get_size() const -> view::vector {
		//! @todo What is the most useful size to report?
		return {};
	}

	auto tile_widget::update(sec elapsed_time) -> void {
		_ani->update(elapsed_time);
	}

	auto tile_widget::set_position(view::point position) -> void {
		_position = position;
		_ani->setPosition(to_sfml(_position));
	}

	auto tile_widget::get_position() const -> view::point {
		return _position;
	}

	auto tile_widget::draw(sf::RenderTarget& target, sf::RenderStates states) const -> void {
		//! @todo Use a shader to indicate perception.
		target.draw(*_ani, states);
	}
}
