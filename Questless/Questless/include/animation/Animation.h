//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation/Camera.h"
#include "units/GameSeconds.h"
#include "units/ScreenSpace.h"
#include "utility/Bounded.h"

namespace questless
{
	//! Abstract base for animations.
	class Animation
	{
	public:
		static constexpr double minimum_timescale = 0.0;

		//! The time scale of the animation. E.g., a time scale of 2.0 plays the animation at double speed.
		Bounded<double, minimum_timescale> time_scale = 1.0;

		virtual ~Animation() = default;

		//! Whether the current animation is paused.
		bool paused() const { return _paused; }

		//! Pauses the current animation.
		void pause() { _paused = true; }

		//! Resumes the current animation.
		void resume() { _paused = false; }

		//! Whether the animation has played to its end.
		bool over() const { return _over; }

		//! Updates the current animation.
		void update()
		{
			if (!_over && !_paused) {
				animation_subupdate();
			}
		}

		//! Draws the animation at @p position in screen space.
		virtual void draw(units::ScreenSpace::Point position) const = 0;

		//! Draws the animation at @p position in game space using @p camera.
		//! @param position The position in game space at which to draw the animation.
		//! @param camera The camera with which to draw the animation.
		//! @param color_factor An additional color factor, applied on top of the camera's color factor.
		virtual void draw(units::GameSpace::Point position, Camera const& camera, units::colors::ColorFactor color_factor = units::colors::white_factor()) const = 0;
	protected:
		bool _over = false;

		//! The logical amount of time elapsed since last frame, accounting for the time scale.
		units::GameSeconds elapsed_time() const;
	private:
		bool _paused = false;

		//! Animation subtype-specific update.
		virtual void animation_subupdate() = 0;
	};
}
