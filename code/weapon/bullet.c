#include "bullet.h"

void bullet_update(Bullet *bullet, float delta_time)
{
    // s = s0 + v * t
    bullet->position = Vector3Add(bullet->position, Vector3Scale(bullet->velocity, delta_time));
}
