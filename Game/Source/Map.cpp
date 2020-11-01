#include "Defs.h"
#include "Log.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Player.h"
#include "Map.h"
#include "Window.h"
#include "Collisions.h"
#include "Scene.h"
#include <math.h>

Map::Map() : Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void Map::Draw()
{
	if (map_loaded == false)
		return;

	ListItem<MapLayer*>* layer = this->data.layers.start;

	while (layer != NULL) {
		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int tile_id = layer->data->Get(x, y);
				if (tile_id > 0)
				{
					TileSet* tileset = GetTilesetFromTileId(tile_id);
					if (tileset != nullptr)
					{
						SDL_Rect r = tileset->GetTileRect(tile_id);
						iPoint pos = MapToWorld(x, y);
						app->render->DrawTexture(tileset->texture, pos.x, pos.y, &r, layer->data->parallax);
						



					}
				}
			}
		}
		layer = layer->next;
	}
}

TileSet* Map::GetTilesetFromTileId(int id) const
{
	// Tileset based on a tile id
	ListItem<TileSet*>* tile_item = data.tilesets.start;

	for (tile_item; tile_item != NULL; tile_item = tile_item->next)
	{
		if (tile_item->next == nullptr || tile_item->next->data->firstgid > id)
			return tile_item->data;
	}

}

iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tile_width;
		ret.y = y * data.tile_height;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (data.tile_width * 0.5f);
		ret.y = (x + y) * (data.tile_height * 0.5f);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

iPoint Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{

		float half_width = data.tile_width * 0.5f;
		float half_height = data.tile_height * 0.5f;
		ret.x = int((x / half_width + y / half_height) / 2);
		ret.y = int((y / half_height - (x / half_width)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting
bool Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	ListItem<TileSet*>* item;
	item = data.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// Remove all layers
	ListItem<MapLayer*>* item2;
	item2 = data.layers.start;

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.clear();


	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool Map::Load(const char* file_name)
{
	bool ret = true;
	SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if (ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for (tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if (ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for (layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{

		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if (ret == true)
			data.layers.add(lay);

	}
		

	pugi::xml_node object;
	for (object = map_file.child("map").child("objectgroup"); object && ret; object = object.next_sibling("objectgroup"))
	{
		ret = LoadObjects(object);

	}

	if (ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		ListItem<TileSet*>* item = data.tilesets.start;
		while (item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		ListItem<MapLayer*>* item_layer = data.layers.start;
		while (item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if (bg_color.Length() > 0)
		{
			SString red, green, blue;
			//bg_color.SubString(1, 2, red);
			//bg_color.SubString(3, 4, green);
			//bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.b = v;
		}

		SString orientation(map.attribute("orientation").as_string());

		if (orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if (orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if (orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool Map::LoadTilesetDetails(pugi::xml_node& tileset_data, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_data.attribute("name").as_string());
	set->firstgid = tileset_data.attribute("firstgid").as_int();
	set->tile_width = tileset_data.attribute("tilewidth").as_int();
	set->tile_height = tileset_data.attribute("tileheight").as_int();
	set->margin = tileset_data.attribute("margin").as_int();
	set->spacing = tileset_data.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_data.child("tileoffset");

	if (offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool Map::LoadTilesetImage(pugi::xml_node& tileset_data, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_data.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = app->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if (set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if (set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool Map::LoadLayer(pugi::xml_node& data, MapLayer* layer)
{
	bool ret = true;

	layer->name = data.attribute("name").as_string();
	layer->width = data.attribute("width").as_int();
	layer->height = data.attribute("height").as_int();
	layer->parallax = data.child("properties").child("property").attribute("value").as_float();
	pugi::xml_node layer_data = data.child("data");


	if (layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layer);
	}
	else
	{
		layer->data = new uint[layer->width * layer->height];
		memset(layer->data, 0, layer->width * layer->height);

		int i = 0;
		for (pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			layer->data[i++] = tile.attribute("gid").as_int(0);
		}
	}

	return ret;
}

bool Map::LoadObjects(pugi::xml_node& data)
{
	bool ret = true;
	if (data == NULL)
		LOG("Error");
	SString name(data.attribute("name").as_string());

	if (name == "Colliders")
	{
		for (pugi::xml_node obj = data.child("object"); obj && ret; obj = obj.next_sibling("object"))
		{
			app->collisions->AddCollider({ obj.attribute("x").as_int(),obj.attribute("y").as_int() ,obj.attribute("width").as_int() ,obj.attribute("height").as_int() }, COLLIDER_TYPE::COLLIDER);

		}

	}

	else if (name == "ColliderDmg")
	{
		for (pugi::xml_node obj = data.child("object"); obj && ret; obj = obj.next_sibling("object"))
			app->collisions->AddCollider({ obj.attribute("x").as_int(),obj.attribute("y").as_int() ,obj.attribute("width").as_int() ,obj.attribute("height").as_int() }, COLLIDER_TYPE::COLLIDER_DAMAGE);
	}

	else if (name == "Nextlvl")
	{
		for (pugi::xml_node obj = data.child("object"); obj && ret; obj = obj.next_sibling("object"))
			app->collisions->AddCollider({ obj.attribute("x").as_int(),obj.attribute("y").as_int() ,obj.attribute("width").as_int() ,obj.attribute("height").as_int() }, COLLIDER_TYPE::NEXTLVL);
					
	}



	return ret;
}

