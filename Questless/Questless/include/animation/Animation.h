/**
* @file    Animation.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Animation class, which handles basic 2D animation.
*/

#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
using std::vector;
#include <string>
using std::string;
#include <chrono>
using namespace std::chrono;
#include <exception>

#include "utility/constants.h"
#include "utility/optional.h"
#include "sdl-wrappers/basic-sdl-wrappers.h"
using sdl::Point;

namespace questless
{
	class Animation
	{
	public:
		struct Frame
		{
			duration<double> duration;
			Point origin; ///< The origin of the frame's texture relative to the animation origin.
			Point coords; ///< The cel coordinates within the sprite sheet.

			Frame(double_seconds duration, Point coords, Point origin) : duration{duration}, coords{coords}, origin{origin} {}
		};

		/// Constructs an animation object from the provided frames using the provided loop type.
		/// @param frames The sequence of frames that compose the animation.
		/// @param looping Whether to loop the animation or play just once.
		Animation(vector<Frame> frames, bool looping);

		/// @return Whether the animation is looping.
		bool looping() const { return _looping; }

		/// @param looping Whether to loop the animation or play just once.
		void looping(bool looping) { _looping = looping; }

		/// @return The number of times the animation has looped.
		int loops() const { return _loops; }

		/// @return The current frame of the animation.
		const Frame& current_frame() const { return _frames[_frame_index]; }

		/// @return The time scale.
		double time_scale() const { return _time_scale; }

		/// @param time_scale A non-negative double to which the time scale is to be set. If time_scale is negative, the time scale is set to 0.
		void time_scale(double time_scale) { _time_scale = time_scale < 0 ? 0 : time_scale; }

		/// @return Whether the animation has played to its end.
		bool over() const { return _over; }

		/// @return The total duration of the animation.
		double_seconds duration() const;

		/// Moves to the start or a random point in the animation, sets the loop counter to zero, and sets the over flag to false.
		/// @param randomize_starting_time If true, resets the animation to a random point.
		void reset(bool randomize_starting_time = false);

		/// Advances the animation. Should be called once per game update.
		void update();
	private:
		vector<Frame> _frames;
		int _frame_index;
		double_seconds _accrued_time;
		bool _looping;
		unsigned _loops;
		bool _in_reverse;
		double _time_scale;
		bool _over;
	};
}

#endif