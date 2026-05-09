#include "vehicle.h"
#include <raymath.h>
#include <rlgl.h>

Vehicle vehicle_init(Vector3 position, Vector3 size, VehicleConfig config)
{
    return (Vehicle) {
	.position = position,
	.size = size,
	.rotation = { 0 },
	.color = WHITE,
	.velocity = { 0 },
	.forward_speed = 0,
	.turn_speed = 0,
	.mass = config.mass,
	.drag = config.drag,
	.thrust_force = config.thrust_force,
	.brake_force = config.brake_force,
	.reverse_force = config.reverse_force,
	.turn_angle = config.turn_angle
    };
}

void vehicle_control(Vehicle *vehicle, float delta_time)
{
    // 油门
    if (IsKeyDown(KEY_W)) {
	// v = v0 + f / m * t;
	vehicle->forward_speed += vehicle->thrust_force / vehicle->mass * delta_time;
    }

    // 刹车
    if (IsKeyDown(KEY_S)) {
	float foot_brake_speed; 
	if (IsKeyDown(KEY_W)) {
	    foot_brake_speed = (vehicle->brake_force / vehicle->mass) * 1.0f * delta_time;
	} else {
	    foot_brake_speed = (vehicle->brake_force / vehicle->mass) * 0.1f * delta_time;	    
	}
	    
	if (vehicle->forward_speed > 0.0f) {
	    vehicle->forward_speed -= foot_brake_speed;
	} else {
	    vehicle->forward_speed -= vehicle->reverse_force / vehicle->mass * delta_time;
	}
    }

    // 转向
    vehicle->turn_speed = 0.0f;
    if (IsKeyDown(KEY_A)) {
	vehicle->turn_speed = -vehicle->turn_angle;
    }
    if (IsKeyDown(KEY_D)) {
	vehicle->turn_speed = vehicle->turn_angle;
    }

    // 手刹
    if (IsKeyDown(KEY_SPACE)) {
	float hand_brake_speed; 
	if (IsKeyDown(KEY_W)) {
	    hand_brake_speed = (vehicle->brake_force / vehicle->mass) * 1.0f * delta_time;
	} else {
	    hand_brake_speed = (vehicle->brake_force / vehicle->mass) * 0.1f * delta_time;	    
	}

	if (vehicle->forward_speed > 0.0f) {
	    vehicle->forward_speed -= hand_brake_speed;
	} else {
	    vehicle->forward_speed += hand_brake_speed;
	}
    }

   
    bool can_turn = fabs(vehicle->forward_speed) > 0.1f || (IsKeyDown(KEY_W) && IsKeyDown(KEY_SPACE));
    
    if (can_turn) {
	// 倒车反向
	if (vehicle->forward_speed < 0 ) vehicle->turn_speed *= -1;

	if (IsKeyDown(KEY_W)) {
	    vehicle->rotation.y += vehicle->turn_speed * delta_time;
	} else {
	    vehicle->rotation.y += vehicle->turn_speed * 0.5 * delta_time;
	}
	
	if (vehicle->rotation.y >= 360.0f) vehicle->rotation.y -= 360.0f;
	if (vehicle->rotation.y < 0) vehicle->rotation.y += 360.0f;
    }
}

void vehicle_update(Vehicle *vehicle, float delta_time)
{
    // 空气阻力
    vehicle->forward_speed = vehicle->forward_speed * vehicle->drag;
    
    // 计算速度向量
    float rad = vehicle->rotation.y * DEG2RAD;
    vehicle->velocity.z = -vehicle->forward_speed * cosf(rad);
    vehicle->velocity.x = vehicle->forward_speed * sinf(rad);

    // 位移
    if (Vector3Length(vehicle->velocity) > 0.1f) {
	// s = s0 + v * t
	vehicle->position = Vector3Add(vehicle->position,
				       Vector3Scale(vehicle->velocity, delta_time));
    }
}

void vehicle_render(Vehicle *vehicle)
{
    Vector3 pos = { vehicle->position.x, vehicle->size.y * 0.5f, vehicle->position.z };

    rlPushMatrix();
    rlTranslatef(pos.x, pos.y, pos.z);

    float yaw = -vehicle->rotation.y;
    rlRotatef(yaw, 0.0f, 1.0f, 0.0f);

    DrawCubeV((Vector3){ 0.0f, 0.0f, 0.0f }, vehicle->size, vehicle->color);
    DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, vehicle->size.x * 0.5f, vehicle->size.y, vehicle->size.z, WHITE);

    rlPopMatrix();
}

float vehicle_get_speed(Vehicle *vehicle)
{
    return vehicle->forward_speed;
}
