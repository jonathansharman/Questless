//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/nonnegative.hpp"

#include "meta/quantity.hpp"

#include <nlohmann/json.hpp>

namespace ql {
	namespace dmg {
		using pad = meta::quantity<double, meta::unit_t<struct pad_tag>>;
		using deflect = meta::quantity<double, meta::unit_t<struct deflect_tag>>;
		using fireproof = meta::quantity<double, meta::unit_t<struct fireproof_tag>>;
		using frostproof = meta::quantity<double, meta::unit_t<struct frostproof_tag>>;
		using fortify = meta::quantity<double, meta::unit_t<struct fortify_tag>>;
		using immunize = meta::quantity<double, meta::unit_t<struct immunize_tag>>;
		using insulate = meta::quantity<double, meta::unit_t<struct _tag>>;

		//! A fixed reduction to damage, by type.
		struct protect {
			nonnegative<dmg::pad> pad = dmg::pad{0.0};
			nonnegative<dmg::deflect> deflect = dmg::deflect{0.0};
			nonnegative<dmg::fireproof> fireproof = dmg::fireproof{0.0};
			nonnegative<dmg::frostproof> frostproof = dmg::frostproof{0.0};
			nonnegative<dmg::fortify> fortify = dmg::fortify{0.0};
			nonnegative<dmg::immunize> immunize = dmg::immunize{0.0};
			nonnegative<dmg::insulate> insulate = dmg::insulate{0.0};

			constexpr protect() = default;

			constexpr protect(protect&&) = default;
			constexpr protect(protect const&) = default;

			constexpr protect& operator =(protect&&) = default;
			constexpr protect& operator =(protect const&) = default;

			constexpr protect(dmg::pad pad) : pad{pad} {}
			constexpr protect(dmg::deflect deflect) : deflect{deflect} {}
			constexpr protect(dmg::fireproof fireproof) : fireproof{fireproof} {}
			constexpr protect(dmg::frostproof frostproof) : frostproof{frostproof} {}
			constexpr protect(dmg::fortify fortify) : fortify{fortify} {}
			constexpr protect(dmg::immunize immunize) : immunize{immunize} {}
			constexpr protect(dmg::insulate insulate) : insulate{insulate} {}

			static constexpr protect zero() { return protect{}; }

			protect& operator +=(protect const& p) {
				pad += p.pad;
				deflect += p.deflect;
				fireproof += p.fireproof;
				frostproof += p.frostproof;
				fortify += p.fortify;
				immunize += p.immunize;
				insulate += p.insulate;
				return *this;
			}
			protect& operator -=(protect const& p) {
				pad -= p.pad;
				deflect -= p.deflect;
				fireproof -= p.fireproof;
				frostproof -= p.frostproof;
				fortify -= p.fortify;
				immunize -= p.immunize;
				insulate -= p.insulate;
				return *this;
			}
			protect& operator *=(double k) {
				pad *= k;
				deflect *= k;
				fireproof *= k;
				frostproof *= k;
				fortify *= k;
				immunize *= k;
				insulate *= k;
				return *this;
			}
			protect& operator /=(double k) {
				pad /= k;
				deflect /= k;
				fireproof /= k;
				frostproof /= k;
				fortify /= k;
				immunize /= k;
				insulate /= k;
				return *this;
			}
		
			friend protect operator +(protect const& p1, protect const& p2) {
				protect result = p1;
				result += p2;
				return result;
			}
			friend protect operator -(protect const& p1, protect const& p2) {
				protect result = p1;
				result -= p2;
				return result;
			}
			friend protect operator *(protect const& p, double k) {
				protect result = p;
				result *= k;
				return result;
			}
			friend protect operator *(double k, protect const& p) {
				// Multiplication of double is commutative.
				return p * k;
			}
			friend protect operator /(protect const& p, double k) {
				protect result = p;
				result /= k;
				return result;
			}
		};

		void to_json(nlohmann::json& j, protect const& protect) {
			if (protect.pad != pad{0.0}) j["pad"] = protect.pad.value().value;
			if (protect.deflect != deflect{0.0}) j["deflect"] = protect.deflect.value().value;
			if (protect.fireproof != fireproof{0.0}) j["fireproof"] = protect.fireproof.value().value;
			if (protect.frostproof != frostproof{0.0}) j["frostproof"] = protect.frostproof.value().value;
			if (protect.fortify != fortify{0.0}) j["fortify"] = protect.fortify.value().value;
			if (protect.immunize != immunize{0.0}) j["immunize"] = protect.immunize.value().value;
			if (protect.insulate != insulate{0.0}) j["insulate"] = protect.insulate.value().value;
		}

		void from_json(nlohmann::json const& j, protect& protect) {
			auto pad = j.find("pad");
			if (pad != j.end()) protect.pad = dmg::pad{pad.value().get<double>()};

			auto deflect = j.find("deflect");
			if (deflect != j.end()) protect.deflect = dmg::deflect{deflect.value().get<double>()};

			auto fireproof = j.find("fireproof");
			if (fireproof != j.end()) protect.fireproof = dmg::fireproof{fireproof.value().get<double>()};

			auto frostproof = j.find("frostproof");
			if (frostproof != j.end()) protect.frostproof = dmg::frostproof{frostproof.value().get<double>()};

			auto fortify = j.find("fortify");
			if (fortify != j.end()) protect.fortify = dmg::fortify{fortify.value().get<double>()};

			auto immunize = j.find("immunize");
			if (immunize != j.end()) protect.immunize = dmg::immunize{immunize.value().get<double>()};

			auto insulate = j.find("insulate");
			if (insulate != j.end()) protect.insulate = dmg::insulate{insulate.value().get<double>()};
		}
	}

	constexpr dmg::pad operator "" _pad(long double value) { return dmg::pad{static_cast<double>(value)}; }
	constexpr dmg::deflect operator "" _deflect(long double value) { return dmg::deflect{static_cast<double>(value)}; }
	constexpr dmg::fireproof operator "" _fireproof(long double value) { return dmg::fireproof{static_cast<double>(value)}; }
	constexpr dmg::frostproof operator "" _frostproof(long double value) { return dmg::frostproof{static_cast<double>(value)}; }
	constexpr dmg::fortify operator "" _fortify(long double value) { return dmg::fortify{static_cast<double>(value)}; }
	constexpr dmg::immunize operator "" _immunize(long double value) { return dmg::immunize{static_cast<double>(value)}; }
	constexpr dmg::insulate operator "" _insulate(long double value) { return dmg::insulate{static_cast<double>(value)}; }
}
