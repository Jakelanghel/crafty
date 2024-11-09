bool almost_equals(float a, float b, float epsilon) {
	return fabs(a - b) <= epsilon;
}

bool animate_f32_to_target(float* value, float target, float delta_t, float rate) {
	*value += (target = *value) * (1.0 - pow(2.9f, -rate * delta_t));
	if(almost_equals(*value, target, 0.001f))
	{
		*value = target;
		return true; // reached
	}
	return false;
}

void animate_v2_to_target(Vector2* value, Vector2 target, float delta_t, float rate) {
	// animate_to_target(&value->x, &value->x, delta_t, rate);
	// animate_to_target(&value->y, &value->y, delta_t, rate);
}
 



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


/*
	This defines a custom data structure, World, that contains an array of Entity structs. The array size is set by MAX_ENTITY_COUNT, so this World struct can hold up to 1024 entities.
*/
typedef struct World {
	Entity entities[MAX_ENTITY_COUNT];
}World;


/*
	Here, World* world is declaring a pointer named world that points to a World struct. 
	Setting world = 0 (or NULL in C) initializes it as a null pointer, 
	meaning it doesn’t point to any World data yet. 
	Later, you’ll allocate memory to world, making it point to an actual World struct.
*/
World* world = 0;


/*
	The entity_create function is designed to create a new entity in your game. 
	It searches through a predefined list of entities (up to MAX_ENTITY_COUNT) to find a free (unallocated) entity that can be initialized and used.
*/

Entity* entity_create() {

	/*
		Here, you're declaring a pointer to an Entity called entity_found and initializing it to 0 
		(which means it’s currently pointing to nothing or is effectively a null pointer).
	 	This will later hold the address of a valid entity if one is found.
	*/
	Entity* entity_found = 0;

	/*
		This loop iterates through the array of entities in the world. It checks each entity to see if it is valid or not.
	*/
	for( int i = 0; i < MAX_ENTITY_COUNT; i++) {

		/*
		- world is a pointer to a World structure.
		- world->entities accesses the entities array within the World struct that world points to.
		- The & operator gets the address of the specific entity at index i in the entities array, allowing existing_entity to point to it.
		*/
		Entity* existing_entity = &world->entities[i];


	/*
		This checks if the is_valid flag of the existing_entity is false. If it is, that means this entity is free to be used.
		If an invalid entity is found, entity_found is set to point to this existing_entity, and the loop is exited with break.
	*/
		if(!existing_entity->is_valid) {
			entity_found = existing_entity;
			break;
		}
	}
	assert(entity_found, "No more free entities!");

	/*
		Once a free entity is found, its is_valid flag is set to true, indicating that it’s now in use. Finally, the function returns the pointer to this newly created entity.
	*/
	entity_found->is_valid = true;
	return entity_found;
}

/*
		The function takes a pointer to an Entity as its parameter (Entity* entity).
        It returns void, meaning it doesn’t return any value.

    Using memset:
        The function uses memset to set all bytes of the entity structure to zero. Here’s what happens:
            First Argument: entity is the pointer to the Entity you want to destroy.
            Second Argument: 0 means that all bytes will be set to zero.
            Third Argument: sizeof(Entity) specifies the number of bytes to set. This is equal to the size of the Entity structure.

    Effect of memset:
        Setting all bytes to zero effectively resets all fields in the Entity structure:
            is_valid: This will be set to 0 (or false), indicating the entity is no longer valid.
            arch: This will be reset to 0 (or arch_nil).
            pos: The position vector will be set to (0, 0).
            render_sprite: This will also be reset to 0 (or false).
            sprite_id: This will be reset to 0 (or SPRITE_NIL).

	Purpose of entity_destroy
		The purpose of this function is to effectively "clean up" the entity when it is no longer needed, ensuring that the memory it occupied can be reused for other entities in the future.
		By setting the entity's properties to their default states, it helps prevent the game from behaving unexpectedly if the entity is accidentally accessed later on.

	Memory Management
		While this function resets the entity in memory, it does not free the memory allocated for the entity itself. 
		In this case, the entity is part of a fixed-size array within the World structure, so there’s no need to explicitly free memory.
*/
void entity_destroy(Entity* entity) {
	memset(entity, 0, sizeof(Entity));
}


