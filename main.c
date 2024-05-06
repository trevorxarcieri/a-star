#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "AStarAlgorithm.h"
#include "AStarTileMap.h"
#include "AStarTileHeap.h"
#include "AStarTileStack.h"

// Calculate the Euclidean distance between two points (ax, ay) and (bx, by)
double euclideanDistance(int ax, int ay, int bx, int by) {
    // Calculate square root of sum of squared differences in x and y coordinates
    return sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
}

// Calculate the Manhattan distance between two points, ideal for grid-based pathfinding
double manhattanDistance(int ax, int ay, int bx, int by) {
    // Sum of the absolute differences in x and y coordinates
    return abs(ax - bx) + abs(ay - by);
}

// Generate random distinct start and end coordinates within the map bounds
void generateRandomCoordinates(int* srcX, int* srcY, int* destX, int* destY, int mapSize) {
    srand(time(NULL)); // Seed the random number generator
    *srcX = rand() % mapSize;
    *srcY = rand() % mapSize;
    do {
        *destX = rand() % mapSize;
        *destY = rand() % mapSize;
    } while (*destX == *srcX && *destY == *srcY); // Ensure destination is different from source
}

int main() {
    int sourceX, sourceY;   // Source coordinates
    int destX, destY;       // Destination coordinates

    // Generate random source and destination coordinates on a 10x10 map
    generateRandomCoordinates(&sourceX, &sourceY, &destX, &destY, MAP_SIZE);

    // Initialize the map and set up the A* search with the Euclidean distance as the heuristic
    mapInit(sourceX, sourceY, destX, destY, euclideanDistance);

    // Run the A* search algorithm and retrieve the destination tile
    AStarTile* dest = aStarRunSearch();

    // Output the basic visualization of the map after the search
    printf("A* Path:\n");
    mapPrint(0); // Mode 0: Display path with basic symbols

    // Print the detailed visualization showing costs associated with each tile
    printf("\nA* Costs:\n");
    mapPrint(1); // Mode 1: Display map with F values

    // Display the final path from the source to the destination tile
    printf("\nFinal Path from source to destination:\n");
    aStarTracePath(dest); // Trace and print the path using parent pointers

    return 0;
}
