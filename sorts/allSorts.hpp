#pragma once
#include <vector>
using namespace std;
 
static inline bool bubbleSort(vector<int>& arr, int& i, int& j,  int& toSwap)
{
    int n = arr.size();
    if(i >= n)
        return true;
    for (; i < n - 1; i++){
        for (; j < n - i - 1; j++){
            if (arr[j] > arr[j + 1]){
                swap(arr[j], arr[j + 1]);
                toSwap = j+1;
                return false;
            }
        }
        j = 0;
    }
    return true;
}

static inline bool selectionSort(vector<int>& arr, int& i, int& toSwap)
{
    int n = arr.size();
    if(i >= n)
        return true;
     for (; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        std::swap(arr[i], arr[minIndex]);
        toSwap = minIndex;
        i++;
        return false;
    }
    return true;
}

static inline bool insertionSort(vector<int>& arr, int& i, int& j, int& toSwap)
{
    int n = arr.size();
    if(i >= n)
        return true;
    int key;
    for(; i < n; i++){
        
        key = arr[i];
        while (j >= 0 && arr[j] > arr[j+1])
        {
            swap(arr[j], arr[j+1]);
            toSwap = j;
            j--;
            return false;
        }
        j = i;
        i++;
        toSwap = j;
        return false;
    }
    return true;
}