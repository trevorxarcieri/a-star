#ifndef ASTAR_ALGORITHM_H
#define ASTAR_ALGORITHM_H

#include "AStarTileMap.h"
#include "AStarTileHeap.h"
#include "AStarTileStack.h"

// Function to build the path from destination to source and print it in forward order
void aStarTracePath(AStarTile *goal) {
    if(goal == NULL) {
        printf("No path found.\n");
        return;
    }
    AStarTile *current = goal;
    // Using a dynamically allocated array to simulate a stack for simplicity.
    // You could use a linked list or another stack structure as needed.
    AStarTile **stack = malloc(sizeof(AStarTile*) * MAP_SIZE * MAP_SIZE); // Arbitrary size, ensure it's sufficient for the path
    int top = 0;

    // Trace the path backwards and push the tiles onto the stack
    while (current) {
        stack[top++] = current;
        current = current->parent;
    }

    // Pop the tiles from the stack to print the path in forward order
    while (top > 0) {
        current = stack[--top];
        printf("(%d, %d)", current->x, current->y);
        if (top > 0)
        {
            printf("->");
        }
    }
    printf("\n");

    // Free the allocated stack
    free(stack);
}

// Function to process a neighboring tile (updates or adds to the open set)
void aStarProcessNeighborTile(AStarTile *current, AStarTile *neighbor, AStarTile *goal) {
    if (neighbor->isObstacle || globalMap.closedSet[neighbor->x][neighbor->y]) {
        return; // Skip if neighbor is an obstacle or already visited
    }
    int tentativeG = current->G + neighbor->C;
    if (tentativeG < neighbor->G || neighbor->parent == NULL) {
        neighbor->parent = current;
        neighbor->G = tentativeG;
        neighbor->H = globalMap.distanceFunction(neighbor->x, neighbor->y, goal->x, goal->y);
        neighbor->F = neighbor->G + neighbor->H;
        globalMap.evaluated[neighbor->x][neighbor->y] = 1; // Mark the neighbor as evaluated
        if (!neighbor->inHeap) {
            heapInsert(neighbor);
            neighbor->inHeap = 1;
        } else {
            heapDecreaseKey(neighbor);
        }
    }
}

// Function to execute A* search
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
    globalMap.closedSet[start->x][start->y] = 1; // Mark the start as visited
    globalMap.evaluated[start->x][start->y] = 1; // Mark the start as evaluated

    while (!heapIsEmpty()) {
        AStarTile *current = heapExtractMin();
        current->inHeap = 0;

        if (current == goal) {
            return goal;
        }

        const int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
        for (int i = 0; i < 4; i++) {
            int nx = current->x + directions[i][0];
            int ny = current->y + directions[i][1];
            if (nx >= 0 && nx < MAP_SIZE && ny >= 0 && ny < MAP_SIZE) {
                AStarTile *neighbor = getTile(nx, ny);
                aStarProcessNeighborTile(current, neighbor, goal);
            }
        }

        globalMap.closedSet[current->x][current->y] = 1; // Mark the current as visited
    }
    return NULL;
}

#endif // ASTAR_ALGORITHM_H
