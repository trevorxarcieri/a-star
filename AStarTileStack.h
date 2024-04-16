#ifndef ASTAR_TILE_STACK_H
#define ASTAR_TILE_STACK_H

#include "AStarTileMap.h" // Includes the tile definition

typedef struct StackNode {
    AStarTile* tile;
    struct StackNode* next;
} StackNode;

StackNode* top = NULL; // Top of the stack

// Function to push a tile onto the stack
void stackPush(AStarTile* tile) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    if (newNode != NULL) {
        newNode->tile = tile;
        newNode->next = top;
        top = newNode;
    }
}

// Function to pop the top tile from the stack
AStarTile* stackPop() {
    if (top == NULL) return NULL;
    StackNode* tempNode = top;
    AStarTile* tile = top->tile;
    top = top->next;
    free(tempNode);
    return tile;
}

// Function to print the stack (for debugging)
void printPath() {
    StackNode* current = top;
    while (current != NULL) {
        printf("(%d, %d) -> ", current->tile->x, current->tile->y);
        current = current->next;
    }
    printf("End\n");
}

#endif // ASTAR_TILE_STACK_H
