/**
* @file    Body.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Body class, which represents a being's body. Holds a BodyPart tree.
*/

#ifndef BODY_H
#define BODY_H

#include <memory>
#include <vector>

#include <boost/iterator/transform_iterator.hpp>

#include "entities/beings/BodyPart.h"
#include "sdl-wrappers/basic-sdl-wrappers.h"

namespace questless
{
	class Body
	{
	public:
		using ptr = std::unique_ptr<Body>;

		/// @param root The root of the body parts tree.
		Body(std::unique_ptr<BodyPart> root);

		Body(Body&&) = default;

		/// @return The root body part, to which all other body parts are attached.
		BodyPart& root() { return *_root; }

		/// @return Iterator to beginning of body parts.
		auto begin() { return _parts.begin(); }
		/// @return Iterator to end of body parts.
		auto end() { return _parts.end(); }

		/// @return Const iterator to beginning of body parts.
		auto begin() const
		{
			return boost::make_transform_iterator
				( _parts.begin()
				, (transform_t)[](BodyPart::ref bp) -> const BodyPart::ref { return bp; }
				);
		}
		/// @return Const iterator to end of body parts.
		auto end() const
		{
			return boost::make_transform_iterator
				( _parts.end()
				, (transform_t)[](BodyPart::ref bp) -> const BodyPart::ref { return bp; }
				);
		}
		
		const std::vector<std::reference_wrapper<Torso>>& torsos() { return _torsos; }
		const std::vector<Head::ref>& heads() { return _heads; }
		const std::vector<Arm::ref>& arms() { return _arms; }
		const std::vector<Hand::ref>& hands() { return _hands; }
		const std::vector<Leg::ref>& legs() { return _legs; }
		const std::vector<Foot::ref>& feet() { return _feet; }
		const std::vector<Wing::ref>& wings() { return _wings; }
		const std::vector<Tail::ref>& tails() { return _tails; }

		/// @return The bounding box around the body's parts.
		sdl::Rect bounds() const { return _bounds; }

		/// @return The offset from the upper left corner of the bounds to the body's center.
		sdl::Vector offset_to_center() const { return _offset_to_center; }
	private:
		using transform_t = const BodyPart::ref(*)(BodyPart::ref);

		class PartAttacher : public BodyPartVisitor
		{
		public:
			PartAttacher(Body& body) : _body{body} {}

			void visit(Torso&) override;
			void visit(Head&) override;
			void visit(Arm&) override;
			void visit(Hand&) override;
			void visit(Leg&) override;
			void visit(Foot&) override;
			void visit(Wing&) override;
			void visit(Tail&) override;
		private:
			Body& _body;
		};
		friend PartAttacher;

		std::unique_ptr<BodyPart> _root;

		sdl::Rect _bounds;
		sdl::Vector _offset_to_center;

		std::vector<BodyPart::ref> _parts;

		std::vector<Torso::ref> _torsos;
		std::vector<Head::ref> _heads;
		std::vector<Arm::ref> _arms;
		std::vector<Hand::ref> _hands;
		std::vector<Leg::ref> _legs;
		std::vector<Foot::ref> _feet;
		std::vector<Wing::ref> _wings;
		std::vector<Tail::ref> _tails;
	};
}

#endif
