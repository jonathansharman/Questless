//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "sdl/Texture.h"
#include "sdl/Window.h"
#include "sdl/Input.h"
#include "sdl/Color.h"
#include "world/coordinates.h"
#include "utility/utility.h"
#include "utility/TaggedType.h"

namespace questless
{
	struct Origin : TaggedType<std::optional<units::GamePoint>> { using TaggedType::TaggedType; };
	struct HScale : TaggedType<double> { using TaggedType::TaggedType; };
	struct VScale : TaggedType<double> { using TaggedType::TaggedType; };
	struct HFlip : TaggedType<bool> { using TaggedType::TaggedType; };
	struct VFlip : TaggedType<bool> { using TaggedType::TaggedType; };
	struct SrcRect : TaggedType<std::optional<units::TextureRect>> { using TaggedType::TaggedType; };

	//! A simple 2D camera, useful for panning, zooming, and rotating around a scene.
	class Camera
	{
	public:
		//! Constructs a camera with the specified starting position.
		//! @param position The starting position of the camera.
		Camera(units::GamePoint position)
			: _position{position}
			, _zoom{1.0}
			, _angle{0}
			, _color{sdl::Color::white()}
		{}

		Camera& operator =(Camera const&) = delete;

		//! The camera's position.
		units::GamePoint position() const { return _position; }

		//! Sets the camera's position.
		//! @param position The new camera position.
		void position(units::GamePoint const& position) { _position = position; }

		//! Pans the camera the specified amount.
		//! @param offset The offset which is added to the camera's position.
		void pan(units::GameVector const& offset) { _position += offset; }

		//! The camera's zoom.
		inline double zoom() const { return _zoom; }

		//! Sets the camera's zoom.
		//! @param zoom A non-negative double for the zoom. If zoom is negative, it is clamped to 0.
		void zoom(double zoom) { _zoom = zoom < 0 ? 0 : zoom; }

		//! Multiplies the camera's zoom by the provided factor.
		//! @param factor A non-negative double by which zoom is multiplied.
		void zoom_factor(double factor);

		//! The camera's counter-clockwise rotation in radians, in the range [-pi, pi].
		units::GameRadians angle() const { return _angle; }

		//! The camera's counter-clockwise rotation in radians, in the range [0, tau).
		units::GameRadians positive_angle() const;

		//! Sets the camera's angle.
		//! @param theta The camera's counter-clockwise rotation in radians, in the range [0, tau).
		void angle(units::GameRadians theta);

		//! Adjusts the camera's angle.
		//! @param dtheta The number of radians to offset the angle.
		void rotate(units::GameRadians dtheta);

		//! The camera's color modulator.
		sdl::Color color() const { return _color; }

		//! Sets the camera's color modulator.
		//! @param color The new color modulator.
		sdl::Color color(sdl::Color color) { _color = color; }

		//! The game point the mouse is hovering over.
		units::GamePoint point_hovered() const { return _point_hovered; }

		//! The hex coordinates of the tile the mouse is hovering over.
		RegionTile::Point tile_hovered() const { return _tile_hovered; }

		//! Updates the camera. To be called once per frame.
		void update();

		//! Draws all or part of the provided texture with respect to the camera.
		//! @param texture The texture to be drawn.
		//! @param position The in-game coordinates of the texture.
		//! @param origin The origin point within the texture. If nullopt, the texture's center is used.
		//! @param color An additional color modulator, applied on top of the camera's and texture's color members.
		//! @param horizontal_scale The horizontal scale of the texture.
		//! @param vertical_scale The vertical scale of the texture.
		//! @param angle The counter-clockwise rotation of the texture, in radians.
		//! @param flip_horizontally Whether to flip the texture horizontally.
		//! @param flip_horizontally Whether to flip the texture vertically.
		//! @param src_rect An optional Rect specifying the portion of the texture to be copied. If nullopt, the entire texture is used.
		void draw
			( sdl::Texture const& texture
			, units::GamePoint position
			, Origin origin = Origin{std::nullopt}
			, sdl::Color color = sdl::Color::white()
			, HScale horizontal_scale = HScale{1.0}
			, VScale vertical_scale = VScale{1.0}
			, units::GameRadians angle = units::GameRadians{0.0}
			, HFlip flip_horizontally = HFlip{false}
			, VFlip flip_vertically = VFlip{false}
			, SrcRect const& src_rect = SrcRect{std::nullopt}
			) const;

		//! Draws lines relative to the camera connecting the series of points contained in the vector.
		//! @param points A vector of game points.
		//! @param color The color of the lines.
		void draw_lines(std::vector<units::GamePoint> points, sdl::Color color) const;
	private:
		units::GamePoint _position;
		double _zoom;
		units::GameRadians _angle;
		sdl::Color _color;

		units::GamePoint _point_hovered;
		RegionTile::Point _tile_hovered;

		//! The given game point transformed to screen space, accounting for the camera.
		units::ScreenPoint screen_point(units::GamePoint point) const;
	};
}
