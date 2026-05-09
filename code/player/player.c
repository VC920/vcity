#include "player.h"

#include <raymath.h>
#include <rlgl.h>

Player player_init(Vector3 position, Vector3 size)
{
    return (Player) {
	.position = position,
	.rotation = { 0 },
	.size = size,
	.velocity = { 0 },
	.color = GREEN,
	.max_speed = 1
    };
}

void player_update(Player *player, float delta_time)
{
    Vector2 input = { 0 };
    if (IsKeyDown(KEY_W)) input.y = 1.0f;
    
    if (IsKeyDown(KEY_S)) input.y = -1.0f;
    
    if (IsKeyDown(KEY_A)) input.x = -1.0f;
    
    if (IsKeyDown(KEY_D)) input.x = 1.0f;
    
    if (Vector2Length(input) > 0.1f) {
	input = Vector2Normalize(input);
    }

    // 位移
    player->velocity.x = input.x * player->max_speed;
    player->velocity.z = -input.y * player->max_speed;

    // s = s0 + v * t
    player->position = Vector3Add(player->position,
				  Vector3Scale(player->velocity, delta_time));

    // 旋转
    Vector3 target_rotation = player->rotation;
    if (fabsf(player->velocity.x) > 0.1f || fabsf(player->velocity.z) > 0.1f) {
	float yaw = atan2f(player->velocity.x, player->velocity.z) * RAD2DEG;
	target_rotation.y = yaw;
    }

    // 计算角度差值
    float angle_diff = target_rotation.y - player->rotation.y;
    if (angle_diff > 180.0f) angle_diff -= 360.0f;
    if (angle_diff < -180.0f) angle_diff += 360.0f;
    
    // 计算插值
    float lerp_factor = 20.0f * delta_time;
    if (lerp_factor > 1.0f) lerp_factor = 1.0f;
    player->rotation.y += angle_diff * lerp_factor;
    
    // 限制在0～360
    if (player->rotation.y >= 360.0f) player->rotation.y -= 360.0f;
    if (player->rotation.y < 0.0f) player->rotation.y += 360.0f;
}

void player_render(Player *player)
{
    Vector3 pos = { player->position.x, player->size.y * 0.5f, player->position.z };

    rlPushMatrix();
    rlTranslatef(pos.x, pos.y, pos.z);

    float yaw = player->rotation.y;
    rlRotatef(yaw, 0.0f, 1.0f, 0.0f);

    DrawCubeV((Vector3){ 0.0f, 0.0f, 0.0f }, player->size, player->color);

    rlPopMatrix();
}