// Initializes the given entity as a player by setting its archetype
// to 'arch_player' and assigning the corresponding sprite ID
// to 'SPRITE_PLAYER'.
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
	window.title = STR("Plague Dr.");
	window.clear_color = hex_to_rgba(0x2a2d3aff);
	// Allocates memory for the game world structure and assigns it to the 'world' pointer.
	world = alloc(get_heap_allocator(), sizeof(World));
	// Initialize the world object in memory by setting all its bytes to zero.
	// This ensures all entity states and properties are reset to their default values.
	memset(world, 0, sizeof(World));
	sprites[SPRITE_PLAYER] = (Sprite){.image=load_image_from_disk(STR("player.png"), get_heap_allocator()), .size=v2(21.0, 37.0)};
	sprites[SPRITE_TREE_0] = (Sprite){.image=load_image_from_disk(STR("tree0.png"), get_heap_allocator()), .size=v2(23.0, 62.0)};
	sprites[SPRITE_ROCK_0] = (Sprite){.image=load_image_from_disk(STR("rock0.png"), get_heap_allocator()), .size=v2(10.0, 8.0)};

	// Sprites[SPRITE_CAULDRON] = (Sprite){.image=load_image_from_disk(STR("cauldron.png"), get_heap_allocator(), .size=v2(0, 0))};
	// Sprites[SPRITE_ROCK_1] = (Sprite){.image=load_image_from_disk(STR("rock1.png"), get_heap_allocator(), .size=v2(0, 0))};
	// Gfx_Image* player = load_image_from_disk(fixed_string("player.png"), get_heap_allocator());
    // Gfx_Image* rock_1 = load_image_from_disk(fixed_string("rock1.png"), get_heap_allocator());

	// Create a new Entity on the heap and store its pointer in player_en.
	// Then, pass player_en to setup_player to configure it as the player character.
	Entity* player_en = entity_create();
	setup_player(player_en);


	// Create 10 rocks, each with a random position within the range of -200 to 200 on both axes.
	// Each rock is created using entity_create, set up with setup_rock, and its position is set using v2() with random coordinates.
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
	float zoom = 5.3;
	Vector2 camera_pos = v2(0,0);
	
	
	while (!window.should_close) { 	// Continue looping until the window is closed 
    reset_temporary_storage(); 	// Reset any temporary data structures before starting a new frame
    float64 now = os_get_elapsed_seconds();	// Calculate the time that has passed since the last frame to track frame time
    float64 delta_t = now - last_time; 	// Calculate the time difference between this frame and the last one
    last_time = now;	 // Store the current time as the last time for the next frame
    draw_frame.projection = m4_make_orthographic_projection( 	// Set up the projection matrix for the camera, defining the visible area of the game world
        window.width * -0.5, window.width * 0.5,	 // Define the x-axis range for the camera
        window.height * -0.5, window.height * 0.5,	 // Define the y-axis range for the camera
        -1, 10 	// Define the near and far planes for the camera (depth range)
    );

	// ============================================
	// =============== CAMERA =====================S
	// ============================================
	{
		Vector2 target_pos = player_en->pos;
		animate_to_target(&camera_pos, target_pos, delta_t, 15.0f);
		float zoom = 5.3; // Set the zoom level for the camera
		draw_frame.camera_xform = m4_make_scale(v3(1.0, 1.0, 1.0));
		draw_frame.camera_xform = m4_mul(draw_frame.camera_xform, m4_make_translation(v3(camera_pos.x, camera_pos.y, 0)));
		draw_frame.camera_xform = m4_mul(draw_frame.camera_xform, m4_make_scale(v3(1.0/zoom, 1.0/zoom, 1.0)));
	}

  

   

    os_update(); // Call the platform-specific update function (likely updates input, etc.)

    // Iterate over all entities in the world (assuming world->entities is an array of entities)
    for (int i = 0; i < MAX_ENTITY_COUNT; i++) {
        Entity* en = &world->entities[i]; // Get the pointer to the entity at index i
        if (en->is_valid) { // Check if the entity is valid (i.e., it exists and is active)
            switch (en->arch) { // Perform different actions depending on the entity's architecture/type
                default: {
                    // Get the sprite associated with the entity (it could be different based on sprite_id)
                    Sprite* sprite = get_sprite(en->sprite_id);

                    // Initialize the transformation matrix for the entity (its position and scaling)
                    Matrix4 xform = m4_scalar(1.0); // Start with an identity matrix (no transformation)
                    xform = m4_translate(xform, v3(en->pos.x, en->pos.y, 0)); // Move the sprite to the entity's position
                    xform = m4_translate(xform, v3(sprite->size.x * -0.5, 0, 0)); // Center the sprite horizontally

                    // Draw the sprite to the screen with the calculated transformation matrix
                    draw_image_xform(sprite->image, xform, sprite->size, COLOR_WHITE); // Draw the sprite with the transformation and white color
                    break;
                }
            }
        }
    }

    // Check if the Escape key was just pressed (to close the game window)
    if (is_key_just_pressed(KEY_ESCAPE)) {
        window.should_close = true; // Set the window to close
    }

    // Set up player input handling (WASD keys for movement)
    Vector2 input_axis = v2(0, 0); // Initialize the input axis as zero
    if (is_key_down('A')) { // If the A key is pressed, move left
        input_axis.x -= 1.0;
    }
    if (is_key_down('D')) { // If the D key is pressed, move right
        input_axis.x += 1.0;
    }
    if (is_key_down('S')) { // If the S key is pressed, move down
        input_axis.y -= 1.0;
    }
    if (is_key_down('W')) { // If the W key is pressed, move up
        input_axis.y += 1.0;
    }

    // Update the player's position based on the input axis and the time passed between frames (delta_t)
    player_en->pos = v2_add(player_en->pos, v2_mulf(input_axis, 100 * delta_t)); // Multiply the input by speed and delta time for smooth movement

    gfx_update(); // Update the graphics (e.g., swap buffers, draw everything to the screen)
}


	return 0;
}





