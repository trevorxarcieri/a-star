#ifndef ASTAR_TILE_MAP_H
#define ASTAR_TILE_MAP_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h> // For random seed

#define MAP_SIZE 5 // Assuming a 10x10 map

typedef struct AStarTile {
    int x, y; // Tile coordinates
    struct AStarTile* parent; // Pointer to parent tile in path
    int G; // Cost from start to this tile
    int C; // Cost of passing through this tile
    double H; // Heuristic estimate to the goal
    double F; // Total cost (F = G + C + H)
    int inHeap; // Flag to check if this tile is in the heap
    int isObstacle; // Flag to check if the tile is an obstacle
} AStarTile;

typedef struct {
    AStarTile map[MAP_SIZE][MAP_SIZE];
    int closedSet[MAP_SIZE][MAP_SIZE]; // Closed set included in the map structure
    int evaluated[MAP_SIZE][MAP_SIZE]; // Visited set included in the map structure
    double (*distanceFunction)(int, int, int, int);
    int srcX, srcY;
    int destX, destY;
} AStarMap;

AStarMap globalMap;

void mapInit(int srcX, int srcY, int destX, int destY, double (*distanceFunc)(int, int, int, int)) {
    globalMap.srcX = srcX;
    globalMap.srcY = srcY;
    globalMap.destX = destX;
    globalMap.destY = destY;
    globalMap.distanceFunction = distanceFunc;
    srand(time(NULL)); // Seed for random number generation
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            globalMap.map[i][j].x = i;
            globalMap.map[i][j].y = j;
            globalMap.map[i][j].isObstacle = (rand() % 10 == 0) && !(i == srcX && j == srcY) && !(i == destX && j == destY); // 10% chance, but not for src/dest
            globalMap.map[i][j].C = globalMap.map[i][j].isObstacle ? 0 : (rand() % 9 + 1); // Costs between 1 and 9, 0 if obstacle
            globalMap.map[i][j].G = 0;
            globalMap.map[i][j].H = 0;
            globalMap.map[i][j].F = 0;
            globalMap.map[i][j].inHeap = 0;
            globalMap.map[i][j].parent = NULL;
        }
    }
}

AStarTile* getTile(int x, int y) {
    return &globalMap.map[x][y];
}

void mapPrint(int mode) {
    // Array to store path
    int path[MAP_SIZE][MAP_SIZE] = {0};

    // Optionally tracing back the path from the destination to the source if necessary
    AStarTile* tile = &globalMap.map[globalMap.destX][globalMap.destY];
    while (tile != NULL && tile->parent != NULL) {
        path[tile->x][tile->y] = 1;  // Mark this tile as evaluated
        tile = tile->parent;  // Move to the parent tile in the path
    }

    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            AStarTile* currentTile = &globalMap.map[i][j];
            if (mode == 0) {
                if (i == globalMap.srcX && j == globalMap.srcY) {
                    printf(" S ");  // Source tile
                } else if (i == globalMap.destX && j == globalMap.destY) {
                    printf(" D ");  // Destination tile
                } else if (currentTile->isObstacle) {
                    printf(" X ");  // Obstacle
                } else if (globalMap.evaluated[i][j] == 0) {
                    printf(" ? ");  // Not evaluated by A*
                } else if (path[i][j] == 1) {
                    printf(" * ");  // Path from source to destination
                } else {
                    printf(" _ ");  // Other evaluated tile
                }
            } else if (mode == 1) {
                if (currentTile->isObstacle) {
                    printf("   X   ");
                } else if (globalMap.evaluated[i][j] == 1) {
                    printf("%6.2f ", currentTile->F);  // Display F value of evaluated tiles
                } else {
                    printf("   ?   ");
                }
            }
        }
        printf("\n");
    }
}


#endif // ASTAR_TILE_MAP_H
