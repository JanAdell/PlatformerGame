#include "Entity.h"
#include "Log.h"
#include "Render.h"

Entity::Entity(const fPoint& position, const char* name, ENTITY_TYPE type) :position(position), name(name), type(type)
{
	pugi::xml_document config_file;
	pugi::xml_node ret;

	pugi::xml_parse_result result = config_file.load_file("config.xml");

	if (result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("config").child("entities");

	LoadProperties(ret.child(name));
}

Entity::~Entity()
{

}

bool Entity::LoadData(const char* ent_data)
{
	bool ret = true;

	pugi::xml_parse_result result = entity_data_file.load_file(ent_data);

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", ent_data, result.description());
		ret = false;
	}

	//Info data entity loading
	pugi::xml_node node = entity_data_file.child("tileset");
	data.tiled.name.create(node.attribute("name").as_string());
	data.tiled.tile_width = node.attribute("tilewidth").as_uint();
	data.tiled.tile_height = node.attribute("tileheight").as_uint();
	data.tiled.spacing = node.attribute("spacing").as_uint();
	data.tiled.margin = node.attribute("margin").as_uint();
	data.tiled.tile_count = node.attribute("tilecount").as_uint();
	data.tiled.columns = node.attribute("columns").as_uint();
	data.tiled.image_path = node.child("image").attribute("source").as_string();
	data.tiled.width = node.child("image").attribute("width").as_uint();
	data.tiled.height = node.child("image").attribute("height").as_uint();

	//We load the animations from the tsx of the entity
	for (node = node.child("tile"); node; node = node.next_sibling("tile"))
	{
		pugi::xml_node anima = node.child("animation");
		EntitiesAnim* anim = new EntitiesAnim();

		SString anim_name = node.attribute("type").as_string();
		if (strcmp(anim_name.GetString(), "idle") == 0)
			anim->states = EntityState::IDLE;
		else if (strcmp(anim_name.GetString(), "walking") == 0)
			anim->states = EntityState::WALKING;
		else if (strcmp(anim_name.GetString(), "jump") == 0)
			anim->states = EntityState::JUMP;
		else if (strcmp(anim_name.GetString(), "death") == 0)
			anim->states = EntityState::DEAD;

		for (pugi::xml_node frame = anima.child("frame"); frame; frame = frame.next_sibling("frame"))
		{
			SDL_Rect* rect = new SDL_Rect(data.tiled.GetTileRect(frame.attribute("tileid").as_uint()));
			anim->frames.add(rect);
		}

		anim->speed = node.child("properties").child("property").attribute("value").as_float();

		data.animations.add(anim);
	}

	PushBack();

	//Here we are freeing the animations data because the info has been already loaded in the variables of the entity so we dont need it anymore
	ListItem<EntitiesAnim*>* anim_iterator = data.animations.start;
	while (anim_iterator != nullptr)
	{
		ListItem<SDL_Rect*>* rect_iterator = anim_iterator->data->frames.start;
		while (rect_iterator != nullptr)
		{
			RELEASE(rect_iterator->data);
			rect_iterator = rect_iterator->next;
		}
		RELEASE(anim_iterator->data);
		anim_iterator = anim_iterator->next;
	}

	return ret;
}

void Entity::Draw()
{
	if (current_animation != nullptr)
		app->render->DrawTexture(data.tiled.texture, position.x, position.y, &current_animation->GetCurrentFrame(), 1.0f, flip);
	else
		app->render->DrawTexture(data.tiled.texture, position.x, position.y);
	
}

void Entity::LoadProperties(pugi::xml_node& node)
{
	size.x = node.child("size").attribute("x").as_int();
	size.y = node.child("size").attribute("y").as_int();
	offset.x = node.child("offset").attribute("x").as_int();
	offset.y = node.child("offset").attribute("y").as_int();
	speed.x = node.child("speed").attribute("x").as_float();
	speed.y = node.child("speed").attribute("y").as_float();
	gravity = node.child("gravity").attribute("gravity").as_float();
	data.property.copy_sfx.create(node.child("copy_fx").attribute("path").as_string());
	data.property.death_sfx.create(node.child("death_fx").attribute("path").as_string());
	data.property.jump_sfx.create(node.child("jump_fx").attribute("path").as_string());
	data.property.respawn_sfx.create(node.child("respawn_fx").attribute("path").as_string());

}

void Entity::IdAnimToEntityState()
{
	data.animations.start->data->states = EntityState::IDLE;
}

SDL_Rect TileEntity::GetTileRect(int id) const {
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (id % columns));
	rect.y = margin + ((rect.h + spacing) * (id / columns));
	return rect;
}

//Return how many frames are in one animation
uint EntitiesAnim::FrameCount(pugi::xml_node& n) {
	num_frames = 0;
	pugi::xml_node node = n;
	for (; node != NULL; node = node.next_sibling("frame")) {
		num_frames++;
	}

	return num_frames;
}