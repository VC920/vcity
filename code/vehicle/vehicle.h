#ifndef VEHICLE_H
#define VEHICLE_H

#include <raylib.h>

typedef struct {
    Vector3 position;
    Vector3 rotation;
    Vector3 size;
    Color color;
    Vector3 velocity;

    float forward_speed;
    float turn_speed;

    float thrust_force;  // 动力
    float reverse_force; // 倒车动力
    float brake_force;   // 制动力
    float mass;          // 重量
    float drag;          // 空气阻力系数(0~1)
    float turn_angle;    // 转向角度
} Vehicle;

typedef struct {
    float thrust_force;  // 动力
    float reverse_force; // 倒车动力
    float brake_force;   // 制动力
    float mass;          // 重量
    float drag;          // 空气阻力系数(0~1)
    float turn_angle;    // 转向角度
} VehicleConfig;

// 车辆配置参数
static const VehicleConfig CAR_CONFIG = {
    .mass          = 1000.0f,
    .drag          = 0.995f,
    .thrust_force  = 6000.0f,
    .brake_force   = 6000.0f,
    .reverse_force = 1000.0f,
    .turn_angle    = 120.0f
};

static const VehicleConfig TRUCK_CONFIG = {
    .mass          = 2000.0f,
    .drag          = 0.995f,
    .thrust_force  = 5000.0f,
    .brake_force   = 5000.0f,
    .reverse_force = 800.0f,
    .turn_angle    = 60.0f
};

Vehicle vehicle_init(Vector3 position, Vector3 size, VehicleConfig config);

void vehicle_control(Vehicle *vehicle, float delta_time);

void vehicle_update(Vehicle *vehicle, float delta_time);

void vehicle_render(Vehicle *vehicle);

float vehicle_get_speed(Vehicle *vehicle);

#endif // VEHICLE_H
