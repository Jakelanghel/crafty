


// int entry(int argc, char **argv) {
//     window.title = fixed_string("Jake's Game");
//     window.width = 1280;
//     window.height = 720;
//     window.x = 200;
//     window.y = 200;
//     window.clear_color = hex_to_rgba(0x2a2d3aff);

    // Gfx_Image* player = load_image_from_disk(fixed_string("player.png"), get_heap_allocator());
    // assert(player, "Failed to load player image!");
//     printf("Image loaded: %d x %d\n", player->width, player->height);
    
//     while (!window.should_close) {
//         reset_temporary_storage();
//         os_update();

        // if (is_key_just_pressed(KEY_ESCAPE)) {
        //     window.should_close = true;
        // }

//         Matrix4 xform = m4_scalar(1.0);
//         xform = m4_translate(xform, v3(-.25, -.25, 0));
        
//         draw_image_xform(player, xform, v2(1.0f, 1.0f), COLOR_WHITE); 
//         gfx_update();
//     }

//     return 0;
// }


int entry(int argc, char **argv) {
	
	// This is how we (optionally) configure the window.
	// To see all the settable window properties, ctrl+f "struct Os_Window" in os_interface.c
	window.title = STR("Minimal Game Example");

    Gfx_Image* player = load_image_from_disk(fixed_string("player.png"), get_heap_allocator());
    assert(player, "Failed to load player image!");
    Vector2 player_pos = v2(0,0);
    float64 last_time = os_get_elapsed_seconds();
	
	
	while (!window.should_close) {
		reset_temporary_storage();

        float64 now = os_get_elapsed_seconds();
        float64 delta_t = now = last_time;
        last_time = now;

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
        player_pos = v2_add(player_pos, v2_mulf(input_axis, 1.0 * delta_t));
		
		Matrix4 xform = m4_scalar(1.0);
		xform         = m4_translate(xform, v3(player_pos.x, player_pos.y, 0));
		draw_image_xform(player, xform, v2(250, 250), COLOR_GREEN);
		
		os_update(); 
		gfx_update();
	}

	return 0;
}





