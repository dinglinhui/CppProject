/*
#include <iostream>
using namespace std;

void quick_sort(int a[], int left, int right) {
	int temp = a[left];
	int i = left, j = right;
	if (left < right) {
		while (i < j) {
			while (a[j] > temp && i < j)
				j--;
			if (i < j)
				a[i++] = a[j];
			while (a[i] < temp && i < j)
				i++;
			if (i < j)
				a[j--] = a[i];
		}
		a[i] = temp;
		quick_sort(a, left, i - 1);
		quick_sort(a, i + 1, right);
	}
}
//修整最小堆
void fix_min_heap(int a[], int pos, int len) {
	int left = 2 * pos + 1;
	int right = 2 * pos + 2;
	int min = pos;
	if (left < len && a[left] < a[pos])
		min = left;
	if (right < len && a[right] < a[min])
		min = right;
	if (pos != min) {
		int tep = a[pos];
		a[pos] = a[min];
		a[min] = tep;
		fix_min_heap(a, min, len);
	}
}
//建立最小堆
void build_min_heap(int a[], int len) {
	int parent = (len - 1) / 2;
	int i = 0, j = 0;
	for (i = parent; i >= 0; --i) {
		int left = 2 * i + 1;
		int right = 2 * i + 2;
		int min = i;
		if (left < len && a[left] < a[i])
			min = left;
		if (right < len && a[right] < a[min])
			min = right;
		if (i != min) {
			int tep = a[i];
			a[i] = a[min];
			a[min] = tep;
			fix_min_heap(a, min, len);
		}
	}
}

int main() {
	int total[100000];
	int min_heap[10];
	int n, m, i = 0, j = 0;
	while (cin >> n >> m) {
		if (m == 0 && n == 0)
			break;
		for (i = 0; i < n; i++)
			cin >> total[i];
		for (j = 0; j < m; j++)
			min_heap[j] = total[j];
		build_min_heap(min_heap, m);
		for (i = m; i < n; i++) {
			if (min_heap[0] < total[i]) {
				min_heap[0] = total[i];
				fix_min_heap(min_heap, 0, m);
			}
		}
		quick_sort(min_heap, 0, m - 1);
		for (i = m - 1; i > 0; --i)
			cout << min_heap[i] << " ";
		cout << min_heap[0] << endl;
	}
}*/
