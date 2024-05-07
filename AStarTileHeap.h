#ifndef ASTAR_TILE_HEAP_H
#define ASTAR_TILE_HEAP_H

#include "AStarTileMap.h" // Include the definition of AStarTile for heap operations

#define HEAP_SIZE (MAP_SIZE * MAP_SIZE) // Defines the maximum size of the heap based on the map size

AStarTile* heap[HEAP_SIZE]; // Array to store the heap elements
int heapCount = 0; // Tracks the current number of elements in the heap

// Swaps two tiles in the heap based on their indices
void swapTiles(int index1, int index2) {
    AStarTile* temp = heap[index1];
    heap[index1] = heap[index2];
    heap[index2] = temp;
}

// Restores the heap property upwards from the given index
void heapifyUp(int index) {
    // Continue swapping the current node with its parent if the parent's F value is greater
    while (index > 0 && heap[(index - 1) / 2]->F > heap[index]->F) {
        swapTiles(index, (index - 1) / 2);
        index = (index - 1) / 2; // Move to the parent index
    }
}

// Inserts a new tile into the heap
void heapInsert(AStarTile* tile) {
    // Ensure there is space in the heap before inserting
    if (heapCount < HEAP_SIZE) {
        heap[heapCount] = tile; // Place the new tile at the end of the heap
        heapifyUp(heapCount); // Adjust heap from the end upwards
        heapCount++; // Increment the number of elements in the heap
    }
}

// Restores the heap property downwards from the given index
void heapifyDown(int index) {
    int smallest = index;
    int left = 2 * index + 1; // Left child index
    int right = 2 * index + 2; // Right child index

    // Find the smallest of the node and its children
    if (left < heapCount && heap[left]->F < heap[smallest]->F) {
        smallest = left;
    }
    if (right < heapCount && heap[right]->F < heap[smallest]->F) {
        smallest = right;
    }

    // If the smallest is not the current node, swap and continue
    if (smallest != index) {
        swapTiles(index, smallest);
        heapifyDown(smallest); // Recursively adjust downwards
    }
}

// Extracts the minimum element from the heap
AStarTile* heapExtractMin() {
    if (heapCount == 0) return NULL; // Return NULL if the heap is empty
    AStarTile* minTile = heap[0]; // The root of the heap is the minimum
    heap[0] = heap[--heapCount]; // Replace root with the last element and reduce size
    heapifyDown(0); // Re-heapify from the root downwards
    return minTile;
}

// Decreases the F value of a specific tile and re-adjusts the heap
void heapDecreaseKey(AStarTile* tile) {
    // Find the position of the tile in the heap
    int index = 0;
    for (index = 0; index < heapCount; index++) {
        if (heap[index] == tile) break;
    }
    // If found within the heap, adjust its position upwards as the key is decreased
    if (index < heapCount) {
        heapifyUp(index);
    }
}

// Initializes or clears the heap
void heapInit() {
    heapCount = 0; // Reset the number of elements in the heap to zero
}

// Checks if the heap is empty
int heapIsEmpty() {
    return heapCount == 0; // Return true if the heap count is zero
}

#endif // ASTAR_TILE_HEAP_H
