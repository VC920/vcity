#include <raylib.h>
#include "game.h"

int main()
{
    int monitor = GetCurrentMonitor();
    int screen_width = GetMonitorWidth(monitor);
    int screen_height = GetMonitorHeight(monitor);
    
    InitWindow(screen_width, screen_height, "vcity");
    ToggleFullscreen();
    InitAudioDevice();
    SetTargetFPS(200);

    Game game = game_init();

    while (!WindowShouldClose()) {
	/* UPDATE */
	game_update(&game, GetFrameTime());
	
	/* RENDER */
	BeginDrawing();
	ClearBackground(DARKGRAY);
	
	// 3D Render
	BeginMode3D(game.camera);
	game_render_3d(&game);
	EndMode3D();
	
	// UI Render
	game_render_ui(&game);
	
	EndDrawing();
    }
    
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
