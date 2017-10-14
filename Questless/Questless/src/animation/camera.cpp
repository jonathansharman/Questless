//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/camera.h"

#include <cmath>

#include "sdl/resources.h"

using namespace sdl;
using namespace units;

namespace ql
{
	void camera::zoom_factor(double factor)
	{
		if (factor >= 0.0) {
			_zoom *= factor;
		}
	}

	game_space::radians camera::positive_angle() const
	{
		return _angle < game_space::radians::zero() ? _angle + game_space::radians::circle() : _angle;
	}

	void camera::angle(game_space::radians theta)
	{
		_angle.count() = fmod(theta.count(), game_space::radians::circle().count() / 2.0);
	}

	void camera::rotate(game_space::radians dtheta)
	{
		_angle.count() = fmod((_angle + dtheta).count(), game_space::radians::circle().count() / 2.0);
	}

	void camera::update()
	{
		screen_space::vector center_to_mouse = the_input().mouse_position() - the_window().center();
		game_space::vector scaled_center_to_mouse = game_space::vector{static_cast<double>(center_to_mouse.x()), static_cast<double>(-center_to_mouse.y())} / _zoom;
		_point_hovered = _position + scaled_center_to_mouse;
		_point_hovered.rotate(_position, _angle);
		_tile_hovered = layout::dflt().to_hex_coords<region_tile::point>(_point_hovered);
	}

	void camera::draw
		( texture const& texture
		, game_space::point position
		, origin origin
		, colors::color_factor draw_color_factor
		, h_scale horizontal_scale
		, v_scale vertical_scale
		, game_space::radians angle
		, src_rect const& src_rect
		) const
	{
		texture.draw_transformed
			( screen_point(position)
			, origin
			, draw_color_factor * color_factor
			, static_cast<float>(_zoom * horizontal_scale)
			, static_cast<float>(_zoom * vertical_scale)
			, angle - _angle
			, src_rect
			);
	}

	void camera::draw_lines(std::vector<game_space::point> points, colors::color color) const
	{
		// Transform segment end points.
		std::vector<screen_space::point> screen_points;
		for (game_space::point const& point : points) {
			screen_points.push_back(screen_point(point));
		}

		// Draw transformed line segments using the renderer.
		the_renderer().draw_lines(screen_points, color);
	}

	screen_space::point camera::screen_point(game_space::point point) const
	{
		game_space::point const window_center = game_space::point{0.0, 0.0} + game_space::vector{static_cast<double>(the_window().width()), static_cast<double>(the_window().height())} / 2.0;
		game_space::vector const camera_to_point = point - _position;
		game_space::point const scaled_point = _zoom * game_space::vector{camera_to_point.x(), -camera_to_point.y()} + window_center;
		game_space::point const rotated_scaled_point = scaled_point.rotated(window_center, _angle);
		return screen_space::point{lround(rotated_scaled_point.x()), lround(rotated_scaled_point.y())};
	}
}
