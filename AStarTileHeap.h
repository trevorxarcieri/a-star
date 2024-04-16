#ifndef ASTAR_TILE_HEAP_H
#define ASTAR_TILE_HEAP_H

#include "AStarTileMap.h" // Includes the definition of AStarTile

#define HEAP_SIZE (MAP_SIZE * MAP_SIZE) // Maximum size of the heap

AStarTile* heap[HEAP_SIZE]; // The heap array
int heapCount = 0; // Number of elements in the heap

// Function to swap two tiles in the heap
void swapTiles(int index1, int index2) {
    AStarTile* temp = heap[index1];
    heap[index1] = heap[index2];
    heap[index2] = temp;
}

// Function to maintain the heap property from a given index upwards
void heapifyUp(int index) {
    while (index > 0 && heap[(index - 1) / 2]->F > heap[index]->F) {
        swapTiles(index, (index - 1) / 2);
        index = (index - 1) / 2;
    }
}

// Function to insert a tile into the heap
void heapInsert(AStarTile* tile) {
    if (heapCount < HEAP_SIZE) {
        heap[heapCount] = tile;
        heapifyUp(heapCount);
        heapCount++;
    }
}

// Function to maintain the heap property from a given index downwards
void heapifyDown(int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heapCount && heap[left]->F < heap[smallest]->F) {
        smallest = left;
    }
    if (right < heapCount && heap[right]->F < heap[smallest]->F) {
        smallest = right;
    }
    if (smallest != index) {
        swapTiles(index, smallest);
        heapifyDown(smallest);
    }
}

// Function to extract the minimum element from the heap
AStarTile* heapExtractMin() {
    if (heapCount == 0) return NULL;
    AStarTile* minTile = heap[0];
    heap[0] = heap[heapCount - 1];
    heapCount--;
    heapifyDown(0);
    return minTile;
}

// Function to decrease key for a particular tile in the heap
void heapDecreaseKey(AStarTile* tile) {
    // Find the tile in the heap
    int index = 0;
    for (index = 0; index < heapCount; index++) {
        if (heap[index] == tile) break;
    }
    if (index < heapCount) {
        heapifyUp(index);
    }
}

// Function to initialize the heap
void heapInit() {
    heapCount = 0; // Resets the heap count
}

// Function to check if the heap is empty
int heapIsEmpty() {
    return heapCount == 0;
}

#endif // ASTAR_TILE_HEAP_H
