#include "Defs.h"
#include "Log.h"
#include "App.h"
#include "PathFinding.h"
#include "Render.h"
#include "Map.h"


Pathfinding::Pathfinding() : Module(), map(NULL), lastPath(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name.create("pathfinding");
}

// Destructor
Pathfinding::~Pathfinding()
{
	RELEASE_ARRAY(map);
}

// Called before quitting
bool Pathfinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	lastPath.Clear();
	RELEASE_ARRAY(map);
	return true;
}

// Sets up the walkability map
void Pathfinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width * height];
	memcpy(map, data, width * height);
}

// Utility: return true if pos is inside the map boundaries
bool Pathfinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
		pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool Pathfinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}

// Utility: return the walkability value of a tile
uchar Pathfinding::GetTileAt(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
		return map[(pos.y * width) + pos.x];

	return INVALID_WALK_CODE;
}


// To request all tiles involved in the last generated path
const DynArray<iPoint>* Pathfinding::GetLastPath() const
{
	return &lastPath;
}

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
const ListItem<PathNode>* PathList::Find(const iPoint& point) const
{
	ListItem<PathNode>* item = list.start;
	while (item)
	{
		if (item->data.pos == point)
			return item;
		item = item->next;
	}
	return NULL;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
ListItem<PathNode>* PathList::GetNodeLowestScore() const
{
	ListItem<PathNode>* ret = NULL;
	int min = 65535;

	ListItem<PathNode>* item = list.end;
	while (item)
	{
		if (item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& list_to_fill, EntityType type) const
{
	iPoint cell;
	uint before = list_to_fill.list.count();

	// south
	cell.create(pos.x, pos.y - 1);
	if (app->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// east
	cell.create(pos.x + 1, pos.y);
	if (app->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y);
	if (app->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// north
	cell.create(pos.x, pos.y + 1);
	if (app->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	if (type == EntityType::FLYING_ENEMY)
	{
		// north-west
		cell.create(pos.x + 1, pos.y + 1);
		if (app->pathfinding->IsWalkable(cell))
			list_to_fill.list.add(PathNode(-1, -1, cell, this));

		// north-est
		cell.create(pos.x + 1, pos.y - 1);
		if (app->pathfinding->IsWalkable(cell))
			list_to_fill.list.add(PathNode(-1, -1, cell, this));

		// south-east
		cell.create(pos.x - 1, pos.y + 1);
		if (app->pathfinding->IsWalkable(cell))
			list_to_fill.list.add(PathNode(-1, -1, cell, this));

		// south - west
		cell.create(pos.x - 1, pos.y - 1);
		if (app->pathfinding->IsWalkable(cell))
			list_to_fill.list.add(PathNode(-1, -1, cell, this));
	}
	return list_to_fill.list.count();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
int PathNode::Score() const
{
	return g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const iPoint& destination)
{
	g = parent->g + 1;
	h = pos.DistanceTo(destination);

	return g + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
int Pathfinding::CreatePath(const iPoint& origin, const iPoint& destination, EntityType type)
{
	
	lastPath.Clear();

	if (IsWalkable(origin) == false || IsWalkable(destination) == false)
		return -1;

	PathList open;
	PathList close;
	PathNode origin_path(0, origin.DistanceManhattan(destination), origin, nullptr);

	open.list.add(origin_path);


	while (open.list.count() != 0)
	{
		close.list.add(open.GetNodeLowestScore()->data);
		open.list.del(open.GetNodeLowestScore());
		if (close.list.end->data.pos == destination)
		{
			const PathNode* parent = &close.list.end->data;
			while (parent->parent)
			{
				lastPath.PushBack(parent->pos);
				parent = parent->parent;
			}
			lastPath.Flip();
			return lastPath.Count();
		}
		else
		{
			PathList frontier;
			close.list.end->data.FindWalkableAdjacents(frontier, type);
			for (ListItem<PathNode>* iterator = frontier.list.start; iterator != nullptr; iterator = iterator->next)
			{
				if (close.Find(iterator->data.pos))
					continue;
				else if (open.Find(iterator->data.pos))
				{
					iterator->data.CalculateF(destination);
					PathNode new_node = open.Find(iterator->data.pos)->data;
					if (new_node.g > iterator->data.g)
					{
						new_node.parent = iterator->data.parent;
					}
				}
				else
				{
					iterator->data.CalculateF(destination);
					open.list.add(iterator->data);
				}
			}
		}
	}
	return -1;
}


bool Pathfinding::DrawPath(const DynArray<iPoint>* path)
{
	if (path->Count() <= 0)
		path;
	else
	{
		for (uint i = 0; i < path->Count(); ++i)
		{
			app->render->DrawRectangle({ app->map->MapToWorld(path->At(i)->x,path->At(i)->y).x, app->map->MapToWorld(path->At(i)->x,path->At(i)->y).y , app->map->data.tileWidth, app->map->data.tileHeight }, 255, 0, 0, 250);
		}
	}


	return false;
}