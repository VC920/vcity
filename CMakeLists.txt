
cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(vcity)

add_executable(${PROJECT_NAME}
  code/game/main.c
  code/game/game.c
  code/map/map.c
  code/vehicle/vehicle.c
  code/player/player.c
  code/weapon/bullet.c
 )

target_include_directories(${PROJECT_NAME} PRIVATE
  code/game
  code/map
  code/vehicle
  code/player
  code/weapon
  
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  raylib
  m
)

