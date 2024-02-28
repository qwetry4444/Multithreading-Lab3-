#include <iostream>
#include <Windows.h>
#include <thread>
#include <vector>
#include <chrono>
#include <windef.h>
#include <processthreadsapi.h>

using namespace std;

const int ARRAY_SIZE = 10000;

void fillArray(vector<int> &arr, int size) {
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
    bubbleSortData sortData = *(bubbleSortData*)data;
    for (int i = 0; i < sortData.size - 1; ++i) {
        for (int j = 0; j < sortData.size - i - 1; ++j) {
            if (sortData.arr[j] > sortData.arr[j + 1]) {
                swap(sortData.arr[j], sortData.arr[j + 1]);
            }
        }
    }
    return 0;
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
    quickSortData sortData = *(quickSortData*)data;
    if (sortData.low < sortData.high) {
        int pivotIndex = partition(sortData.arr, sortData.low, sortData.high);

        quickSortData newData;
        newData.arr = sortData.arr;
        newData.low = sortData.low;
        newData.high = pivotIndex - 1;
        quickSort(&newData);

        newData.low = pivotIndex + 1;
        newData.high = sortData.high;
        quickSort(&newData);
    }
    return 0;
}

void printThreadTimes(HANDLE threadBubbleSortHandle)
{
    FILETIME fileTime1, fileTime2, fileTime3, fileTime4;
    GetThreadTimes(threadBubbleSortHandle, &fileTime1, &fileTime2, &fileTime3, &fileTime4);

    SYSTEMTIME sysTime1, sysTime2, sysTime3, sysTime4;
    FileTimeToSystemTime(&fileTime1, &sysTime1);
    FileTimeToSystemTime(&fileTime2, &sysTime2);
    FileTimeToSystemTime(&fileTime3, &sysTime3);
    FileTimeToSystemTime(&fileTime4, &sysTime4);

    cout << "CreationTime = " << sysTime1.wHour << "hour " << sysTime1.wMinute << "min " << sysTime1.wSecond << "sec " << sysTime1.wMilliseconds << "ms" << endl;
    cout << "ExitTime = " << sysTime2.wHour << "hour " << sysTime2.wMinute << "min " << sysTime2.wSecond << "sec " << sysTime2.wMilliseconds << "ms" << endl;
    cout << "KernelTime = " << sysTime3.wMilliseconds << " ms" << endl;
    cout << "UserTime = " << sysTime4.wMilliseconds << " ms" << endl << endl;
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


    cout << "Bubble sort:" << endl;
    HANDLE threadBubbleSortHandle = CreateThread(NULL, CREATE_SUSPENDED, &bubbleSort, &bubbleData, 0, NULL);
    ResumeThread(threadBubbleSortHandle);
    SetThreadPriority(threadBubbleSortHandle, THREAD_PRIORITY_LOWEST);
    WaitForSingleObject(threadBubbleSortHandle, INFINITE);
    TerminateThread(threadBubbleSortHandle, 0);
    printThreadTimes(threadBubbleSortHandle);
    
    cout << "Quick sort:" << endl;
    HANDLE threadQuickSortHandle = CreateThread(NULL, CREATE_SUSPENDED, &quickSort, &quickData, 0, NULL);
    ResumeThread(threadQuickSortHandle);
    SetThreadPriority(threadQuickSortHandle, THREAD_PRIORITY_HIGHEST);
    WaitForSingleObject(threadQuickSortHandle, INFINITE);
    TerminateThread(threadQuickSortHandle, 0);
    printThreadTimes(threadQuickSortHandle);

    CloseHandle(threadBubbleSortHandle);
    CloseHandle(threadQuickSortHandle);
}
