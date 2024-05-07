#ifndef ASTAR_ALGORITHM_H
#define ASTAR_ALGORITHM_H

#include "AStarTileMap.h"
#include "AStarTileHeap.h"
#include "AStarTileStack.h"

// Traces and prints the path from the destination to the source in forward order.
void aStarTracePath(AStarTile *goal) {
    if (goal == NULL) {
        printf("No path found.\n");  // Handle case where no path exists
        return;
    }
    AStarTile *current = goal;
    AStarTile **stack = malloc(sizeof(AStarTile*) * MAP_SIZE * MAP_SIZE); // Allocate stack to reverse the path
    int top = 0;

    // Trace back from goal to source using parent pointers, pushing nodes onto the stack
    while (current) {
        stack[top++] = current;
        current = current->parent;
    }

    // Print the path in forward order by popping from the stack
    while (top > 0) {
        current = stack[--top];
        printf("(%d, %d)", current->x, current->y);
        if (top > 0) {
            printf("->");  // Print arrow unless it's the last node
        }
    }
    printf("\n");
    free(stack);  // Clean up the dynamically allocated stack
}

// Processes a neighboring tile during A* search, updating or adding it to the heap if necessary
void aStarProcessNeighborTile(AStarTile *current, AStarTile *neighbor, AStarTile *goal) {
    if (neighbor->isObstacle || globalMap.closedSet[neighbor->x][neighbor->y]) {
        return; // Skip obstacles and tiles already in the closed set
    }
    int tentativeG = current->G + neighbor->C;
    // Update tile if a cheaper path is found or if it has not been evaluated
    if (tentativeG < neighbor->G || neighbor->parent == NULL) {
        neighbor->parent = current;
        neighbor->G = tentativeG;
        neighbor->H = globalMap.distanceFunction(neighbor->x, neighbor->y, goal->x, goal->y);
        neighbor->F = neighbor->G + neighbor->H;
        globalMap.evaluated[neighbor->x][neighbor->y] = 1; // Mark as evaluated
        if (!neighbor->inHeap) {
            heapInsert(neighbor); // Add new tile to the heap
            neighbor->inHeap = 1;
        } else {
            heapDecreaseKey(neighbor); // Update position in the heap
        }
    }
}

// Executes the A* search algorithm starting from the source tile
AStarTile *aStarRunSearch() {
    AStarTile *start = getTile(globalMap.srcX, globalMap.srcY);
    AStarTile *goal = getTile(globalMap.destX, globalMap.destY);
    start->G = 0;
    start->H = globalMap.distanceFunction(start->x, start->y, goal->x, goal->y);
    start->F = start->G + start->H;
    start->parent = NULL;
    start->inHeap = 1;

    heapInit();
    heapInsert(start);
    globalMap.closedSet[start->x][start->y] = 1; // Mark start as visited
    globalMap.evaluated[start->x][start->y] = 1; // Mark start as evaluated

    while (!heapIsEmpty()) {
        AStarTile *current = heapExtractMin(); // Get the tile with the lowest F value
        current->inHeap = 0;

        if (current == goal) {  // If goal has been reached, return it
            return goal;
        }

        // Explore all four possible directions from current tile
        const int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
        for (int i = 0; i < 4; i++) {
            int nx = current->x + directions[i][0];
            int ny = current->y + directions[i][1];
            if (nx >= 0 && nx < MAP_SIZE && ny >= 0 && ny < MAP_SIZE) {
                AStarTile *neighbor = getTile(nx, ny);
                aStarProcessNeighborTile(current, neighbor, goal);
            }
        }

        globalMap.closedSet[current->x][current->y] = 1; // Mark current as visited
    }
    return NULL; // Return NULL if no path is found
}

#endif // ASTAR_ALGORITHM_H
