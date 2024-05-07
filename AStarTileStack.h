#ifndef ASTAR_TILE_STACK_H
#define ASTAR_TILE_STACK_H

#include "AStarTileMap.h" // Include the tile definition for access to AStarTile structures

// Structure to represent a node in a stack
typedef struct StackNode {
    AStarTile* tile;       // Pointer to the AStarTile stored in this node
    struct StackNode* next; // Pointer to the next node in the stack
} StackNode;

StackNode* top = NULL; // Initialize the top of the stack to NULL

// Pushes a tile onto the stack
void stackPush(AStarTile* tile) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode)); // Dynamically allocate a new node
    if (newNode != NULL) { // Ensure newNode was allocated successfully
        newNode->tile = tile; // Set the tile of the new node
        newNode->next = top; // Link the new node with the current top of the stack
        top = newNode; // Update top to point to the new node
    }
}

// Pops the top tile from the stack and returns it
AStarTile* stackPop() {
    if (top == NULL) return NULL; // Return NULL if the stack is empty
    StackNode* tempNode = top; // Temporary node to hold the top before it's removed
    AStarTile* tile = top->tile; // Retrieve the tile from the top node
    top = top->next; // Move the top pointer to the next node
    free(tempNode); // Free the memory of the old top node
    return tile; // Return the tile
}

// Prints all tiles in the stack from top to bottom for debugging purposes
void printPath() {
    StackNode* current = top; // Start from the top of the stack
    while (current != NULL) { // Traverse until the end of the stack
        printf("(%d, %d) -> ", current->tile->x, current->tile->y); // Print the current tile's coordinates
        current = current->next; // Move to the next node
    }
    printf("End\n"); // Indicate the end of the path output
}

#endif // ASTAR_TILE_STACK_H
