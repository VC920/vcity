#ifndef MAP_H
#define MAP_H

#define MAP_WIDTH  60
#define MAP_HEIGHT 40

typedef struct {
    int width;
    int height;
    int data[MAP_WIDTH][MAP_HEIGHT];
} Map;

Map map_init(const char *path);

void map_render(Map *map);

#endif // MAP_H
