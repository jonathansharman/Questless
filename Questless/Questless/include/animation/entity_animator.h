//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation.h"
#include "entities/entity_visitor.h"
#include "sdl/resources.h"
#include "utility/reference.h"

namespace ql
{
	//! Creates animations for entities.
	class entity_animator : public entity_const_visitor
	{
	public:
		void visit(human const&) final;
		void visit(goblin const&) final;

		void visit(campfire const&) final;
		void visit(corpse const&) final;
		void visit(item_box const&) final;

		//! The animation produced by the last visit. Moves the animation out of the animator.
		uptr<animation> animation() { return std::move(_animation); }
	private:
		uptr<ql::animation> _animation;
	};
}
