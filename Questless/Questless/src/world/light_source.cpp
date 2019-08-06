//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "light_source.hpp"

#include "region.hpp"

namespace ql {
	namespace {
		constexpr auto lum_per_span = 10_lum / 1_span;
	}
	span light_source::range() const {
		return luminance / lum_per_span;
	}

	lum light_source::luminance_at(location target_location) const {
		auto const& location = reg.get<ql::location>(id);

		// Only provides light within its own region.
		if (location.region_id != target_location.region_id) { return 0_lum; }

		// Compute light source's luminance at this distance.
		span const distance = (target_location.coords - location.coords).length();
		auto& region = reg.get<ql::region>(id);
		double const occlusion = region.occlusion(target_location.coords, location.coords);
		return std::max(0_lum, luminance - cancel::quantity_cast<lum>(distance * lum_per_span * occlusion));
	}
}
