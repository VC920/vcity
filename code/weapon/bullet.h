#ifndef BULLET_H
#define BULLET_H

#include <raylib.h>
#include <raymath.h>

typedef struct {
    Vector3 position;
    Vector3 velocity;
    float radius;
    float damage;
} Bullet;

void bullet_update(Bullet *bullet, float delta_time);

#endif // BULLET_H
