/*
	- This is defining a new structure called Sprite with two members:
	- Gfx_Image* image: A pointer to an image associated with the sprite
	- Vector2 size: Stores the sprite's dimensions as a Vector2 
*/
typedef struct Sprite {
	Gfx_Image* image;
	Vector2 size;
}Sprite;


/*
	- Here, you're defining an enum (enumeration) called SpriteID, which assigns names to specific integer values. 
	- Enums are a convenient way to label constants, which helps with code readability and prevents "magic numbers.

	- SPRITE_MAX will hold the value 4 here, as it’s the fifth item in the list. 
	- SPRITE_MAX effectively becomes the number of items in the enum, 
	- which you can use for things like array sizing.
*/
typedef enum SpriteID {
	SPRITE_NIL,
	SPRITE_PLAYER,
	SPRITE_TREE_0,
	SPRITE_ROCK_0,
	SPRITE_MAX,
}SpriteID;


/*
	This line declares an array called sprites of type Sprite with a length equal to SPRITE_MAX (which is 4 in this case).
	Because SPRITE_MAX is defined in the enum, the compiler can use its value (4) to set the array length.
*/
Sprite sprites[SPRITE_MAX]; 


/*
	- This defines a function named get_sprite that takes a SpriteID (id) and returns a pointer to a Sprite (Sprite*).
	- &sprites[id]: This returns the address of sprites[id] if id is within the bounds (0 <= id < SPRITE_MAX).
	- The & symbol gets the memory address of the array element, so &sprites[id] is a pointer to that specific Sprite.
	- return &sprites[0];: If id is out of bounds, it returns &sprites[0], a fallback to prevent errors. 
	- This is often called a "nil pointer," similar to null in JavaScript, as it’s returning a "safe" value (the default sprites[0]) if the ID is invalid.
*/
Sprite* get_sprite(SpriteID id) {
	if (id >= 0 && id < SPRITE_MAX) {
		return &sprites[id];
	}
	 return &sprites[0];
}



typedef enum EntityArchetype {
	arch_nil = 0,
	arch_rock = 1,
	arch_tree = 2,
	arch_player = 3,
}EntityArchetype;



typedef struct Entity {
	bool is_valid;
	EntityArchetype arch;
	Vector2 pos;
	bool render_sptite;
	SpriteID sprite_id;
}Entity;
#define MAX_ENTITY_COUNT  1024


typedef struct World {
	Entity entities[MAX_ENTITY_COUNT];
}World;
World* world = 0;


Entity* entity_create() {
	Entity* entity_found = 0;
	for( int i = 0; i < MAX_ENTITY_COUNT; i++) {
		Entity* existing_entity = &world->entities[i];
		if(!existing_entity->is_valid) {
			entity_found = existing_entity;
			break;
		}
	}
	assert(entity_found, "No more free entities!");
	entity_found->is_valid = true;
	return entity_found;
}


void entity_destroy(Entity* entity) {
	memset(entity, 0, sizeof(Entity));
}

void setup_player(Entity* en) {
	en->arch = arch_player;
	en->sprite_id = SPRITE_PLAYER;
}

void setup_rock(Entity* en) {
	en->arch = arch_rock;
	en->sprite_id = SPRITE_ROCK_0;

}

void setup_tree(Entity* en) {
	en->arch = arch_tree;
	en->sprite_id = SPRITE_TREE_0;
	// en->sprite_id = SPRITE_TREE_1;
}

int entry(int argc, char **argv) {
	
	// This is how we (optionally) configure the window.
	// To see all the settable window properties, ctrl+f "struct Os_Window" in os_interface.c
	window.title = STR("Minimal Game Example");
	window.clear_color = hex_to_rgba(0x2a2d3aff);
	
	world = alloc(get_heap_allocator(), sizeof(World));
	memset(world, 0, sizeof(World));
	sprites[SPRITE_PLAYER] = (Sprite){.image=load_image_from_disk(STR("player.png"), get_heap_allocator()), .size=v2(21.0, 37.0)};
	sprites[SPRITE_TREE_0] = (Sprite){.image=load_image_from_disk(STR("tree0.png"), get_heap_allocator()), .size=v2(23.0, 62.0)};
	sprites[SPRITE_ROCK_0] = (Sprite){.image=load_image_from_disk(STR("rock0.png"), get_heap_allocator()), .size=v2(10.0, 8.0)};

	// Sprites[SPRITE_CAULDRON] = (Sprite){.image=load_image_from_disk(STR("cauldron.png"), get_heap_allocator(), .size=v2(0, 0))};
	// Sprites[SPRITE_ROCK_1] = (Sprite){.image=load_image_from_disk(STR("rock1.png"), get_heap_allocator(), .size=v2(0, 0))};

	// Gfx_Image* player = load_image_from_disk(fixed_string("player.png"), get_heap_allocator());
    // Gfx_Image* rock_1 = load_image_from_disk(fixed_string("rock1.png"), get_heap_allocator());


	Entity* player_en = entity_create();
	setup_player(player_en);

	for (int i = 0; i< 10; i++) {
		Entity* en = entity_create();
		setup_rock(en);
		en->pos = v2(get_random_float32_in_range(-200, 200),  get_random_float32_in_range(-200, 200));
	}

	for (int i = 0; i< 10; i++) {
		Entity* en = entity_create();
		setup_tree(en);
		en->pos = v2(get_random_float32_in_range(-300, 300),  get_random_float32_in_range(-300, 300));
	}


    float64 last_time = os_get_elapsed_seconds();
	
	
	while (!window.should_close) {
		reset_temporary_storage();

		draw_frame.projection = m4_make_orthographic_projection(window.width * -0.5, window.width * 0.5, window.height * -0.5, window.height * 0.5, -1, 10 );

		float zoom = 5.3;
		draw_frame.camera_xform = m4_make_scale(v3(1.0/zoom, 1.0/zoom, 1.0));

        float64 now = os_get_elapsed_seconds();
        float64 delta_t = now - last_time;
        last_time = now;

		os_update(); 

		for(int i = 0; i < MAX_ENTITY_COUNT; i++) {
			Entity* en = &world->entities[i];
			if (en->is_valid) {
				switch (en->arch) {

					default:
					{
						Sprite* sprite = get_sprite(en->sprite_id);
						Matrix4 xform = m4_scalar(1.0);
						xform         = m4_translate(xform, v3(en->pos.x, en->pos.y, 0));
						xform         = m4_translate(xform, v3(sprite->size.x * -0.5, 0, 0));
						draw_image_xform(sprite->image, xform, sprite->size, COLOR_WHITE);
						break;
					}
				}
			}
		}

         if (is_key_just_pressed(KEY_ESCAPE)) {
            window.should_close = true;
        }

        Vector2 input_axis = v2(0, 0);
		if (is_key_down('A')) {
			input_axis.x -= 1.0;
		}
		if (is_key_down('D')) {
			input_axis.x += 1.0;
		}
		if (is_key_down('S')) {
			input_axis.y -= 1.0;
		}
		if (is_key_down('W')) {
			input_axis.y += 1.0;
		}

        player_en->pos = v2_add(player_en->pos, v2_mulf(input_axis, 100 * delta_t));

		gfx_update();
	}

	return 0;
}





