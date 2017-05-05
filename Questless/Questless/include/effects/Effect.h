//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "EffectVisitor.h"
#include "entities/beings/Damage.h"
#include "utility/Id.h"

namespace questless
{
	class Being;

	//! An in-game effect perceivable by beings, such as light and sound stimuli.
	class Effect : public Element<EffectSubtypeList>
	{
	public:
		//! @param origin The coordinates of the effect's origin.
		Effect(RegionTileCoords origin) : _origin{origin} {}

		virtual ~Effect() = default;

		//! The maximum distance from the origin at which this effect may be perceived.
		virtual int range() const = 0;

		RegionTileCoords origin() const { return _origin; }
	private:
		RegionTileCoords _origin;
	};

	DEFINE_ELEMENT_BASE(Effect, Effect)

	//! @todo Eventually move effect subtypes to individual header files.

	class EagleEyeEffect : public EffectBase<EagleEyeEffect>
	{
	public:
		Id<Being> const caster_id;

		//! @param origin The coordinates of the effect's origin.
		//! @param caster_id The ID of the caster.
		EagleEyeEffect(RegionTileCoords origin, Id<Being> caster_id)
			: EffectBase<EagleEyeEffect>{origin}, caster_id{caster_id}
		{}

		int range() const final { return 3; }
	};

	class InjuryEffect : public EffectBase<InjuryEffect>
	{
	public:
		Damage const damage;
		Id<Being> target_id;
		std::optional<Id<Being>> opt_source_id;

		//! @param origin The coordinates of the effect's origin.
		//! @param damage The damage dealt.
		//! @param target_id The ID of the injured being.
		//! @param opt_source_id The ID of the being who caused the injury or nullopt if there is none.
		InjuryEffect(RegionTileCoords origin, Damage damage, Id<Being> target_id, std::optional<Id<Being>> opt_source_id)
			: EffectBase<InjuryEffect>{origin}, damage{damage}, target_id{target_id}, opt_source_id{opt_source_id}
		{}

		int range() const final { return 7; }
	};

	class LightningBoltEffect : public EffectBase<LightningBoltEffect>
	{
	public:
		using EffectBase<LightningBoltEffect>::EffectBase;

		int range() const final { return 10; }
	};
}
