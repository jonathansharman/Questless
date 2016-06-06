/**
* @file    WorldRenderer.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the WorldRenderer class, which draws the elements of the world that the player can see.
*/

#ifndef WORLD_RENDERER_H
#define WORLD_RENDERER_H

#include <memory>
using std::shared_ptr;

#include "Camera.h"
#include "AnimationCollection.h"
#include "entities/Entity.h"
#include "entities/beings/WorldView.h"
#include "sdl-wrappers/Renderer.h"
#include "world/Tile.h"
#include "sdl-wrappers/Renderable.h"
using sdl::Renderable;

namespace questless
{
	class WorldRenderer : public Renderable
	{
	public:
		/// @param world_view The world view to render.
		WorldRenderer(const WorldView& world_view) : _world_view{world_view}, _terrain_render_is_current{false} {}

		void update();
		void draw_terrain(const Camera& camera);
		void draw_beings(const Camera& camera);
	private:
		const WorldView& _world_view;

		std::map<Tile::Type, sdl::Texture::ptr> _tile_textures;
		std::map<Entity::id_t, std::unique_ptr<AnimationCollection>> _entity_animations;
		sdl::Texture::ptr _terrain_texture;
		sdl::Rect _terrain_bounds;
		bool _terrain_render_is_current;

		void refresh() override;

		sdl::Texture& create_and_cache_tile_texture(const Tile& tile);
		AnimationCollection& create_and_cache_entity_animation(Entity& entity);
		void render_terrain();
	};
}

#endif