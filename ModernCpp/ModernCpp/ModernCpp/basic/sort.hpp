#pragma once
#include <iostream>
#include <cassert>

#define RANDOM_PARAMETER 100

void print(int a[], int n) {
    for (int j = 0; j < n; j++) {
        std::cout << a[j] << "  ";
    }
    std::cout << std::endl;
}

void swap(int * p1, int * p2) {
    *p1 ^= *p2;
    *p2 ^= *p1;
    *p1 ^= *p2;
}

void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

void sort(int *array, unsigned int num) {
    for (unsigned int i = 0; i < num; i++) {
        for (unsigned int j = num - 1; j > i; j--) {
            if (array[i] > array[j]) {
                swap(&array[i], &array[j]);
            }
        }
        print(array, num);
    }
}

void insertion_sort(int array[], int first, int last) {
    int i, j;
    int temp;
    for (i = first + 1; i <= last; i++) {
        temp = array[i];
        j = i - 1;

        //与已排序的数逐一比较，大于temp时，该数后移
        while ((j >= first) && (array[j] > temp)) //当first=0，j循环到-1时，由于[[短路求值]]，不会运算array[-1]
        {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = temp;      //被排序数放到正确的位置

    }
}

template<typename T>
struct list_node {
    struct list_node<T> *next;
    T value;
};

template<typename T>
struct _list {
    struct list_node<T> *head;
    int size;
};

template<typename T>
void sortlink(struct _list<T> * link) {
    struct list_node<T> *pHead, *pRear, *p, *tp;
    if (!link)
        return;
    for (pHead = link->head, pRear = 0; pHead; pHead = pHead->next) {
        for (tp = pHead, p = pHead->next; p; tp = p, p = p->next)
            if (pHead->value >= p->value)
                tp->next = p->next, p->next = pHead, pHead = p, p = tp;
        if (!pRear)
            link->head = pHead;
        else
            pRear->next = pHead;
        pRear = pHead;
    }
}

void bininsertsort(int a[], int n) {
    int key, left, right, middle;
    for (int i = 1; i < n; i++) {
        key = a[i];
        left = 0;
        right = i - 1;

        while (left <= right) {
            middle = (left + right) / 2;
            if (a[middle] > key)
                right = middle - 1;
            else
                left = middle + 1;
        }

        for (int j = i - 1; j >= left; j--) {
            a[j + 1] = a[j];
        }

        a[left] = key;
    }
}

void selectionsort(int *a, int len) {
    register int i, j, min;
    for (i = 0; i < len - 1; i++) {
        min = i;
        //查找最小值
        for (j = i + 1; j < len; j++)
            if (a[min] > a[j])
                min = j;
        //交换
        if (min != i) {
            swap(&a[min], &a[i]);
        }
    }
}

void cocktailsort(int *a, int nsize) {
    int tail = nsize - 1;
    for (int i = 0; i < tail;) {
        for (int j = tail; j > i; --j) //第一轮，先将最小的数据排到前面
                {
            if (a[j] < a[j - 1]) {
                int temp = a[j];
                a[j] = a[j - 1];
                a[j - 1] = temp;
            }
        }
        ++i;                    //原来i处数据已排好序，加1
        for (int j = i; j < tail; ++j)                    //第二轮，将最大的数据排到后面
                {
            if (a[j] > a[j + 1]) {
                int temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
        tail--;                 //原tail处数据也已排好序，将其减1
    }
}

void quicksort(int *arr, int left, int right) {
    int i, j;
    if (left < right) {
        i = left;
        j = right;
        arr[0] = arr[i]; //准备以本次最左边的元素值为标准进行划分，先保存其值
        do {
            while (arr[j] > arr[0] && i < j)
                j--;        //从右向左找第1个小于标准值的位置j
            if (i < j)        //找到了，位置为j
                    {
                arr[i] = arr[j];
                i++;
            }           //将第j个元素置于左端并重置i
            while (arr[i] < arr[0] && i < j)
                i++;           //从左向右找第1个大于标准值的位置i
            if (i < j)           //找到了，位置为i
                    {
                arr[j] = arr[i];
                j--;
            }           //将第i个元素置于右端并重置j
        } while (i != j);
        arr[i] = arr[0];         //将标准值放入它的最终位置,本次划分结束
        quicksort(arr, left, i - 1);         //对标准值左半部递归调用本函数
        quicksort(arr, i + 1, right);         //对标准值右半部递归调用本函数
    }
}

//取区间内随机数的函数
int rand(int low, int high) {
    int size = high - low + 1;
    return low + rand() % size;
}

//快排的partition算法，这里的基准数是随机选取的
int randpartition(int* data, int low, int high) {
    swap(data[rand(low, high)], data[low]);    //
    int key = data[low];
    int i = low;

    for (int j = low + 1; j <= high; j++) {
        if (data[j] <= key) {
            i = i + 1;
            swap(data[i], data[j]);
        }
    }
    swap(data[i], data[low]);
    return i;
}

//递归完成快速排序
void QuickSort(int* data, int low, int high) {
    if (low < high) {
        int k = randpartition(data, low, high);
        QuickSort(data, low, k - 1);
        QuickSort(data, k + 1, high);
    }
}

//将有二个有序数列a[first...mid]和a[mid...last]合并。
void MergeArray(int a[], int first, int mid, int last, int temp[]) {
    int i = first, j = mid + 1;
    int m = mid, n = last;
    int k = 0;
    while (i <= m && j <= n) {
        if (a[i] <= a[j])
            temp[k++] = a[i++];
        else
            temp[k++] = a[j++];
    }
    while (i <= m)
        temp[k++] = a[i++];
    while (j <= n)
        temp[k++] = a[j++];
    for (i = 0; i < k; i++)
        a[first + i] = temp[i];
}
//递归地完成归并排序
void MergeSort(int a[], int first, int last, int temp[]) {
    if (first < last) {
        int mid = (first + last) / 2;
        MergeSort(a, first, mid, temp);    //左边有序
        MergeSort(a, mid + 1, last, temp);    //右边有序
        MergeArray(a, first, mid, last, temp);    //再将二个有序数列合并
    }
}

void foo(int* p) {
    std::cout << *p << std::endl;
}

/*void test_sort() {
    int array[10] = { };
    for (int i = 0; i < 10; i++)
        array[i] = Random::uniform(RANDOM_PARAMETER);

    for (auto i : array)
        std::cout << i << " ";
    std::cout << std::endl;

    QuickSort(array, 0, 9);
    //	sort(array, 10);
    for (auto i : array)
        std::cout << i << " ";
}*/
