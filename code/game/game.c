#include "game.h"

#include <raymath.h>

Game game_init()
{
    GameMode game_mode = MENU;
    
    Music bgm = LoadMusicStream("");
    PlayMusicStream(bgm);

    Map map = map_init("resource/map/000.map");
    
    Camera3D camera;
    camera.position = (Vector3){ map.width * 0.5f, 20.0f, map.height * 0.5f + 10.0f };
    camera.target = (Vector3){ map.width * 0.5f, 0.0f, map.height * 0.5f };
    camera.up = (Vector3){ 0.0f, 0.0f, -1.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Vector3 car_spawn_pos = { 20.0f, 0.0f, 20.0f };
    Vector3 car_size = { 1.0f, 0.5f, 1.5f};
    Vehicle car = vehicle_init(car_spawn_pos, car_size, CAR_CONFIG);

    Vector3 truck_spawn_pos = {40.0f, 0.0f, 20.0f };
    Vector3 truck_size = { 1.0f, 0.5f, 2.5f};
    Vehicle truck = vehicle_init(truck_spawn_pos, truck_size, TRUCK_CONFIG);

    Vector3 player_spawn_pos = { 21.0f, 0.0f, 21.0f };
    Vector3 player_size = { 0.5f, 0.4f, 0.25f };
    Player player = player_init(player_spawn_pos, player_size);
    
    return (Game) {
	.game_mode = game_mode,
	.bgm = bgm,
	.camera = camera,
	.map = map,
	.drive_vehicle = { 0 },
	.car = car,
	.truck = truck,
	.player = player
    };
}

void game_update(Game *game, float delta_time)
{   
    Music *bgm = &game->bgm;
    UpdateMusicStream(*bgm);

    Camera3D *camera = &game->camera;
    
    Vehicle *car = &game->car;
    vehicle_update(car, delta_time);

    Vehicle *truck = &game->truck;
    vehicle_update(truck, delta_time);
    
    Player *player = &game->player;

    if (IsKeyPressed(KEY_PAGE_UP)) {
	game->game_mode = EDITOR;
    }
    
    // 开始页面
    if (game->game_mode == MENU) {
	if (IsKeyPressed(KEY_ENTER)) {
	    game->game_mode = WALK;
	}
    }

    // 编辑模式
    else if (game->game_mode == EDITOR) {
	camera->target = camera->position;
	camera->target.y -= 10.0f;
	if (IsKeyDown(KEY_W)) {
	    camera->position.z -= 2 * delta_time;
	}
	if (IsKeyDown(KEY_S)) {
	    camera->position.z += 2 * delta_time;
	}
	if (IsKeyDown(KEY_A)) {
	    camera->position.x -= 2 * delta_time;
	}
	if (IsKeyDown(KEY_D)) {
	    camera->position.x += 2 * delta_time;
	}
	if (IsKeyPressed(KEY_PAGE_DOWN)) {
	    game->game_mode = WALK;
	}
    }
    
    // 驾驶模式
    else if (game->game_mode == DRIVE) {
	vehicle_control(game->drive_vehicle, delta_time);
	
	player->position = game->drive_vehicle->position;
	
	camera->position = game->drive_vehicle->position;
	if (vehicle_get_speed(game->drive_vehicle) < 4.0f) {
	    camera->position.y = 8.0f;
	} else {
	    camera->position.y = 8.0f + (vehicle_get_speed(game->drive_vehicle) - 4.0f) * 1.5f;
	}
	camera->target = game->drive_vehicle->position;
	camera->fovy = 60.0f;
	UpdateCamera(camera, CAMERA_CUSTOM);

	if (IsKeyPressed(KEY_F)) {
	    game->game_mode = WALK;
	}
    }

    // 步行模式
    else if (game->game_mode == WALK) {
	player_update(player, delta_time);

	camera->position = player->position;
	camera->position.y = 8.0f;
	camera->target = player->position;
	camera->fovy = 45.0f;
	UpdateCamera(camera, CAMERA_CUSTOM);
	
	if (Vector3Distance(player->position, car->position) < 1.5f) {
	    if (IsKeyPressed(KEY_F)) {
		player->velocity = (Vector3){ 0 };
		game->drive_vehicle = car;
		game->game_mode = DRIVE;
	    }
	}

	if (Vector3Distance(player->position, truck->position) < 1.5f) {
	    if (IsKeyPressed(KEY_F)) {
		player->velocity = (Vector3){ 0 };
		game->drive_vehicle = truck;
		game->game_mode = DRIVE;
	    }
	}
    }
}

void game_render_3d(Game *game)
{
    DrawGrid(200, 1.0f);
    map_render(&game->map);

    vehicle_render(&game->car);
    vehicle_render(&game->truck);
    DrawLine3D(game->car.position, Vector3Add(game->car.position, Vector3Normalize(game->car.velocity)), RED);

    player_render(&game->player);
}

static void DrawMenu();
void game_render_ui(Game *game)
{
    DrawFPS(10, 10);
    
    if (game->game_mode == MENU) {
	DrawMenu();
    }

    else if (game->game_mode == DRIVE) {
	DrawText(TextFormat("km/h: %.f", vehicle_get_speed(&game->car) * 20.0f), 10, 30, 20, RED);
    }

    else if (game->game_mode == WALK) {
	if (Vector3Distance(game->player.position, game->car.position) < 1.5f) {
	    DrawText("Press F Enter Vehicle", 10, 50, 20, GREEN);
	}

	if (Vector3Distance(game->player.position, game->truck.position) < 1.5f) {
	    DrawText("Press F Enter Vehicle", 10, 50, 20, GREEN);
	}
    }
}

// UI
static void DrawMenu() {
    int screen_width = GetScreenWidth();
    int screen_height = GetScreenHeight();
    
    // 标题
    const char *title = "V CITY";
    int titleWidth = MeasureText(title, 100);
    DrawText(title, screen_width/2 - titleWidth/2, screen_height/4, 100, WHITE);
    
    // 副标题或装饰线
    DrawLine(screen_width/2 - 190, screen_height/4 + 110, 
             screen_width/2 + 190, screen_height/4 + 110, RAYWHITE);
    
    // 提示按 Enter
    const char *prompt = "PRESS ENTER TO START";
    int promptWidth = MeasureText(prompt, 24);
    DrawText(prompt, screen_width/2 - promptWidth/2, screen_height/2, 24, GREEN);
    
    // 闪烁效果（让提示文字闪烁）
    float time = GetTime();
    if ((int)(time * 2) % 2 == 0) {
        DrawText(prompt, screen_width/2 - promptWidth/2, screen_height/2, 24, YELLOW);
    } else {
        DrawText(prompt, screen_width/2 - promptWidth/2, screen_height/2, 24, GREEN);
    }
}
