#include <iostream>
#include <Windows.h>
#include <thread>
#include <vector>
#include <chrono>
#include <windef.h>
#include <processthreadsapi.h>

using namespace std;

const int ARRAY_SIZE = 30000;

void fillArray(vector<int>& arr, int size) {
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


    HANDLE threadQuickSortHandle = CreateThread(NULL, 0, &quickSort, &quickData, CREATE_SUSPENDED, NULL);
    HANDLE threadBubbleSortHandle = CreateThread(NULL, 0, &bubbleSort, &bubbleData, CREATE_SUSPENDED, NULL);

    SetProcessPriorityBoost(threadQuickSortHandle, 0);
    SetProcessPriorityBoost(threadBubbleSortHandle, 0);


    SetThreadPriority(threadBubbleSortHandle, THREAD_PRIORITY_HIGHEST);
    SetThreadPriority(threadQuickSortHandle, THREAD_PRIORITY_LOWEST);

    ResumeThread(threadQuickSortHandle);
    ResumeThread(threadBubbleSortHandle);

    WaitForSingleObject(threadQuickSortHandle, INFINITE);
    WaitForSingleObject(threadBubbleSortHandle, INFINITE);

    TerminateThread(threadQuickSortHandle, 0);
    TerminateThread(threadBubbleSortHandle, 0);

    cout << "Quick sort:" << endl;
    printThreadTimes(threadQuickSortHandle);

    cout << "Bubble sort:" << endl;
    printThreadTimes(threadBubbleSortHandle);

    CloseHandle(threadBubbleSortHandle);
    CloseHandle(threadQuickSortHandle);
}





//SetProcessPriorityBoost
//SetProcessAffinityMask



