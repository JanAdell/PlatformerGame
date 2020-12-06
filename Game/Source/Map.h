#ifndef __MAP_H__
#define __MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "List.h"
#include "Point.h"
#include "Module.h"
#include "SString.h"

struct Properties
{
	struct Property
	{
		SString name;
		int value;
	};

	~Properties()
	{
		ListItem<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.clear();
	}

	int Get(const char* name, int default_value = 0) const;

	List<Property*>	list;
};

// ----------------------------------------------------
struct MapLayer
{
	SString	name;
	int			width;
	int			height;
	uint* data;
	Properties	properties;
	float		parallax;
	int			navigation;
	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	inline uint Get(int x, int y) const
	{
		return data[(y * width) + x];
	}
};

// ----------------------------------------------------
struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	SString				name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tileWidth;
	int					tileHeight;
	SDL_Texture*		texture;
	int					texWidth;
	int					texHeight;
	int					numTilesWidth;
	int					numTilesHeight;
	int					offsetX;
	int					offsetY;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tileWidth;
	int					tileHeight;
	SDL_Color			backgroundColor;
	MapTypes			type;
	List<TileSet*>		tilesets;
	List<MapLayer*>		layers;
};

// ----------------------------------------------------
class Map : public Module
{
public:

	Map();

	// Destructor
	virtual ~Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;
	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_data, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_data, TileSet* set);
	bool LoadLayer(pugi::xml_node& data, MapLayer* layer);
	bool LoadProperties(pugi::xml_node& node, MapLayer& layer);
	bool LoadObjects(pugi::xml_node& data);

	

	TileSet* GetTilesetFromTileId(int id) const;

public:

	MapData data;
	Collider* collider;
	

private:

	pugi::xml_document	mapFile;
	SString				folder;
	bool				mapLoaded;
};

#endif // __MAP_H__