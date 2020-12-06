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
#include "EntityManager.h"
#include <math.h>

Map::Map() : Module(), mapLoaded(false)
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
	if (mapLoaded == false)
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
		ret.x = x * data.tileWidth;
		ret.y = y * data.tileHeight;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (data.tileWidth * 0.5f);
		ret.y = (x + y) * (data.tileHeight * 0.5f);
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
		ret.x = x / data.tileWidth;
		ret.y = y / data.tileHeight;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{

		float half_width = data.tileWidth * 0.5f;
		float half_height = data.tileHeight * 0.5f;
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
	rect.w = tileWidth;
	rect.h = tileHeight;
	rect.x = margin + ((rect.w + spacing) * (relative_id % numTilesWidth));
	rect.y = margin + ((rect.h + spacing) * (relative_id / numTilesWidth));
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
	mapFile.reset();

	return true;
}

// Load new map
bool Map::Load(const char* file_name)
{
	bool ret = true;
	SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());

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
	for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
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
	for (layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{

		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if (ret == true)
			data.layers.add(lay);

	}
		

	pugi::xml_node object;
	for (object = mapFile.child("map").child("objectgroup"); object && ret; object = object.next_sibling("objectgroup"))
	{
		ret = LoadObjects(object);

	}

	if (ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tileWidth: %d tileHeight: %d", data.tileWidth, data.tileHeight);

		ListItem<TileSet*>* item = data.tilesets.start;
		while (item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tileWidth, s->tileHeight);
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

	mapLoaded = ret;

	return ret;
}

// Load map general properties
bool Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = mapFile.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tileWidth = map.attribute("tilewidth").as_int();
		data.tileHeight = map.attribute("tileheight").as_int();
		SString bg_color(map.attribute("backgroundcolor").as_string());

		data.backgroundColor.r = 0;
		data.backgroundColor.g = 0;
		data.backgroundColor.b = 0;
		data.backgroundColor.a = 0;

		if (bg_color.Length() > 0)
		{
			SString red, green, blue;
			//bg_color.SubString(1, 2, red);
			//bg_color.SubString(3, 4, green);
			//bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.backgroundColor.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.backgroundColor.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.backgroundColor.b = v;
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
	set->tileWidth = tileset_data.attribute("tilewidth").as_int();
	set->tileHeight = tileset_data.attribute("tileheight").as_int();
	set->margin = tileset_data.attribute("margin").as_int();
	set->spacing = tileset_data.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_data.child("tileoffset");

	if (offset != NULL)
	{
		set->offsetX = offset.attribute("x").as_int();
		set->offsetY = offset.attribute("y").as_int();
	}
	else
	{
		set->offsetX = 0;
		set->offsetY = 0;
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
		set->texWidth = image.attribute("width").as_int();

		if (set->texWidth <= 0)
		{
			set->texWidth = w;
		}

		set->texHeight = image.attribute("height").as_int();

		if (set->texHeight <= 0)
		{
			set->texHeight = h;
		}

		set->numTilesWidth = set->texWidth / set->tileWidth;
		set->numTilesHeight = set->texHeight / set->tileHeight;
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
	LoadProperties(data, *layer);
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

bool Map::LoadProperties(pugi::xml_node& node, MapLayer& layer)
{
	bool ret = false;

	pugi::xml_node data = node.child("properties");

	if (data != NULL)
	{
		pugi::xml_node prop;

		for (prop = data.child("property"); prop; prop = prop.next_sibling("property"))
		{
			Properties::Property* p = new Properties::Property();

			p->name = prop.attribute("name").as_string();
			p->value = prop.attribute("value").as_int();
					
			layer.properties.list.add(p);
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
			app->collisions->AddCollider({ obj.attribute("x").as_int(),obj.attribute("y").as_int() ,obj.attribute("width").as_int() ,obj.attribute("height").as_int() }, ColliderType::COLLIDER);

		}

	}

	else if (name == "ColliderDmg")
	{
		for (pugi::xml_node obj = data.child("object"); obj && ret; obj = obj.next_sibling("object"))
			app->collisions->AddCollider({ obj.attribute("x").as_int(),obj.attribute("y").as_int() ,obj.attribute("width").as_int() ,obj.attribute("height").as_int() }, ColliderType::COLLIDER_DAMAGE);
	}

	else if (name == "Nextlvl")
	{
		for (pugi::xml_node obj = data.child("object"); obj && ret; obj = obj.next_sibling("object"))
			app->collisions->AddCollider({ obj.attribute("x").as_int(),obj.attribute("y").as_int() ,obj.attribute("width").as_int() ,obj.attribute("height").as_int() }, ColliderType::NEXTLVL);
					
	}

	else if (name == "Spawn")
	{
		fPoint spawnPoint;
		pugi::xml_node obj = data.child("object");
		spawnPoint.x = obj.attribute("x").as_int();
		spawnPoint.y = obj.attribute("y").as_int();
		app->entityManager->player = app->entityManager->CreateEntity(spawnPoint, EntityType::PLAYER);
						
		
	}

	else if (name == "Checkpoint")
	{
		for (pugi::xml_node obj = data.child("object"); obj && ret; obj = obj.next_sibling("object"))
		{
			//app->entityManager->player->checkpointPos.x = obj.attribute("x").as_int();
			//app->entityManager->player->checkpointPos.y = obj.attribute("y").as_int();
			app->collisions->AddCollider({ obj.attribute("x").as_int(),obj.attribute("y").as_int() ,obj.attribute("width").as_int() ,obj.attribute("height").as_int() }, ColliderType::CHECKPOINT);
		}
	}

	else if (name == "GroundEnemySpawn")
	{
		for (pugi::xml_node obj = data.child("object"); obj && ret; obj = obj.next_sibling("object"))
			app->entityManager->groundEnemy = app->entityManager->CreateEntity({ obj.attribute("x").as_float(),obj.attribute("y").as_float() }, EntityType::GROUND_ENEMY);
	}
	else if (name == "FlyingEnemySpawn")
	{
		for (pugi::xml_node obj = data.child("object"); obj && ret; obj = obj.next_sibling("object"))
			app->entityManager->flyingEnemy = app->entityManager->CreateEntity({ obj.attribute("x").as_float(),obj.attribute("y").as_float() }, EntityType::FLYING_ENEMY);
	}


	return ret;
}

bool Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	bool ret = false;
	ListItem<MapLayer*>* item;
	item = data.layers.start;

	for (item = data.layers.start; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if (layer->properties.Get("Navigation", 0) == 0)
			continue;

		uchar* map = new uchar[layer->width * layer->height];
		memset(map, 1, layer->width * layer->height);

		for (int y = 0; y < layer->width * layer->height; ++y)
		{
			if (layer->data[y] == NULL)
				map[y] = 1;
			else
				map[y] = 0;
		}

		*buffer = map;
		width = data.width;
		height = data.height;
		ret = true;

		break;
	}
	return ret;
}

int Properties::Get(const char* name, int default_value) const
{
	ListItem<Property*>* item = list.start;

	while (item)
	{
		if (item->data->name == name)
			return item->data->value;
		item = item->next;
	}

	return default_value;
}

