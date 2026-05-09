#include "map.h"
#include <stdio.h>
#include <raylib.h>

Map map_init(const char *path)
{
    Map map;
    map.width = MAP_WIDTH;
    map.height = MAP_HEIGHT;

    for (int i = 0; i < map.width; i++) {
	for (int j = 0; j < map.height; j++) {
	    map.data[i][j] = 0;
	}
    }

    FILE *file = fopen(path, "r");
    if (file == NULL) {
	fprintf(stderr, "fopen Error: %s", path);
	return map;
    }

    for (int j = 0; j < map.height; j++) {
	for (int i = 0; i < map.width; i++) {
	    if (fscanf(file, "%d", &map.data[i][j]) != 1) {
		fprintf(stderr, "load map Error: %s", path);
		fclose(file);
		return map;
	    }
	}
    }

    fclose(file);
    return map;
}

void map_render(Map *map)
{
    for (int i = 0; i < map->width; i++) {
	for (int j = 0; j < map->height; j++) {
	    if (map->data[i][j] == -1) {
		Vector3 pos = { i, 0.0f, j};
		DrawPlane(pos, (Vector2){ 1.0f, 1.0f }, BLACK);
	    }
	    
	    if (map->data[i][j] > 0) {
		Vector3 pos = { i, map->data[i][j] * 0.5f, j };
		Vector3 size = { 1.0f, (float)map->data[i][j], 1.0f };
		DrawCubeV(pos, size, GRAY);
		DrawCubeWiresV(pos, size, BLACK);
	    }
	}
    }
}
