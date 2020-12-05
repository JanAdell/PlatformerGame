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
		
}

Entity::~Entity()
{

}


void Entity::Draw()
{
	if (currentAnim != nullptr)
		app->render->DrawTexture(characterTex, position.x, position.y, &currentAnim->GetCurrentFrame(), 1.0f, flip);
	else
		app->render->DrawTexture(characterTex, position.x, position.y);
	
}




//Return how many frames are in one animation
/*uint EntitiesAnim::FrameCount(pugi::xml_node& n) {
	num_frames = 0;
	pugi::xml_node node = n;
	for (; node != NULL; node = node.next_sibling("frame")) {
		num_frames++;
	}

	return num_frames;
}*/