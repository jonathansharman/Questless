//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#include "section.hpp"

#include "tile.hpp"

#include "entities/beings/being.hpp"
#include "magic/spell.hpp"
#include "utility/random.hpp"
#include "world/light_source.hpp"
#include "world/region.hpp"

namespace ql {
	section::section(reg& reg, id region_id, section_hex_point coords) : _reg{&reg}, _coords{coords} {
		// Create a section with random tiles.
		auto const center = center_coords();
		for (pace q = -section_radius; q <= section_radius; ++q) {
			for (pace r = -section_radius; r <= section_radius; ++r) {
				auto const terrain = static_cast<ql::terrain>(uniform(0, static_cast<int>(terrain::terrain_count) - 1));
				auto const tile_coords = center + tile_hex_vector{q, r};
				location location{region_id, tile_coords};
				id const tile_id = reg.create();
				make_tile(reg, tile_id, terrain, location, 0_temp, 0_lum);

				// Add the tile's ID to the tile ID array.
				auto [i, j] = indices(tile_coords);
				_tile_ids[i][j] = tile_id;
			}
		}
	}

	auto section::section_coords() const {
		return _coords;
	}

	auto section::center_coords() const -> tile_hex_point {
		return tile_hex_point{_coords.q.data * section_diameter, _coords.r.data * section_diameter};
	}

	auto section::entity_id_map() const -> std::unordered_map<tile_hex_point, id> const& {
		return _entity_id_map;
	}

	auto section::entity_id_at(tile_hex_point tile_coords) const -> std::optional<id> {
		auto it = _entity_id_map.find(tile_coords);
		return it != _entity_id_map.end() ? std::make_optional(it->second) : std::nullopt;
	}

	auto section::try_add(id entity_id) -> bool {
		auto result = _entity_id_map.insert({_reg->get<location>(entity_id).coords, entity_id});
		return result.second;
	}

	auto section::remove_at(tile_hex_point coords) -> void {
		auto it = _entity_id_map.find(coords);
		if (it != _entity_id_map.end()) { _entity_id_map.erase(it); }
	}

	auto section::remove(id entity_id) -> void {
		remove_at(_reg->get<location>(entity_id).coords);
	}

	auto section::tile_id_at(tile_hex_point coords) const -> id {
		auto [i, j] = indices(coords);
		return _tile_ids[i][j];
	}

	auto section::indices(tile_hex_point coords) const -> std::tuple<size_t, size_t> {
		auto const offset = center_coords() - coords + tile_hex_vector{section_radius, section_radius};
		return std::make_tuple(static_cast<size_t>(offset.q.data), static_cast<size_t>(offset.r.data));
	}
}
