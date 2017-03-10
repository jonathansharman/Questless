/**
* @file    Goblin.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Goblin class.
*/

#pragma once

#include "entities/beings/CorporealBeing.h"

namespace questless
{
	class Goblin : public CorporealBeing
	{
	public:
		static constexpr Stats dflt_base_stats
			{ Vitality{100.0}
			, Spirit{0.0}
			, HealthRegen{0.1}
			, ManaRegen{1.0}
			, Strength{0.0}
			, Endurance{100.0}
			, Stamina{100.0}
			, Agility{0.0}
			, Dexterity{0.0}
			, Stealth{300.0}
			, Vision
				{ Acuity{60.0}
				, IdealLight{100.0}
				, LightTolerance{10.0}
				}
			, Hearing{180.0}
			, Intellect{0.0}
			, Weight{0.0}
			, MinTemp{-100.0}
			, MaxTemp{100.0}
			, Mute{true}
			, Protection::zero()
			, Resistance::zero()
			, Vulnerability::zero()
			, Magic
				{ WhiteMagic{100.0}
				, BlackMagic{100.0}
				, GreenMagic{100.0}
				, RedMagic{100.0}
				, BlueMagic{100.0}
				, YellowMagic{100.0}
				}
			, Antimagic
				{ WhiteMagic{100.0}
				, BlackMagic{100.0}
				, GreenMagic{100.0}
				, RedMagic{100.0}
				, BlueMagic{100.0}
				, YellowMagic{100.0}
				}
			};

		static constexpr Vitality head_vitality{25.0};
		static constexpr Weight head_weight{5.0};
		static constexpr Intellect head_intellect{60.0};
		static constexpr Spirit head_spirit{20.0};
		static constexpr Mute head_mute{false};

		static constexpr Vitality torso_vitality{50.0};
		static constexpr Weight torso_weight{35.0};
		static constexpr Strength torso_strength{20.0};

		static constexpr Vitality arm_vitality{25.0};
		static constexpr Weight arm_weight{10.0};
		static constexpr Strength arm_strength{20.0};

		static constexpr Vitality hand_vitality{15.0};
		static constexpr Weight hand_weight{5.0};
		static constexpr Dexterity hand_dexterity{100.0};

		static constexpr Vitality leg_vitality{25.0};
		static constexpr Weight leg_weight{10.0};
		static constexpr Agility leg_agility{40.0};
		static constexpr Strength leg_strength{20.0};

		static constexpr Vitality foot_vitality{15.0};
		static constexpr Weight foot_weight{5.0};
		static constexpr Agility foot_agility{20.0};

		Goblin(Game& game, const std::function<std::unique_ptr<Agent>(Being&)>& make_agent, Id<Being> id);
		Goblin(Game& game, std::istream& in);

		void accept(EntityVisitor& visitor) override { return visitor.visit(*this); }
		void accept(EntityVisitor& visitor) const override { return visitor.visit(*this); }

		EntityClass entity_class() const override { return EntityClass::GoblinClass; }

		void serialize(std::ostream& out) const override;

		std::string const& description() const { return "The goblins are a diminutive and simpleminded humanoid race, known "
			"particularly for their bad tempers and love of mischief. Goblins rarely form large communities, preferring small "
			"family groups or solitude. They are remarkably sneaky, and many an unwary traveller through goblin territory has "
			"lost a coin purse or a limb to a goblin ambush."; }
	protected:
		Body make_body() override;
	};
}
