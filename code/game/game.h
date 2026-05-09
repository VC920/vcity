
#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include "map.h"
#include "vehicle.h"
#include "player.h"

typedef enum {
    MENU,
    EDITOR,
    DRIVE,
    WALK
} GameMode;

typedef struct {
    GameMode game_mode;
    
    Music bgm;
    Camera3D camera;
    Map map;
    Vehicle *drive_vehicle;
    Vehicle car;
    Vehicle truck;
    Player player;
} Game;

Game game_init();

void game_update(Game *game, float delta_time);

void game_render_3d(Game *game);

void game_render_ui(Game *game);

#endif // GAME_H
