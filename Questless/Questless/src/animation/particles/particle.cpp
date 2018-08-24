//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/particles/particle.hpp"

#include "animation/camera.hpp"
#include "game.hpp"
#include "utility/random.hpp"

using namespace sdl;
using namespace units;

namespace ql {
	particle::particle
		( game_space::vector displacement
		, game_space::velocity velocity
		, game_space::acceleration acceleration
		, game_space::radians angle
		, game_space::radians_per_sec angular_velocity
		, scale scale
		, game_space::scale_velocity scale_velocity
		, lifetime lifetime
		, max_displacement max_displacement
		)
		: _displacement{displacement + random_displacement(max_displacement)}
		, _velocity{std::move(velocity)}
		, _acceleration{std::move(acceleration)}
		, _angle{std::move(angle)}
		, _angular_velocity{std::move(angular_velocity)}
		, _scale{scale}
		, _scale_velocity{scale_velocity}
		, _lifetime{lifetime}
		, _time_left{lifetime}
		, _color_vector{colors::white_vector()}
	{}
	void particle::animation_subupdate() {
		auto elapsed_time = animation::elapsed_time();

		_time_left -= elapsed_time;
		if (_time_left < units::game_space::seconds::zero()) {
			_over = true;
			return;
		}

		_displacement += _velocity * elapsed_time;
		_velocity += _acceleration * elapsed_time;
		if (face_towards_heading() && _velocity != game_space::velocity::zero()) {
			_angle = _velocity.step().angle();
		} else {
			_angle += _angular_velocity * elapsed_time;
		}
		_scale += _scale_velocity * elapsed_time;

		particle_subupdate();
	}

	void particle::draw(units::window_space::point position) const {
		texture().draw(position, texture_space::align_center, texture_space::align_middle);
	}

	void particle::draw(units::game_space::point position, camera const& camera, colors::color_vector color_vector) const {
		// Determine color vector to use.
		color_vector = ignore_color_mod() ? _color_vector : color_vector * _color_vector;
		if (fade_out()) {
			color_vector[3] *= static_cast<float>(_time_left.count() / _lifetime.count());
		}

		camera.draw
			( texture()
			, position + _displacement
			, origin{texture_space::vector::zero()}
			, color_vector
			, view_space::vector
				{ static_cast<view_space::scalar>(_scale)
				, static_cast<view_space::scalar>(_scale)
				}
			, _angle
			);
	}
}