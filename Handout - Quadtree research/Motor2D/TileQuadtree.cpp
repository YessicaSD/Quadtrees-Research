#include "p2Defs.h"
#include "p2Point.h"
#include "p2Log.h"
#include "SDL/include/SDL.h"
#include <assert.h>

#include "j1App.h"
#include "j1Map.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Textures.h"




TileQuadtree::TileQuadtree(uint max_levels, SDL_Rect section, uint size, uint level): Quadtree(max_levels, section, level), size(size)
{
	//Set the subnodes as nullptr
	for (int i = 0; i < 4; i++)
		nodes[i] = nullptr;

	/*We will only store the tiles in the bottom nodes, 
	so, if this node will have subnodes, it won't need room for tiles*/
	if (level != max_levels)
		tiles =NULL;

	else
	{
		tiles = new TileData[size];

		for (int i = 0; i < size; ++i)
			tiles[i] = TileData(0, { 0,0 });

	}

	tiles_contained = 0;

	assert(level <= max_levels,"MAX LEVELS > LEVELS");
}

void TileQuadtree::CleanUp()
{
	if (divided == true)
	{
		for (int i = 0; i < 4; i++)
		{
			nodes[i]->CleanUp();
			nodes[i] = nullptr;
		}

	}
	RELEASE(tiles);
}

void TileQuadtree::Split()
{
	if (level < max_levels && divided == false)
	{
		nodes[0] = new TileQuadtree(max_levels, { section.x,section.y, section.w / 2, section.h / 2 }, size/4, level + 1);
		nodes[1] = new TileQuadtree(max_levels, { section.x + section.w / 2,section.y, section.w / 2,section.h / 2 }, size/ 4, level + 1);
		nodes[2] = new TileQuadtree(max_levels, { section.x,section.y + section.h / 2 , section.w / 2, section.h / 2 }, size/ 4, level + 1);
		nodes[3] = new TileQuadtree(max_levels, { section.x + section.w / 2,section.y + section.h / 2, section.w / 2,section.h / 2 }, size/4 , level + 1);
		divided = true;
	}

}

void TileQuadtree::InsertTile(TileData tile)
{
	//TODO 1 - If we insert a tile to a node located at the bottom of the tree, add it to the array


	//TODO 2 - If we insert a tile to a node that has more subnodes, insert it to the subnode it belongs to
	
}

bool TileQuadtree::CheckVisibility()
{
	uint screen_w;
	uint screen_h;
	App->win->GetWindowSize(screen_w, screen_h);

	if (-App->render->camera.x > (section.x + section.w) ||
		(-App->render->camera.x + int(screen_w)) < section.x ||
		-App->render->camera.y > (section.y + section.h) ||
		(-App->render->camera.y + int(screen_h)) < section.y)
		return false;

	return true;
}

void TileQuadtree::DrawMap()
{
	//TODO 3 - Let's draw the map
	//If the node is in the screen and is located at the bottom of the tree, draw its tiles


	//If it's not in the bottom of the tree, make its subnodes call the function
}

void TileQuadtree::DrawQuadtree()
{
	//TODO 0 : WARMING UP
	//Haven't worked with recursive functions for a while, so let's start by drawing the nodes

}