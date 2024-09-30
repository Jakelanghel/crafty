
// int entry(int argc, char **argv) {
//     window.title = fixed_string("Jake's Game");
//     window.width = 1280;
//     window.height = 720;
//     window.x = 200;
//     window.y = 200;
//     window.clear_color = hex_to_rgba(0x2a2d3aff);

//     Gfx_Image* player = load_image_from_disk(fixed_string("player.png"), get_heap_allocator());
//     assert(player, "Shits Fucked");

//     while (!window.should_close) {
//         reset_temporary_storage();

//         os_update();

//         if(is_key_just_pressed(KEY_ESCAPE)) {
//             window.should_close = true;
//         }
//         float64 now = os_get_elapsed_seconds();
//         Matrix4 xform = m4_scalar(1.0);

//         xform = m4_translate(xform, v3(-.25f, -.25f, 0));
//         // draw_image_xform(player, xform, v2(.5f, .5f), COLOR_WHITE);
//         draw_rect(v2(window.width / 2.0f - 105.5f, window.height / 2.0f - 45.5f), v2(211, 91), COLOR_WHITE);

//         gfx_update();
//     }

//     return 0;
// }

int entry(int argc, char **argv) {
    window.title = fixed_string("Jake's Game");
    window.width = 1280;
    window.height = 720;
    window.x = 200;
    window.y = 200;
    window.clear_color = hex_to_rgba(0x2a2d3aff);

    Gfx_Image* player = load_image_from_disk(fixed_string("player.png"), get_heap_allocator());
    assert(player, "Failed to load player image!");

    while (!window.should_close) {
        reset_temporary_storage();
        os_update();

        if (is_key_just_pressed(KEY_ESCAPE)) {
            window.should_close = true;
        }

        Matrix4 xform = m4_identity(); // Start with the identity matrix
        // Center the player image in the window
        xform = m4_translate(xform, v3(window.width / 2.0f - 10.0f / 2.0f, window.height / 2.0f - 20.0f / 2.0f, 0));

        // Draw the player image using its original size
        draw_image_xform(player, xform, v2(1.0f, 1.0f), COLOR_WHITE); 

        gfx_update();
    }

    return 0;
}
