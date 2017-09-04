//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "ui/qte/Shock.h"

#include "Game.h"
#include "units/math.h"
#include "utility/utility.h"

using namespace sdl;
using namespace units;
using namespace units::math;

namespace questless::qte
{
	Shock::Shock(RegionTile::Point target_coords, std::function<void(double)> cont)
		: _target_point{units::Layout::dflt().to_world(target_coords)}
		, _cont{std::move(cont)}
	{
		load_textures();
	}

	Dialog::State Shock::update()
	{
		_elapsed_time += Game::frame_duration;
		if (_elapsed_time > _time_limit) {
			return _cont(_charges.size() / _expected_charges);
		}

		// Acclerate only when the mouse moves to the next quadrant over.
		bool accelerate = false;
		{
			GameSpace::Vector v = game().camera().point_hovered() - _target_point;
			switch (_quadrant) {
				case 0:
					if (v.x() > 0.0 && v.y() > 0.0) {
						_quadrant = 1;
						accelerate = true;
					}
					break;
				case 1:
					if (v.x() < 0.0 && v.y() > 0.0) {
						_quadrant = 2;
						accelerate = true;
					}
					break;
				case 2:
					if (v.x() < 0.0 && v.y() < 0.0) {
						_quadrant = 3;
						accelerate = true;
					}
					break;
				case 3:
					if (v.x() > 0.0 && v.y() < 0.0) {
						_quadrant = 0;
						accelerate = true;
					}
					break;
			}
		}
		if (accelerate) {
			for (int i = 0; i < _charges_per_quadrant; ++i) {
				_charges.push_back(Charge
					{ _target_point + random_displacement(100.0)
					, GameVelocity{GameSpace::Vector{random_angle(), 100.0}}
					});
			}
		}

		for (auto& point_charge : _charges) {
			GameSpace::Vector r = _target_point - point_charge.position; // displacement to target
			double d = r.length(); // distance to target

			if (accelerate) {
				// Accelerate counter-clockwise from the target.
				point_charge.velocity.step() += 7'000.0 * GameSpace::Vector{-r.y(), r.x()} / square(d);
			}

			// Apply drag.
			point_charge.velocity *= 0.99;
			// Apply random acceleration.
			point_charge.velocity.step() += random_displacement(50.0);
			// Apply attractive and repulsive forces.
			point_charge.velocity.step() += 20'000.0 * r / std::max(1.0, square(d));
			point_charge.velocity.step() -= 800'000.0 * r / std::max(1.0, cube(d));
			// Update position.
			point_charge.position += point_charge.velocity * Game::frame_duration;
		}

		return State::open;
	}

	void Shock::draw() const
	{
		//! @todo Refactor point charge to use a particle animation?

		static auto point_charge_texture_handle = texture_manager().add("resources/textures/particles/glow-large.png");

		// Draw point charges.
		for (auto const& point_charge : _charges) {
			float intensity = uniform(0.0f, 1.0f);
			game().camera().draw
				( texture_manager()[point_charge_texture_handle]
				, point_charge.position
				, Origin{TextureSpace::Vector::zero()}
				, colors::ColorFactor{1.0f, 1.0f, intensity, 1.0f}
				, HScale{(1.0 + intensity) / 2.0}
				, VScale{(1.0 + intensity) / 2.0}
				, GameSpace::Radians{0.0}
				);
		}

		draw_title(*_txt_title);
		draw_prompt(*_txt_prompt);
	}

	void Shock::load_textures()
	{
		_txt_title = make_title("Build up a charge!");
		_txt_prompt = make_prompt("Circle the target counter-clockwise as fast as you can!");
	}
}