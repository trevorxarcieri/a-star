#ifndef ASTAR_TILE_MAP_H
#define ASTAR_TILE_MAP_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h> // Include for random seed initialization

#define MAP_SIZE 10 // Define the size of the map as 10x10

// Structure to represent each tile on the map
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

// Structure to represent the entire map containing tiles
typedef struct {
    AStarTile map[MAP_SIZE][MAP_SIZE]; // 2D array of tiles
    int closedSet[MAP_SIZE][MAP_SIZE]; // Array to track the closed set of nodes
    int evaluated[MAP_SIZE][MAP_SIZE]; // Array to track evaluated nodes
    double (*distanceFunction)(int, int, int, int); // Pointer to the distance function used as the heuristic
    int srcX, srcY; // Coordinates of the source tile
    int destX, destY; // Coordinates of the destination tile
} AStarMap;

AStarMap globalMap; // Global instance of the map

// Initializes the map with source and destination coordinates and sets the heuristic
void mapInit(int srcX, int srcY, int destX, int destY, double (*distanceFunc)(int, int, int, int)) {
    globalMap.srcX = srcX; // Set source x-coordinate
    globalMap.srcY = srcY; // Set source y-coordinate
    globalMap.destX = destX; // Set destination x-coordinate
    globalMap.destY = destY; // Set destination y-coordinate
    globalMap.distanceFunction = distanceFunc; // Assign the distance function as the heuristic
    srand(time(NULL)); // Seed the random number generator for obstacle placement
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            globalMap.map[i][j].x = i;
            globalMap.map[i][j].y = j;
            globalMap.map[i][j].isObstacle = (rand() % 10 == 0) && !(i == srcX && j == srcY) && !(i == destX && j == destY); // Ensure source/dest are not obstacles, 10% chance of obstacle
            globalMap.map[i][j].C = globalMap.map[i][j].isObstacle ? 0 : (rand() % 9 + 1); // Assign random cost 1-9 for non-obstacle tiles, otherwise 0
            globalMap.map[i][j].G = 0;
            globalMap.map[i][j].H = 0;
            globalMap.map[i][j].F = 0;
            globalMap.map[i][j].inHeap = 0;
            globalMap.map[i][j].parent = NULL;
        }
    }
}

// Retrieves a tile based on x, y coordinates from the map
AStarTile* getTile(int x, int y) {
    return &globalMap.map[x][y]; // Return the address of the tile at coordinates (x, y)
}

// Prints the map in different modes based on the specified parameter.
// Mode 0: Simple representation with symbols for source, destination, obstacles, evaluated and unevaluated tiles.
// Mode 1: Detailed display showing the F-costs for tiles that have been evaluated.
void mapPrint(int mode) {
    // Array to store path
    int path[MAP_SIZE][MAP_SIZE] = {0};

    // Tracing back the path from the destination to the source
    AStarTile* tile = &globalMap.map[globalMap.destX][globalMap.destY];
    while (tile != NULL && tile->parent != NULL) {
        path[tile->x][tile->y] = 1;  // Mark this tile as evaluated
        tile = tile->parent;  // Move to the parent tile in the path
    }

    // Iterate over each row of the map
    for (int i = 0; i < MAP_SIZE; i++) {
        // Iterate over each column in the current row
        for (int j = 0; j < MAP_SIZE; j++) {
            AStarTile* currentTile = &globalMap.map[i][j]; // Access the tile at position (i, j)

            // Mode 0: Visual representation with symbols
            if (mode == 0) {
                // Print source tile with 'S'
                if (i == globalMap.srcX && j == globalMap.srcY) printf(" S ");
                // Print destination tile with 'D'
                else if (i == globalMap.destX && j == globalMap.destY) printf(" D ");
                // Print obstacle tiles with 'X'
                else if (currentTile->isObstacle) printf(" X ");
                // Print tiles not evaluated by the algorithm with '?'
                else if (globalMap.evaluated[i][j] == 0) printf(" ? ");
                // Print path tiles (part of the final path) with '*'
                else if (path[i][j] == 1) printf(" * ");
                // Print all other tiles with '_'
                else printf(" _ ");
            }
            // Mode 1: Detailed representation with F-cost values
            else if (mode == 1) {
                // Print obstacles distinctly with 'X'
                if (currentTile->isObstacle) printf("   X   ");
                // Print evaluated non-obstacle tiles with their F-cost
                else if (globalMap.evaluated[i][j] == 1) printf("%6.2f ", currentTile->F);
                // Print unevaluated tiles with '?'
                else printf("   ?   ");
            }
        }
        // End each row with a newline character
        printf("\n");
    }
}

#endif // ASTAR_TILE_MAP_H
