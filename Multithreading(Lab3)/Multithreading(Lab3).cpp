#include <iostream>
#include <Windows.h>
#include <thread>
#include <vector>
#include <chrono>
#include <windef.h>
#include <processthreadsapi.h>

using namespace std;

const int ARRAY_SIZE = 10000;

void fillArray(vector<int> arr, int size) {
    srand(time(nullptr));
    for (int i = 0; i < size; ++i) {
        arr.push_back(rand() % 1000);
    }
}

struct bubbleSortData
{
    vector<int> arr;
    int size;
};

struct quickSortData
{
    vector<int> arr;
    int low;
    int high;
};


DWORD WINAPI bubbleSort(LPVOID data) {
    for (int i = 0; i < data.size - 1; ++i) {
        for (int j = 0; j < data.size - i - 1; ++j) {
            if (data.arr[j] > data.arr[j + 1]) {
                swap(data.arr[j], data.arr[j + 1]);
            }
        }
    }
}

int partition(vector<int> arr, int low, int high) {
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

DWORD WINAPI quickSort(LPVOID data) {
    if (data.low < data.high) {
        int pivotIndex = partition(data.arr, data.low, data.high);

        quickSortData newData;

        newData.low = data.low;
        newData.high = pivotIndex - 1;
        quickSort(newData);

        newData.low = pivotIndex + 1;
        newData.high = data.high;
        quickSort(newData);
    }
}

DWORD WINAPI func(LPVOID x)
{
    return 1;
}


int main()
{
  
    vector<int> array;
    fillArray(array, ARRAY_SIZE);

    vector<int> bubbleSortArray = array;
    vector<int> quickSortArray = array;

    struct bubbleSortData bubbleData;
    bubbleData.arr = bubbleSortArray;
    bubbleData.size = ARRAY_SIZE;

    struct quickSortData quickData;
    quickData.arr = quickSortArray;
    quickData.low = 0;
    quickData.high = ARRAY_SIZE - 1;
    
    HANDLE threadBubbleSort = CreateThread(NULL, 0, &func, &bubbleData, 0, NULL);

    HANDLE threadBubbleSort = CreateThread(NULL, 0, &bubbleSort, &bubbleData, 0, NULL);
    HANDLE threadQuickSort = CreateThread(NULL, 0,  &quickSort, &quickData, 0, NULL);

    WaitForSingleObject(threadBubbleSort, INFINITE);
    WaitForSingleObject(threadQuickSort, INFINITE);


    //bubbleSortThread.get_id();
    //bubbleSortThread.get_id();

    //bubbleSortThread.join();
    //quickSortThread.join();

    //bubbleSortThread.
}
