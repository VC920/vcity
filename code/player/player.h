#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

typedef struct {
    Vector3 position;
    Vector3 rotation;
    Vector3 size;
    Color color;

    Vector3 velocity;
    float max_speed;
} Player;

Player player_init(Vector3 position, Vector3 size);

void player_update(Player *player, float delta_time);

void player_render(Player *player);

#endif // PLAYER_H
