#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "App.h"
#include "Point.h"
#include "Textures.h"
#include "Animation.h"
#include "SDL_image/include/SDL_image.h"

#include "List.h"

enum class COLLIDER_DIRECTION;

struct TileEntity {

	SDL_Rect GetTileRect(int id) const;

	SString name;
	uint tile_width = 0;
	uint tile_height = 0;
	uint spacing = 0;
	uint margin = 0;
	uint tile_count = 0;
	uint columns = 0;
	SString image_path;
	SDL_Texture* texture = nullptr;
	uint width = 0;
	uint height = 0;
};
struct PropertyEntity {
	//SFX
	SString jump_sfx;
	SString copy_sfx;
	SString death_sfx;
	SString respawn_sfx;
};

enum class EntityState {
	IDLE,
	WALKING,
	JUMP,
	FALL,
	DEAD,
	UNKNOWN
};

struct EntitiesAnim {
	int id = 0;
	SString type;
	uint num_frames = 0;
	List<SDL_Rect*> frames;
	EntityState states = EntityState::UNKNOWN;
	uint FrameCount(pugi::xml_node&);
	float speed = 0.F;

	~EntitiesAnim()
	{
		ListItem<SDL_Rect*>* iter = frames.start;
		while (iter != frames.end)
		{
			RELEASE(iter->data);
			iter = iter->next;
		}
	}
};

struct EntityInfo {
	TileEntity tiled;
	PropertyEntity property;
	List<EntitiesAnim*> animations;
	uint num_animations = 0;

	~EntityInfo()
	{
		ListItem<EntitiesAnim*>* iter = animations.start;
		while (iter != animations.end)
		{
			RELEASE(iter->data);
			iter = iter->next;
		}
		
		app->tex->UnLoad(tiled.texture);
		tiled.texture = nullptr;
	}
};

enum class ENTITY_TYPE
{
	NO_ENTITY,
	PLAYER,
	PLATFORM,
	FLYING_ENEMY,
	GROUND_ENEMY,
};

class Entity
{
public:

	Entity(const fPoint& position, const char* name, ENTITY_TYPE type);
	~Entity();

	virtual bool Start() = 0;
	virtual void PreUpdate(float dt) = 0;
	virtual void Move(float dt) = 0;
	virtual void Draw();
	virtual void CleanUp() = 0;

	virtual void OnCollision(Collider*) = 0;
	virtual void LoadProperties(pugi::xml_node&);
	virtual void IdAnimToEntityState();
	virtual void PushBack() = 0;
	virtual bool Load(pugi::xml_node&) { return true; };
	virtual bool Save(pugi::xml_node&) const { return true; };
	bool LoadData(const char*);
public:
	SString name;
	Collider* collider = nullptr;
	pugi::xml_document	entity_data_file;

	fPoint position;
	fPoint speed;
	iPoint size;
	iPoint offset;
	float gravity;

	EntityInfo data;
	ENTITY_TYPE type = ENTITY_TYPE::NO_ENTITY;
	SDL_RendererFlip flip = (SDL_RendererFlip)SDL_FLIP_NONE;
	float anim_speed;
	Animation anim_idle;
	Animation* current_animation = nullptr;
	bool to_delete = false;
};

#endif
