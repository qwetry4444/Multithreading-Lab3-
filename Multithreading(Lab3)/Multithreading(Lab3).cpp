#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

const int ARRAY_SIZE = 10000;

void fillArray(int arr[], int size) {
    srand(time(nullptr));
    for (int i = 0; i < size; ++i) {
        arr[i] = rand() % 1000;
    }
}


void bubbleSort(int arr[], int size) {
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pivotIndex = partition(arr, low, high);

        quickSort(arr, low, pivotIndex - 1);
        quickSort(arr, pivotIndex + 1, high);
    }
}


int main()
{
    int array[ARRAY_SIZE];
    fillArray(array, ARRAY_SIZE);

    int bubbleSortArray[ARRAY_SIZE];
    int quickSortArray[ARRAY_SIZE];

    copy(begin(array), end(array), begin(bubbleSortArray));
    copy(begin(array), end(array), begin(quickSortArray));

    std::thread bubbleSortThread(bubbleSort, bubbleSortArray, ARRAY_SIZE);
    std::thread quickSortThread(quickSort, quickSortArray, 0, ARRAY_SIZE - 1);

    bubbleSortThread.join();
    quickSortThread.join();

    bubbleSortThread.
        

}
