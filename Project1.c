#include <stdio.h>
#include <math.h>
#include "AStarAlgorithm.h"
#include "AStarTileMap.h"
#include "AStarTileHeap.h"
#include "AStarTileStack.h"

// Distance function for Euclidean distance
double euclideanDistance(int ax, int ay, int bx, int by) {
    return sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
}

// Distance function for Manhattan distance
double manhattanDistance(int ax, int ay, int bx, int by) {
    return abs(ax - bx) + abs(ay - by);
}

int main() {
    int sourceX = 0, sourceY = 0;   // Source coordinates
    int destX = 4, destY = 4;       // Destination coordinates

    // Initialize the map with source and destination coordinates and a distance function
    mapInit(sourceX, sourceY, destX, destY, manhattanDistance);

    // A* Search
    AStarTile* dest = aStarRunSearch();

    // Printing results
    printf("A* Path:\n");
    mapPrint(0); // Mode 0 for basic map print

    printf("\nA* Costs:\n");
    mapPrint(1); // Mode 1 for detailed map print with F values

    printf("\nFinal Path from source to destination:\n");
    aStarTracePath(dest); // Print the path from the source to the destination

    return 0;
}
