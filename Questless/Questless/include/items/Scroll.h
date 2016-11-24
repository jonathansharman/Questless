/**
* @file    Scroll.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Scroll class.
*/

#ifndef SCROLL_H
#define SCROLL_H

#include <memory>

#include "Item.h"
#include "spell/Spell.h"

namespace questless
{
	class Scroll : public Item
	{
	public:
		Scroll(spell::Spell::ptr spell) : _spell{std::move(spell)} {}

		void accept(ItemVisitor& visitor) const override { return visitor.visit(*this); }

		std::string name() const override { return blank() ? "Blank Scroll" : "Scroll"; }

		double weight() const override { return 0.1; }

		bool blank() const { return _spell == nullptr; }

		spell::Spell& spell() { return *_spell; }
		const spell::Spell& spell() const { return *_spell; }

		std::vector<Action::ptr> actions() override
		{
			std::vector<Action::ptr> actions;
			if (!blank()) {
				if (_spell->charges() > 0) {
					actions.push_back(Cast::make(*this));
				}
				if (auto max_charges = _spell->max_charges()) {
					if (_spell->charges() < *max_charges) {
						actions.push_back(Incant::make(*this));
					}
				}
				if (_spell->charges() > 0) {
					actions.push_back(Discharge::make(*this));
				}
			}
			actions.push_back(Drop::make(*this));
			actions.push_back(Throw::make(*this));
			return actions;
		}

		void update() override { _spell->update(); }

		/// @todo COOLDOWNS. Spell will need to be modified as well.
	private:
		class Cast : public Action
		{
		public:
			Cast(Scroll& scroll) : _scroll{scroll} {}

			static ptr make(Scroll& item) { return std::make_unique<Cast>(item); }

			std::string name() const override { return "Cast"; }

			Action::Complete perform(Being& actor, cont_t cont) override
			{
				return _scroll.spell().perform(actor, std::move(cont));
			}
		private:
			Scroll& _scroll;
		};

		class Incant : public Action
		{
		public:
			Incant(Scroll& scroll) : _scroll{scroll} {}

			static ptr make(Scroll& item) { return std::make_unique<Incant>(item); }

			std::string name() const override { return "Incant"; }

			Action::Complete perform(Being& /*actor*/, cont_t cont) override
			{
				/// @todo Costs, times, etc.
				_scroll.spell().charge();
				return cont(Result::success);
			}
		private:
			Scroll& _scroll;
		};

		class Discharge : public Action
		{
		public:
			Discharge(Scroll& scroll) : _scroll{scroll} {}

			static ptr make(Scroll& item) { return std::make_unique<Discharge>(item); }

			std::string name() const override { return "Discharge"; }

			Action::Complete perform(Being& /*actor*/, cont_t cont) override
			{
				/// @todo Costs, times, etc.
				_scroll.spell().discharge();
				return cont(Result::success);
			}
		private:
			Scroll& _scroll;
		};

		spell::Spell::ptr _spell;
	};
}

#endif
