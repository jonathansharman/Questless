/**
* @file    AnimationCollection.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation of the AnimationCollection class.
*/

#include "animation/AnimationCollection.h"

using namespace sdl;

namespace questless
{
	AnimationCollection::AnimationCollection(std::string sprite_sheet_name, int cel_columns, int cel_rows)
		: _sprite_sheet_name{sprite_sheet_name}, _current_animation{nullptr}, _paused{false}
	{
		_cel_width = texture_manager()[_sprite_sheet_name].width() / cel_columns;
		_cel_height = texture_manager()[_sprite_sheet_name].height() / cel_rows;
	}

	void AnimationCollection::add(const string& animation_name, Animation::ptr animation)
	{
		_animations[animation_name] = std::move(animation);
	}
	
	void AnimationCollection::start(const string& animation_name, bool randomize_starting_time)
	{
		_current_animation = _animations[animation_name].get();
		if (_current_animation != nullptr) {
			_current_animation->reset(randomize_starting_time);
		}
	}

	void AnimationCollection::update()
	{
		if (!_paused && _current_animation != nullptr) {
			_current_animation->update();
		}
	}

	void AnimationCollection::draw(Point origin) const
	{
		if (_current_animation != nullptr) {
			const Animation::Frame& frame = _current_animation->current_frame();
			Rect dst_rect(origin.x - frame.origin.x, origin.y - frame.origin.y, _cel_width, _cel_height);
			Rect src_rect(_cel_width * frame.coords.x, _cel_height * frame.coords.y, _cel_width, _cel_height);
			texture_manager()[_sprite_sheet_name].draw(dst_rect, src_rect);
		}
	}

	void AnimationCollection::draw(Point origin, const Camera& camera) const
	{
		if (_current_animation != nullptr) {
			const Animation::Frame& frame = _current_animation->current_frame();
			Rect src_rect(_cel_width * frame.coords.x, _cel_height * frame.coords.y, _cel_width, _cel_height);
			camera.draw(texture_manager()[_sprite_sheet_name], origin - Vector{frame.origin.x, frame.origin.y}, nullopt, 1.0, 1.0, false, false, 0.0, Color::white(), src_rect);
		}
	}
}